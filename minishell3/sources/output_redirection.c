/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:38:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/06 11:46:52 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	handle_output_redirection(char *args[], char *output_file,
		int *stdin_backup, int *stdout_backup)
{
	bool	redirection;
	int		i;
	int		fd;

	redirection = false;
	i = 0;
	while (args[i] != NULL)
	{
		if (strcmp(args[i], ">") == 0)
		{
			redirection = true;
			continue ;
		}
		if (redirection)
		{
			output_file = args[i];
			redirection = false;
		}
		i++;
	}
	return (perform_output_redirection(output_file,
			stdin_backup, stdout_backup));
}

bool	redirect_stdout(int *stdout_backup, const char *output_file)
{
	int	fd;

	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdout_backup == -1)
	{
		perror("dup");
		return (false);
	}
	fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (true);
}

bool	perform_output_redirection(char *output_file,
		int *stdin_backup, int *stdout_backup)
{
	if (output_file != NULL)
	{
		*stdin_backup = dup(STDIN_FILENO);
		if (*stdin_backup == -1)
		{
			perror("dup");
			return (false);
		}
		if (!redirect_stdout(stdout_backup, output_file))
			return (false);
		return (true);
	}
	return (false);
}
