/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:22:30 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 14:26:14 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_command_with_redirections(char *input,
		t_status *status, int *stdin_backup, int *stdout_backup)
{
	char	*args[MAX_ARGS];
	char	*file_info[4];
	char	*processed_input;

	processed_input = process_quote(input);
	process_and_handle_special_commands(input, &status->exit_requested,
		processed_input, g_environment);
	split_redirections(processed_input, args, MAX_ARGS, file_info);
	redirections(args, file_info, stdin_backup, stdout_backup);
	execute_command_internal_or_external(args, status);
	restore_io_and_exit(*stdin_backup, *stdout_backup, file_info);
	free(processed_input);
	free(input);
}

void	execute_command_without_redirections(char *input,
		t_status *status, int *stdin_backup, int *stdout_backup)
{
	char	*args[MAX_ARGS];
	char	*file_info[4];
	char	*processed_input;

	processed_input = process_quote(input);
	process_and_handle_special_commands(input, &status->exit_requested,
		processed_input, g_environment);
	split_and_check_command(input, args, MAX_ARGS, file_info);
	redirections(args, file_info, stdin_backup, stdout_backup);
	execute_command_internal_or_external(args, status);
	restore_io_and_exit(*stdin_backup, *stdout_backup, file_info);
	free(processed_input);
	free(input);
}

void	execute_command(char *input, t_status *status)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = -1;
	stdout_backup = -1;
	if ((ft_strstr(input, "\"") != NULL || ft_strstr(input, "\'") != NULL)
		&& ft_contains_special_chars(input))
	{
	}
	else if (ft_strchr(input, '|') != NULL)
	{
		execute_pipeline(input);
		return ;
	}
	else if (ft_strchr(input, '<') || ft_strchr(input, '>')
		|| ft_strstr(input, "<<") || ft_strstr(input, ">>"))
	{
		execute_command_with_redirections(input, status,
			&stdin_backup, &stdout_backup);
		return ;
	}
	execute_command_without_redirections(input, status,
		&stdin_backup, &stdout_backup);
}

void	process_and_handle_special_commands(char *input, int *exit_requested,
		char *processed_input, char **g_environment)
{
	(void)exit_requested;
	(void)g_environment;
	if (ft_strcmp(processed_input, "clear_history") == 0)
	{
		clear_history_command();
		free(processed_input);
		free(input);
		return ;
	}
	if (ft_strcmp(processed_input, "exit") == 0)
		return ;
}
