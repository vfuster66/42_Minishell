/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:29:07 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/06 10:11:43 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	est_builtin(const char *command)
{
	char	*command_copy;
	char	*token;

	command_copy = strdup(command);
	token = strtok(command_copy, " \t\n");
	if (strcmp(token, "echo") == 0
		|| strcmp(token, "cd") == 0
		|| strcmp(token, "pwd") == 0
		|| strcmp(token, "export") == 0
		|| strcmp(token, "unset") == 0
		|| strcmp(token, "env") == 0
		|| strcmp(token, "exit") == 0)
	{
		free(command_copy);
		return (1);
	}
	return (0);
}

void	execute_export_builtin(char **arguments)
{
	char	*var_value;
	char	*var_name;

	if (arguments[1] != NULL)
	{
		var_name = strtok(arguments[1], "=");
		var_value = strtok(NULL, "=");
		builtin_export(var_name, var_value);
	}
	else
		printf("export: missing argument\n");
}

void	execute_unset_builtin(char **arguments)
{
	if (arguments[1] != NULL)
		builtin_unset(arguments[1]);
	else
		printf("unset: missing argument\n");
}

void	execute_builtin(const char *command, char **arguments)
{
	char	*var_value;
	char	*var_name;

	if (strcmp(command, "echo") == 0)
		builtin_echo(arguments);
	else if (strcmp(command, "cd") == 0)
		builtin_cd(arguments[1]);
	else if (strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (strcmp(command, "export") == 0)
		execute_export_builtin(arguments);
	else if (strcmp(command, "unset") == 0)
		execute_unset_builtin(arguments);
	else if (strcmp(command, "env") == 0)
		builtin_env();
	else if (strcmp(command, "exit") == 0)
		builtin_exit(0);
}
