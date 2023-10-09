/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:26:01 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/04 11:13:52 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_env_variable(char *var_name)
{
	int		i;
	char	*env_var;

	i = 0;
	while (my_environment[i] != NULL)
	{
		env_var = my_environment[i];
		if (strncmp(env_var, var_name, strlen(var_name)) == 0
			&& env_var[strlen(var_name)] == '=')
		{
			free(my_environment[i]);
			while (my_environment[i + 1] != NULL)
			{
				my_environment[i] = my_environment[i + 1];
				i++;
			}
			my_environment[i] = NULL;
			return ;
		}
		i++;
	}
}

void	builtin_unset(char *var_name)
{
	if (var_name != NULL)
		remove_env_variable(var_name);
	else
		printf("unset: missing argument\n");
}
