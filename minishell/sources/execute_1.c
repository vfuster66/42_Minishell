/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 09:34:35 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/25 16:07:11 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*perform_substitutions(char *input, t_status *status)
{
	char	*temp_input;	

	temp_input = substitute_question_mark(input,
			status->previous_command_status);
	if (temp_input != input)
	{
		free(input);
		input = temp_input;
	}
	temp_input = substitute_environment_variables(input, g_environment);
	if (temp_input != input)
	{
		free(input);
		input = temp_input;
	}
	return (input);
}

char	*process_user_input(char *input, t_status *status)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (!is_whitespace(input[i]))
			break ;
		i++;
	}
	if (input[i] == '\0')
	{
		free(input);
		return (NULL);
	}
	return (perform_substitutions(input, status));
}

int	split_and_check_command(char *input, char *args[],
		int max_args, char *file_info[4])
{
	int	num_args;

	num_args = split_command(input, args, max_args, file_info);
	if (num_args == -1)
		return (-1);
	return (num_args);
}

void	execute_command_internal_or_external(char *args[], t_status *status)
{
	if (is_builtin(args[0]))
		execute_builtin(args[0], args, status);
	else
		execute_external_command(args[0], args, g_environment, status);
}

int	ft_contains_special_chars(char *input)
{
	while (*input)
	{
		if (ft_isspecialchar(*input) && *input != '\"')
			return (1);
		++input;
	}
	return (0);
}
