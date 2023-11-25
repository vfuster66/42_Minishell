/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:41:23 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 17:24:47 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_environment_variable(const char *env_var, const char *var_name)
{
	return (ft_strncmp(env_var, var_name, ft_strlen(var_name)) == 0
		&& env_var[ft_strlen(var_name)] == '=');
}

void	substitute_variable_value(char **str, const char *var_name,
		const char *var_value, const char *end)
{
	size_t	var_value_length;
	size_t	prefix_length;
	size_t	suffix_length;
	size_t	new_length;
	char	*new_str;

	var_value_length = ft_strlen(var_value);
	prefix_length = var_name - *str;
	suffix_length = ft_strlen(end);
	new_length = prefix_length + var_value_length + suffix_length;
	new_str = (char *)malloc(new_length + 1);
	if (new_str == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	ft_memcpy(new_str, *str, prefix_length);
	ft_memcpy(new_str + prefix_length, var_value, var_value_length);
	ft_memcpy(new_str + prefix_length + var_value_length, end, suffix_length);
	new_str[new_length] = '\0';
	free(*str);
	*str = new_str;
}

char	*find_environment_variable(const char *var_name, char *g_environment[])
{
	int	i;

	i = 0;
	while (g_environment[i] != NULL)
	{
		if (is_environment_variable(g_environment[i], var_name))
			return (g_environment[i] + ft_strlen(var_name) + 1);
		i++;
	}
	return (NULL);
}

const char	*find_variable_end(const char *start)
{
	const char	*end;

	end = start + 1;
	while (*end != '\0' && (*end == '_' || ft_isalnum(*end)))
		end++;
	return (end);
}

char	*extract_variable_name(const char *start, const char *end)
{
	size_t		var_name_length;
	char		*var_name;

	var_name_length = end - start - 1;
	var_name = (char *)malloc(var_name_length + 1);
	if (var_name == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	ft_strncpy(var_name, start + 1, var_name_length);
	var_name[var_name_length] = '\0';
	return (var_name);
}
