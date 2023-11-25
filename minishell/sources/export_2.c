/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:24:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 10:21:44 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*create_new_variable(const char *var_name, const char *new_var_value)
{
	int		var_name_len;
	int		new_var_len;
	char	*new_var;

	var_name_len = ft_strlen(var_name);
	new_var_len = var_name_len + ft_strlen(new_var_value) + 1;
	new_var = (char *)malloc(new_var_len + 1);
	if (new_var != NULL)
	{
		ft_strcpy(new_var, var_name);
		ft_strcat(new_var, "=");
		ft_strcat(new_var, new_var_value);
	}
	return (new_var);
}

void	add_new_variable(const char *var_name,
		const char *new_var_value)
{
	int		i;
	char	*new_var;

	i = 0;
	while (g_environment[i] != NULL)
		i++;
	new_var = create_new_variable(var_name, new_var_value);
	if (new_var == NULL)
	{
		perror("export");
		free((char *)new_var_value);
		return ;
	}
	g_environment = (char **)realloc(g_environment, (i + 2) * sizeof(char *));
	if (g_environment == NULL)
	{
		perror("export");
		free((char *)new_var_value);
		return ;
	}
	g_environment[i] = new_var;
	g_environment[i + 1] = NULL;
}

void	update_or_add_environment_variable(const char *var_name,
		const char *new_var_value)
{
	int		i;
	int		var_name_len;
	char	*env_var;

	i = 0;
	while (g_environment[i] != NULL)
	{
		env_var = g_environment[i];
		var_name_len = ft_strlen(var_name);
		if (is_matching_variable(env_var, var_name, var_name_len))
		{
			if (new_var_value != NULL)
				update_existing_variable(i, var_name, new_var_value);
			else
				remove_variable(i);
			return ;
		}
		i++;
	}
	if (new_var_value != NULL)
		add_new_variable(var_name, new_var_value);
}

char	*extract_env_value(const char *var_value)
{
	int		var_value_len;
	char	*new_var_value;

	if (var_value == NULL)
		return (NULL);
	var_value_len = ft_strlen(var_value);
	if (var_value_len >= 2 && var_value[0] == '"'
		&& var_value[var_value_len - 1] == '"')
	{
		new_var_value = (char *)malloc(var_value_len - 1);
		if (new_var_value == NULL)
		{
			perror("export");
			return (NULL);
		}
		ft_strncpy(new_var_value, var_value + 1, var_value_len - 2);
		new_var_value[var_value_len - 2] = '\0';
		return (new_var_value);
	}
	else
		return (ft_strdup(var_value));
}

void	builtin_export(const char *var_name, const char *var_value)
{
	char	*new_var_value;

	if (!is_valid_identifier(var_name))
		printf("bash: export: `%s': not a valid identifier\n", var_name);
	if (!is_valid_identifier(var_name))
		return ;
	if (var_name == NULL || (var_name != NULL && ft_strlen(var_name) == 0))
	{
		if (var_value == NULL)
			printf("bash: export: `=': not a valid identifier\n");
		else
			print_all_environment();
		return ;
	}
	if (var_value != NULL)
	{
		new_var_value = extract_env_value(var_value);
		if (new_var_value != NULL)
		{
			update_or_add_environment_variable(var_name, new_var_value);
			free(new_var_value);
		}
	}
	else
		update_or_add_environment_variable(var_name, NULL);
}
