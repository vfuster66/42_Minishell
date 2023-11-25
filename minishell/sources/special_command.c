/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 08:41:22 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/25 15:23:45 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_variable_occurrence(char **str, char **dollar_sign,
		char *g_environment[], bool *is_literal)
{
	const char	*end;
	char		*var_name;
	char		*var_value;

	end = find_variable_end(*dollar_sign);
	var_name = extract_variable_name(*dollar_sign, end);
	if (*dollar_sign > *str && *(*dollar_sign - 1) == '\'')
		*is_literal = !*is_literal;
	else if (!*is_literal)
	{
		var_value = find_environment_variable(var_name, g_environment);
		if (var_value != NULL)
		{
			substitute_variable_value(str, *dollar_sign, var_value, end);
			end = *str + (end - *dollar_sign) + strlen(var_value);
		}
	}
	free(var_name);
	*dollar_sign = strchr(end, '$');
}

void	substitute_environment_variables_in_string(char **str,
		char *g_environment[])
{
	char	*dollar_sign;
	bool	is_literal;

	dollar_sign = ft_strchr(*str, '$');
	is_literal = false;
	while (dollar_sign != NULL)
		process_variable_occurrence(str, &dollar_sign,
			g_environment, &is_literal);
}

char	*substitute_environment_variables(char *input,
		char *g_environment[])
{
	char	*output;

	output = ft_strdup(input);
	if (output == NULL)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	substitute_environment_variables_in_string(&output, g_environment);
	return (output);
}

bool	is_literal_quote(char c)
{
	return (c == '\'' || c == '"');
}

char	*substitute_question_mark(char *input,
		int previous_command_status)
{
	char	*dollar_question;
	char	*new_input;
	char	*status_str;
	size_t	prefix_length;

	dollar_question = ft_strstr(input, "$?");
	if (dollar_question != NULL && (dollar_question == input
			|| !is_literal_quote(*(dollar_question - 1))))
	{
		new_input = (char *)malloc(ft_strlen(input) + 12);
		if (new_input != NULL)
		{
			prefix_length = dollar_question - input;
			ft_strncpy(new_input, input, prefix_length);
			new_input[prefix_length] = '\0';
			status_str = ft_itoa(previous_command_status);
			ft_strcat(new_input, status_str);
			free(status_str);
			ft_strcat(new_input, dollar_question + 2);
			return (new_input);
		}
		free(new_input);
	}
	return (input);
}
