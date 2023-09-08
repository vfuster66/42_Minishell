/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 07:57:11 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 16:19:38 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Copie jusqu'à size octets de src dans dst, en ajoutant un '\0' final.
** La copie commence à la fin de la chaîne dst (après le '\0').
** La fonction renvoie la longueur de la chaîne qu'elle a essayé de créer.
** La longueur totale de la chaîne est la plus petite entre (size - 1) et
** la somme des longueurs de src et dst.
**
** Si size est nul, la fonction renvoie la longueur de src.
** Si src est NULL, la fonction renvoie la longueur de dst + la longueur de src.
**
** La fonction renvoie la longueur totale de la chaîne qu'elle a essayé de créer.
*/

size_t	ft_strlcat(char *dst, char const *src, size_t size)
{
	// Indice de parcours de dst
	size_t	i;
	// Indice de parcours de src
	size_t	j; 

	// Si src est NULL ou size est nul, on renvoie la longueur de src
	if (!src || !size) 
		return (ft_strlen(src));

	i = 0;
	// On cherche la fin de dst (après le '\0') ou la fin de la zone allouée
	while (dst[i] && i < size) 
		i++;
	j = i;
	// On copie src à partir de dst jusqu'à size - 1 caractères
	while (src[i - j] && i < size - 1) 
	{
		dst[i] = src[i - j];
		i++;
	}
	// Si la taille allouée pour dst était suffisante pour contenir la 
	// chaîne complète, on ajoute le '\0' final
	if (j < size) 
		dst[i] = '\0';
	// On renvoie la longueur totale de la chaîne qu'on a essayé de créer
	return (j + ft_strlen(src));
} 

/*#include <stdio.h>
#include <string.h>
#include "libft.h"

int main(void)
{
    char dst[15] = "Hello";
    char src[10] = " world!";
    size_t n = sizeof(dst);

    printf("Before ft_strlcat dst = %s, src = %s\n", dst, src);
    size_t ret = ft_strlcat(dst, src, n);
    printf("After ft_strlcat dst = %s, src = %s\n", dst, src);
    printf("ft_strlcat return value: %zu\n", ret);

    return (0);
}*/

