/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:59:21 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 13:48:36 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Cette fonction cherche la première occurrence d'un caractère "c" 
// dans une zone mémoire "s" de taille "n"
void	*ft_memchr(const void *s, int c, size_t n)
{
	// On parcourt la zone mémoire "s" de taille "n" à l'aide 
	// d'une boucle while
	while (n--)
	{
		// Si on trouve le caractère "c" à l'adresse "s", 
		// on retourne un pointeur vers cette adresse
		if (*(unsigned char *)s == (unsigned char)c)
			return ((void *)s);
		// Sinon, on passe à l'octet suivant en incrémentant 
		// le pointeur "s"
		s++;
	}
	
	// Si on n'a pas trouvé le caractère "c" dans la zone mémoire,
	// on retourne un pointeur NULL
	return (0);
}

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	*ft_memchr(const void *s, int c, size_t n);

int	main(void)
{
	// On crée une chaîne de caractères
	char str[] = "Hello World!";
	
	// On cherche la première occurrence de la lettre 'o' dans 
	// la chaîne
	void *result = ft_memchr(str, 'o', strlen(str));
	
	// Si on a trouvé la lettre 'o', on affiche son adresse mémoire
	if (result != NULL)
		printf("La lettre 'o' a été trouvée à l'adresse %p\n", result);
	else
		printf("La lettre 'o' n'a pas été trouvée dans la chaîne\n");

	return (0);
}*/
