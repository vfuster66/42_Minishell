/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 17:36:20 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 10:39:32 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*check_access_and_return(char *path, char *command)
{
	char	*full_path;

	full_path = malloc(strlen(path) + ft_strlen(command) + 2);
	if (full_path == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	ft_strcpy(full_path, path);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, command);
	if (access(full_path, X_OK) == 0)
	{
		return (full_path);
	}
	free(full_path);
	return (NULL);
}

char	*search_in_path_and_return(char *path, char *command)
{
	char	*token;
	char	*full_path;

	token = ft_strtok(path, ":");
	full_path = NULL;
	while (token != NULL)
	{
		full_path = check_access_and_return(token, command);
		if (full_path != NULL)
			return (full_path);
		token = ft_strtok(NULL, ":");
	}
	return (NULL);
}

char	*extract_path_from_environment(char **g_environment)
{
	int	i;

	i = 0;
	while (g_environment[i] != NULL)
	{
		if (ft_strncmp(g_environment[i], "PATH=", 5) == 0)
		{
			return (g_environment[i] + 5);
		}
		i++;
	}
	return (NULL);
}

char	*search_command_path(char *command, char **g_environment)
{
	char	*path_env;
	char	*path;
	char	*result;

	path_env = extract_path_from_environment(g_environment);
	path = NULL;
	if (path_env == NULL)
	{
		printf("Erreur : variable d'environnement non trouvée.\n");
		return (NULL);
	}
	path = ft_strdup(path_env);
	result = search_in_path_and_return(path, command);
	if (result == NULL && access(command, X_OK) == 0)
	{
		free(path);
		return (ft_strdup(command));
	}
	free(path);
	return (result);
}
