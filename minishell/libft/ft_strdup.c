/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:19:42 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 15:18:32 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Cette fonction alloue de la mémoire pour une nouvelle chaîne de caractères
// identique à la chaîne 's' passée en paramètre et la renvoie.

char	*ft_strdup(char const *s)
{
	// pointeur vers la nouvelle chaîne de caractères allouée
	char	*str;
	// un compteur pour l'itération sur les caractères de la chaîne 's' 
	size_t	n; 

	n = 0;
	// alloue suffisamment de mémoire pour la nouvelle chaîne de caractères
	str = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	// vérifie si l'allocation de mémoire a échoué
	if (str == NULL)
		// retourne NULL si l'allocation de mémoire a échoué
		return (NULL);
	// copie chaque caractère de la chaîne 's' dans la nouvelle chaîne 
	// de caractères jusqu'à la fin de la chaîne 
	while (s[n]) 
	{
		str[n] = s[n];
		n++;
	// ajoute un caractère nul à la fin de la nouvelle chaîne de caractères
	str[n] = '\0';
	}
	// renvoie un pointeur vers la nouvelle chaîne de caractères allouée

	return (str); 
}

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*ft_strdup(char const *s);

int		main(void)
{
	char	*src = "Hello World";
	char	*dest1;
	char	*dest2;

	dest1 = strdup(src);
	dest2 = ft_strdup(src);
	printf("src: %s\n", src);
	printf("strdup: %s\n", dest1);
	printf("ft_strdup: %s\n", dest2);
	free(dest1);
	free(dest2);
	return (0);
}*/
