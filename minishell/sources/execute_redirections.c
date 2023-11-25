/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:20:37 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/15 11:20:43 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	input_case(char *args[], char *file_info[],
		int *stdin_backup, int *stdout_backup)
{
	if (!handle_input_redirection(args, file_info, stdin_backup, stdout_backup))
	{
		perror("input_case");
		exit(EXIT_FAILURE);
	}
}

void	output_case(char *args[], char *file_info[],
		int *stdin_backup, int *stdout_backup)
{
	if (!handle_output_redirection(args, file_info,
			stdin_backup, stdout_backup))
	{
		perror("output_case");
		exit(EXIT_FAILURE);
	}
}

void	append_case(char *args[], char *file_info[],
		int *stdin_backup, int *stdout_backup)
{
	if (!handle_output_append_redirection(args, file_info,
			stdin_backup, stdout_backup))
	{
		perror("append_case");
		exit(EXIT_FAILURE);
	}
}

void	redirections(char *args[], char *file_info[],
		int *stdin_backup, int *stdout_backup)
{
	if (file_info[0] != NULL)
		input_case(args, file_info, stdin_backup, stdout_backup);
	else if (file_info[1] != NULL)
	{
		if (file_info[3] != NULL && ft_strcmp(file_info[3], "true") == 0)
			append_case(args, file_info, stdin_backup, stdout_backup);
		else
			output_case(args, file_info, stdin_backup, stdout_backup);
	}
	else if (file_info[2] != NULL)
		handle_input_redirection_delimiter(file_info[2], file_info);
}
