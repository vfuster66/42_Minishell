/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_internal_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:29:07 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 10:26:52 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(const char *command)
{
	char	*command_copy;
	char	*token;

	command_copy = ft_strdup(command);
	token = ft_strtok(command_copy, " \t\n");
	if (ft_strcmp(token, "echo") == 0
		|| ft_strcmp(token, "cd") == 0
		|| ft_strcmp(token, "pwd") == 0
		|| ft_strcmp(token, "export") == 0
		|| ft_strcmp(token, "unset") == 0
		|| ft_strcmp(token, "env") == 0
		|| ft_strcmp(token, "exit") == 0)
	{
		free(command_copy);
		return (1);
	}
	free(command_copy);
	return (0);
}

void	execute_export_builtin(char **arguments)
{
	char	*var_name;
	char	*var_value;

	if (arguments[1] != NULL)
	{
		while (*arguments[1] == ' ')
			arguments[1]++;
		if (*arguments[1] != '\0')
		{
			var_name = arguments[1];
			var_value = ft_strchr(var_name, '=');
			if (var_value != NULL)
			{
				*var_value = '\0';
				var_value++;
			}
			else
				var_value = NULL;
			if (var_name != NULL && *var_name != '\0')
				builtin_export(var_name, var_value);
			return ;
		}
	}
	print_all_environment();
}

void	execute_unset_builtin(char **arguments)
{
	int	i;

	i = 1;
	while (arguments[i] != NULL)
	{
		builtin_unset(arguments[i]);
		i++;
	}
}

void	execute_builtin(const char *command, char **arguments, t_status *status)
{
	if (ft_strcmp(command, "echo") == 0)
		builtin_echo(arguments);
	else if (ft_strcmp(command, "cd") == 0)
		builtin_cd(arguments[1]);
	else if (ft_strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command, "export") == 0)
		execute_export_builtin(arguments);
	else if (ft_strcmp(command, "unset") == 0)
		execute_unset_builtin(arguments);
	else if (ft_strcmp(command, "env") == 0)
		builtin_env();
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit(arguments);
	status->previous_command_status = 0;
}
