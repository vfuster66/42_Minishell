/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 15:22:00 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 10:08:05 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pipe_input_redirection(const char *input_file)
{
	int	input_fd;

	if (input_file != NULL)
	{
		input_fd = open(input_file, O_RDONLY);
		if (input_fd < 0)
		{
			perror("open");
			exit(1);
		}
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
}

void	pipe_output_redirection(const char *output_file, bool append_output)
{
	int	flags;
	int	output_fd;

	if (output_file != NULL)
	{
		flags = O_WRONLY | O_CREAT;
		if (append_output)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		output_fd = open(output_file, flags, 0666);
		if (output_fd < 0)
		{
			perror("open");
			exit(1);
		}
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

int	pipe_execute_command(char *command_path, char *args[])
{
	if (command_path != NULL)
	{
		execve(command_path, args, g_environment);
		perror("execve");
		exit(1);
	}
	else
	{
		printf("Command not found: %s\n", args[0]);
		exit(1);
	}
}

int	execute_pipe_command(char *args[], char *input_file,
		char *output_file, bool append_output)
{
	char	*path;
	int		child_status;

	pipe_input_redirection(input_file);
	pipe_output_redirection(output_file, append_output);
	path = search_command_path(args[0], g_environment);
	child_status = pipe_execute_command(path, args);
	return (child_status);
}
