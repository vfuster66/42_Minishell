/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:21:54 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 14:55:42 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t	launch_child_process(char *full_path, char **arguments,
		char **g_environment)
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (child_pid == 0)
	{
		if (execve(full_path, arguments, g_environment) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	return (child_pid);
}

bool	wait_for_child_process(pid_t child_pid, int *status)
{
	if (waitpid(child_pid, status, 0) == -1)
	{
		perror("waitpid");
		return (false);
	}
	return (true);
}

bool	execute_command_from_path(char *full_path, char **arguments,
		char **g_environment, t_status *status)
{
	pid_t	child_pid;
	int		child_status;

	child_pid = launch_child_process(full_path, arguments, g_environment);
	if (child_pid == -1)
		return (false);
	if (!wait_for_child_process(child_pid, &child_status))
		return (false);
	if (WIFEXITED(child_status))
		status->previous_command_status = WEXITSTATUS(child_status);
	else
		status->previous_command_status = -1;
	return (true);
}

void	execute_external_command(char *command, char **arguments,
		char **g_environment, t_status *status)
{
	char	*full_path;
	bool	execution_success;

	execution_success = false;
	full_path = search_command_path(command, g_environment);
	if (full_path != NULL)
	{
		execution_success = execute_command_from_path(full_path, arguments,
				g_environment, status);
		free(full_path);
		if (!execution_success)
		{
			printf("Execution of command '%s' failed.\n", command);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		printf("Command not found\n");
		return ;
	}
}
