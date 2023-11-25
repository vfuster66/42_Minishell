/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 11:12:23 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 09:13:09 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	redirect_input(char *input_file)
{
	int	fd_input;

	if (input_file != NULL)
	{
		fd_input = open(input_file, O_RDONLY);
		if (fd_input == -1)
		{
			perror("open");
			return (false);
		}
		if (dup2(fd_input, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (false);
		}
		close(fd_input);
	}
	return (true);
}

bool	open_input_redirection(char *input_file, int *stdin_backup,
		int *stdout_backup)
{
	int	fd;

	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdin_backup == -1 || *stdout_backup == -1)
	{
		perror("dup");
		return (false);
	}
	fd = open(input_file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (true);
}

bool	handle_input_redirection(char *args[], char *file_info[4],
		int *stdin_backup, int *stdout_backup)
{
	bool	redirection;
	int		i;

	redirection = false;
	i = 0;
	while (args[i] != NULL)
	{
		if (ft_strcmp(args[i], "<") == 0)
		{
			redirection = (true);
			continue ;
		}
		if (redirection)
		{
			file_info[0] = args[i];
			redirection = (false);
		}
		i++;
	}
	if (file_info[0] != NULL)
		return (open_input_redirection(file_info[0],
				stdin_backup, stdout_backup));
	return (false);
}

bool	configure_input_redirection(char **arguments, char *file_info[4])
{
	int	i;

	i = 0;
	while (arguments[i] != NULL)
	{
		if (ft_strcmp(arguments[i], "<") == 0)
		{
			file_info[0] = arguments[i + 1];
			arguments[i] = NULL;
			return (true);
		}
		i++;
	}
	return (false);
}
