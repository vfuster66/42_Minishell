/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:38:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/08 15:04:18 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	open_output_file(char *output_file, int *stdout_backup,
		int *fd, int flags)
{
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdout_backup == -1)
	{
		perror("dup");
		return (false);
	}
	*fd = open(output_file, flags, 0666);
	if (*fd == -1)
	{
		perror("open");
		return (false);
	}
	if (dup2(*fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		return (false);
	}
	return (true);
}

void	find_output_file(char *args[], char **output_file,
		int *i, const char *redirection)
{
	while (args[*i] != NULL)
	{
		if (strcmp(args[*i], redirection) == 0)
		{
			(*i)++;
			if (args[*i] != NULL)
				*output_file = args[*i];
			break ;
		}
		(*i)++;
	}
}

bool	handle_output_redirection(char *args[], char **output_file,
		int *stdin_backup, int *stdout_backup)
{
	(void)stdin_backup;
	int	i;
	int	fd;

	i = 0;
	find_output_file(args, output_file, &i, ">");
	if (*output_file != NULL)
	{
		if (!open_output_file(*output_file, stdout_backup,
					&fd, O_WRONLY | O_CREAT | O_TRUNC))
			exit(EXIT_FAILURE);
		close(fd);
		return (true);
	}
	return (false);
}

bool	handle_output_append_redirection(char *args[],
		char **output_file, int *stdin_backup, int *stdout_backup)
{
	(void)stdin_backup;
	int	i;
	int	fd;

	i = 0;
	find_output_file(args, output_file, &i, ">>");
	if (*output_file != NULL)
	{
		if (!open_output_file(*output_file, stdout_backup,
					&fd, O_WRONLY | O_CREAT | O_APPEND))
			exit(EXIT_FAILURE);
		close(fd);
		return (true);
	}
	return (false);
}
