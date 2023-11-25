/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 09:19:19 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 16:50:58 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	split_pipe_command(char *input, char *commands[MAX_ARGS],
		int *num_commands)
{
	char	*token;

	token = ft_strtok(input, "|");
	*num_commands = 0;
	while (token != NULL && *num_commands < MAX_ARGS)
	{
		commands[*num_commands] = token;
		(*num_commands)++;
		token = ft_strtok(NULL, "|");
	}
}

void	parent_process(int pipe_fds[2], int *input_fd)
{
	close(pipe_fds[1]);
	wait(NULL);
	*input_fd = pipe_fds[0];
}

void	setup_child_process(int i, int num_commands, int pipe_fds[2],
		int input_fd)
{
	if (i < num_commands - 1)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
	}
	if (i > 0)
		dup2(input_fd, STDIN_FILENO);
}
