/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:20:18 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 15:24:24 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Concatène les chaînes `s1` et `s2` pour former une nouvelle chaîne allouée
** dynamiquement. Retourne un pointeur sur cette nouvelle chaîne, ou NULL
** en cas d'échec d'allocation mémoire.
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	// Vérifier si les chaînes passées en paramètre ne sont pas NULL
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	// Allouer de la mémoire pour la nouvelle chaîne
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (str == NULL)
		return (NULL);
	// Copier la première chaîne dans la nouvelle chaîne
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	// Copier la deuxième chaîne à la suite de la première dans la nouvelle chaîne
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	// Ajouter le caractère nul de fin de chaîne à la fin de la nouvelle chaîne
	str[i + j] = '\0';
	// Retourner un pointeur sur la nouvelle chaîne
	return (str);
}

/*#include <stdio.h>
#include <unistd.h>

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		write(1, &s[i++], 1);
}

char	*ft_strjoin(char const *s1, char const *s2);

int	main(void)
{
	char	*s1;
	char	*s2;
	char	*str;

	s1 = "Hello, ";
	s2 = "world!";
	str = ft_strjoin(s1, s2);
	if (str == NULL)
		return (1);
	ft_putstr(str);
	free(str);
	return (0);
}*/
