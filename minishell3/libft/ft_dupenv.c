/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dupenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 12:56:43 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/03 13:23:51 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Fonction pour allouer et initialiser une copie de l'environnement
char	**initialize_environment_copy(char **envp)
{
	int		env_size;
	char	**env_copy;

	env_size = 0;
	env_copy = NULL;
	while (envp[env_size] != NULL)
	{
		env_size++;
	}
	env_copy = (char **)malloc((env_size + 1) * sizeof(char *));
	if (env_copy == NULL)
	{
		exit(1);
	}
	return (env_copy);
}

// Fonction pour copier une variable d'environnement
char	*copy_environment_variable(const char *env_var)
{
	char	*copy;

	copy = ft_strdup(env_var);
	if (copy == NULL)
	{
		exit(1);
	}
	return (copy);
}

// Fonction pour dupliquer l'environnement actuel
char	**ft_dupenv(char **envp)
{
	char	**env_copy;
	int		i;

	i = 0;
	env_copy = initialize_environment_copy(envp);
	while (envp[i] != NULL)
	{
		env_copy[i] = copy_environment_variable(envp[i]);
		i++;
	}
	return (env_copy);
}
