/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:25:52 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/25 15:13:17 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*trim_whitespace(char *str)
{
	char	*end;

	while (isspace(*str))
		str++;
	if (*str == '\0')
		return (str);
	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
		end--;
	*(end + 1) = '\0';
	return (str);
}

int	tokenize_commands(char *input, char *commands[], int max_commands)
{
	char	*token;
	int		num_commands;
	char	*trimmed_token;

	num_commands = 0;
	token = strtok(input, "|");
	while (token != NULL && num_commands < max_commands)
	{
		trimmed_token = trim_whitespace(token);
		commands[num_commands] = trimmed_token;
		num_commands++;
		token = strtok(NULL, "|");
	}
	return (num_commands);
}

void	execute_pipeline(char *input)
{
	char	*commands[MAX_ARGS];
	int		num_commands;

	num_commands = tokenize_commands(input, commands,
			MAX_ARGS);
	exec_commands(commands, num_commands);
	free(input);
}
