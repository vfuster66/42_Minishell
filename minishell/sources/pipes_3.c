/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 15:22:00 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 10:07:44 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_redirection_symbols(char *command)
{
	return (ft_strstr(command, ">") || ft_strstr(command, "<")
		|| ft_strstr(command, ">>") || ft_strstr(command, "<<"));
}

void	parse_command(char *command, char *args[],
		int max_args, char *file_info[])
{
	initialize_file_info(file_info);
	if (has_redirection_symbols(command))
	{
		if (split_redirections(command, args, max_args, file_info) < 0)
		{
			printf("Error parsing command with redirections: %s\n", command);
			exit(1);
		}
	}
	else
	{
		if (split_command(command, args, max_args, file_info) < 0)
		{
			printf("Error parsing command: %s\n", command);
			exit(1);
		}
	}
}

int	parse_and_execute_command(char *command, int cmd_info[4], int *status)
{
	char	*args[MAX_ARGS];
	char	*file_info[4];
	pid_t	child_pid;
	int		pipe_fds[2];
	int		child_status;

	parse_command(command, args, MAX_ARGS, file_info);
	pipe(pipe_fds);
	child_pid = fork();
	if (child_pid == 0)
	{
		setup_child_process(cmd_info[1], cmd_info[2], pipe_fds, cmd_info[0]);
		child_status = execute_pipe_command(args, file_info[0],
				file_info[1], file_info[3]);
		*status = child_status;
		exit(0);
	}
	else
	{
		parent_process(pipe_fds, &cmd_info[0]);
		return (pipe_fds[0]);
	}
}

void	exec_commands(char *commands[], int num_commands)
{
	int	input_fd;
	int	status;
	int	i;
	int	cmd_info[4];

	input_fd = 0;
	status = 0;
	i = 0;
	while (i < num_commands)
	{
		cmd_info[0] = input_fd;
		cmd_info[1] = i;
		cmd_info[2] = num_commands;
		input_fd = parse_and_execute_command(commands[i], cmd_info, &status);
		i++;
	}
}
