/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 15:31:52 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:47:27 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction bzero remplit un bloc de mémoire avec des zéros.
** Cette implémentation utilise une boucle while pour parcourir 
** le bloc de mémoire.
*/

void	ft_bzero(void *s, size_t n)
{
	// Pointeur vers le début du bloc de mémoire
	unsigned char	*dest;
	// Variable de boucle
	size_t			i;     

	dest = s;
	i = 0;

	// Remplir chaque octet du bloc de mémoire avec zéro
	while (i++ < n)
		*dest++ = 0;
}

/*#include <stdio.h>
#include <string.h>
#include "libft.h"

void	ft_bzero(void *s, size_t n);

int main(void)
{
    char str1[20] = "Hello, world!";
    char str2[20] = "Hello, world!";

    // Utilisation de la fonction bzero de la bibliothèque standard
    printf("Avant bzero de la bibliothèque standard : %s\n", str1);
    bzero(str1, 5);
    printf("Après bzero de la bibliothèque standard : %s\n", str1);

    // Utilisation de la fonction ft_bzero de notre libft
    printf("Avant ft_bzero : %s\n", str2);
    ft_bzero(str2, 5);
    printf("Après ft_bzero : %s\n", str2);

    return (0);
}*/
