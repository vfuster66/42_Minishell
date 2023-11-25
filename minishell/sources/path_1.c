/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 09:11:44 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 10:41:54 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_custom_env_variable(const char *variable_name,
		char **g_environment)
{
	int			i;
	size_t		name_length;

	i = 0;
	name_length = ft_strlen(variable_name);
	while (g_environment[i] != NULL)
	{
		if (ft_strncmp(g_environment[i], variable_name, name_length) == 0
			&& g_environment[i][name_length] == '=')
			return (g_environment[i] + name_length + 1);
		i++;
	}
	return (NULL);
}

char	*search_in_current_directory(char *command)
{
	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	return (NULL);
}

char	*search_in_path_directories(char *command, char *path)
{
	char	full_path[1024];

	ft_strcpy(full_path, path);
	if (full_path[ft_strlen(full_path) - 1] != '/')
		ft_strcat(full_path, "/");
	ft_strcat(full_path, command);
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path));
	return (NULL);
}
