/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:15:48 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 09:13:00 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	redirect_output(char *output_file, bool append_output)
{
	int	fd_output;
	int	flags;

	if (output_file != NULL)
	{
		if (append_output)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd_output = open(output_file, flags, 0666);
		if (fd_output == -1)
		{
			perror("open");
			return (false);
		}
		if (dup2(fd_output, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (false);
		}
		close(fd_output);
	}
	return (true);
}

bool	handle_redirections(char *output_file, char *input_file,
		bool append_output)
{
	bool	output_success;
	bool	input_success;

	output_success = redirect_output(output_file, append_output);
	input_success = redirect_input(input_file);
	return (output_success && input_success);
}

bool	configure_redirections(char **arguments, char *file_info[4])
{
	return (configure_input_redirection(arguments, file_info)
		|| configure_output_redirection(arguments, file_info));
}

void	process_redirection(char *token, char *args[],
		int *i, char *file_info[4])
{
	if (detect_redirection(token, file_info))
	{
		if (file_info[0] && token != NULL)
		{
			file_info[INPUT_FILE] = token;
			token = ft_strtok(NULL, " ");
		}
	}
	else if (strlen(token) > 0)
	{
		args[*i] = token;
		(*i)++;
	}
}

int	split_redirections(char *input, char *args[],
		int max_args, char *file_info[4])
{
	int		i;
	char	*token;

	i = 0;
	initialize_file_info(file_info);
	token = ft_strtok(input, " ");
	while (token != NULL && i < max_args - 1)
	{
		process_redirection(token, args, &i, file_info);
		token = ft_strtok(NULL, " ");
	}
	args[i] = NULL;
	if (i == 0)
	{
		printf("Erreur : Aucune commande entrée.\n");
		return (-1);
	}
	return (i);
}
