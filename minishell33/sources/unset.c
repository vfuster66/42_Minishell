/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:26:01 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 16:53:51 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void remove_env_variable(char *var_name)
{
    int i;
    char *env_var;

    i = 0;
    while (g_environment[i] != NULL)
	{
        env_var = g_environment[i];
        if (strncmp(env_var, var_name, strlen(var_name)) == 0 && env_var[strlen(var_name)] == '=')
		{
            free(g_environment[i]);
            while (g_environment[i + 1] != NULL)
			{
                g_environment[i] = g_environment[i + 1];
                i++;
            }
            g_environment[i] = NULL;
            return ;
        }
        i++;
    }
}

void builtin_unset(char *var_name)
{
    if (var_name != NULL)
        remove_env_variable(var_name);
    else
        printf("unset: missing argument\n");
}
