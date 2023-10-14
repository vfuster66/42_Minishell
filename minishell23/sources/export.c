/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:24:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/14 17:38:35 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*create_env_variable(const char *var_name, const char *var_value)
{
    int	var_name_len;
    int	var_value_len;
    char	*new_env_var;

	var_name_len = strlen(var_name);
    var_value_len = strlen(var_value);
    new_env_var = (char *)malloc(var_name_len + var_value_len + 2);
	if (new_env_var == NULL)
    {
        perror("export");
        exit(EXIT_FAILURE);
    }
    strcpy(new_env_var, var_name);
    strcat(new_env_var, "=");
    strcat(new_env_var, var_value);
    return (new_env_var);
}

void	update_env_variable(char *var_name, const char *var_value)
{
	char	*env_var;

	env_var = getenv(var_name);
    if (env_var != NULL)
    {
        if (setenv(var_name, var_value, 1) != 0)
            perror("export");
    }
    else
        printf("export: variable not found in environment\n");
}

void add_env_variable(const char *var_name, const char *var_value)
{
    char    *new_env_var;
    int i;

    new_env_var = create_env_variable(var_name, var_value);
    if (setenv(var_name, var_value, 1) != 0)
    {
        perror("export");
    }
    i = 0;
    while (my_environment[i] != NULL)
    {
        i++;
    }
    my_environment[i] = new_env_var;
}

void	builtin_export(char *var_name, char *var_value)
{
    if (var_name != NULL)
    {
        int	i;

		i = 0;
        while (my_environment[i] != NULL)
        {
            char *env_var = my_environment[i];
            if (strncmp(env_var, var_name, strlen(var_name)) == 0 && env_var[strlen(var_name)] == '=')
            {
                update_env_variable(var_name, var_value);
                return ;
            }
            i++;
        }
        add_env_variable(var_name, var_value);
    }
    else
        printf("export: missing arguments\n");
}

void execute_export(char **arguments)
{
    char *var_value;
    char *var_name;

    if (arguments[1] != NULL)
    {
        var_name = strtok(arguments[1], "=");
        var_value = strtok(NULL, "=");
        builtin_export(var_name, var_value);
    }
    else
    {
        printf("export: missing argument\n");
    }
}
