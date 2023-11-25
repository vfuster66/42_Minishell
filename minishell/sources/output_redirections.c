/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:38:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/02 07:19:35 by vfuster-         ###   ########.fr       */
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
		if (ft_strcmp(args[*i], redirection) == 0)
		{
			(*i)++;
			if (args[*i] != NULL)
				*output_file = args[*i];
			break ;
		}
		(*i)++;
	}
}

bool	handle_output_redirection(char *args[], char *file_info[4],
		int *stdin_backup, int *stdout_backup)
{
	int	i;
	int	fd;

	(void)stdin_backup;
	i = 0;
	find_output_file(args, &file_info[1], &i, ">");
	if (file_info[1] != NULL)
	{
		if (!open_output_file(file_info[1], stdout_backup,
				&fd, O_WRONLY | O_CREAT | O_TRUNC))
			exit(EXIT_FAILURE);
		close(fd);
		return (true);
	}
	return (false);
}

bool	handle_output_append_redirection(char *args[], char *file_info[4],
		int *stdin_backup, int *stdout_backup)
{
	int	i;
	int	fd;

	(void)stdin_backup;
	i = 0;
	find_output_file(args, &file_info[1], &i, ">>");
	if (file_info[1] != NULL)
	{
		if (!open_output_file(file_info[1], stdout_backup,
				&fd, O_WRONLY | O_CREAT | O_APPEND))
			exit(EXIT_FAILURE);
		close(fd);
		return (true);
	}
	return (false);
}

bool	configure_output_redirection(char **arguments, char *file_info[4])
{
	int	i;

	i = 0;
	while (arguments[i] != NULL)
	{
		if (ft_strcmp(arguments[i], ">") == 0)
		{
			file_info[1] = arguments[i + 1];
			arguments[i] = NULL;
			return (true);
		}
		else if (ft_strcmp(arguments[i], ">>") == 0)
		{
			file_info[1] = arguments[i + 1];
			arguments[i] = NULL;
			file_info[3] = "true";
			return (true);
		}
		i++;
	}
	return (false);
}
