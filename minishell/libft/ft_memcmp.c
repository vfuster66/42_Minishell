/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 11:19:52 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 13:51:41 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Cette fonction compare les "n" premiers octets des zones mémoire 
// "s1" et "s2"
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	// Pointeur vers la zone mémoire "s1" en tant que tableau 
	// d'octets non signés
	unsigned char	*sone;
	// Pointeur vers la zone mémoire "s2" en tant que tableau 
	// d'octets non signés
	unsigned char	*stwo; 

	// Si les deux zones mémoire sont identiques (ou si "n" est nul),
	// on retourne 0
	if (s1 == s2 || n == 0)
		return (0);

	// On cast les pointeurs en pointeurs vers des tableaux d'octets
	// non signés
	sone = (unsigned char *)s1;
	stwo = (unsigned char *)s2;
	
	// On parcourt les "n" premiers octets des deux zones mémoire
	while (n--)
	{
		// Si l'octet "s1[i]" est différent de l'octet "s2[i]", 
		//on retourne leur différence
		if (*sone != *stwo)
			return (*sone - *stwo);

		// Sinon, on passe à l'octet suivant
		if (n)
		{
			sone++;
			stwo++;
		}
	}

	// Si les "n" premiers octets des deux zones mémoire sont 
	//identiques, on retourne 0
	return (0);
}

/*#include <stdio.h>
#include <string.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n);

int	main(void)
{
	char	str1[] = "Hello, world!"; // Première chaîne de caractères
	char	str2[] = "Hello, world!"; // Deuxième chaîne de caractères identique
	char	str3[] = "Bonjour, monde!"; // Deuxième chaîne de caractères différente
	int		result; // Résultat de la comparaison

	// Comparaison des deux chaînes identiques
	result = ft_memcmp(str1, str2, strlen(str1));
	printf("Comparaison de str1 et str2 : %d\n", result);

	// Comparaison des deux chaînes différentes
	result = ft_memcmp(str1, str3, strlen(str1));
	printf("Comparaison de str1 et str3 : %d\n", result);

	return (0);
}*/
