/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:24:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 07:45:28 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_all_environment(void)
{
	int		i;
	char	*env_var;
	char	*equals_sign;
	char	*var_name;
	char	*var_value;

	i = 0;
	while (g_environment[i] != NULL)
	{
		env_var = g_environment[i];
		equals_sign = ft_strchr(env_var, '=');
		if (equals_sign != NULL)
		{
			*equals_sign = '\0';
			var_name = env_var;
			var_value = equals_sign + 1;
			printf("declare -x %s=\"%s\"\n", var_name, var_value);
			*equals_sign = '=';
		}
		i++;
	}
}

int	is_matching_variable(const char *env_var, const char *var_name,
		int var_name_len)
{
	return (ft_strncmp(env_var, var_name, var_name_len) == 0
		&& env_var[var_name_len] == '=');
}

void	update_existing_variable(int i, const char *var_name,
		const char *new_var_value)
{
	int		var_name_len;
	int		new_var_len;
	char	*new_var;

	var_name_len = ft_strlen(var_name);
	new_var_len = var_name_len + ft_strlen(new_var_value) + 1;
	new_var = (char *)malloc(new_var_len + 1);
	if (new_var == NULL)
	{
		perror("export");
		free((char *)new_var_value);
		return ;
	}
	ft_strcpy(new_var, var_name);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, new_var_value);
	free(g_environment[i]);
	g_environment[i] = new_var;
}

void	remove_variable(int i)
{
	int	j;

	free(g_environment[i]);
	j = i;
	while (g_environment[j] != NULL)
	{
		g_environment[j] = g_environment[j + 1];
		j++;
	}
}

int	is_valid_identifier(const char *var_name)
{
	int	i;

	i = 0;
	while (var_name[i] != '\0')
	{
		if (!(var_name[i] == '$' || ft_isalnum(var_name[i])))
			return (0);
		++i;
	}
	return (1);
}
