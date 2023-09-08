/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:53:16 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 13:56:57 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// copie les n premiers octets de la zone memoire de src
// vers la zone memoire de dest
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	// Pointeur vers le début de la zone de destination
	unsigned char	*new_dest;
	// Pointeur vers le début de la zone source
	unsigned char	*new_src;
	// Compteur pour le nombre d'octets copiés
	size_t			i; 

	// Si les deux pointeurs sont nuls, on retourne un pointeur nul
	if (!dest && !src) 
		return (0);
	// On initialise le pointeur new_dest au début de la zone 
	// de destination
	new_dest = dest;
	// On initialise le pointeur new_src au début de la zone source
	new_src = (unsigned char *)src;
	// On initialise le compteur i à 0 
	i = 0; 
	// On boucle n fois pour copier n octets de la zone source 
	// à la zone destination
	while (i++ < n) 
		// On copie un octet de la zone source à la zone destination 
		// en incrémentant les pointeurs
		*new_dest++ = *new_src++; 
	// On retourne un pointeur vers le début de la zone de destination
	return (dest); 
}

/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void	*ft_memcpy(void *dest, const void *src, size_t n);

int	main(void)
{
	char	src[] = "Hello, world!"; // On définit une chaîne de caractères source
	char	dest[20]; // On définit une zone de destination de taille suffisante pour copier la chaîne source

	printf("Source string: %s\n", src); // On affiche la chaîne source avant la copie

	ft_memcpy(dest, src, strlen(src) + 1); // On copie la chaîne source dans la zone de destination

	printf("Destination string: %s\n", dest); // On affiche la chaîne de destination après la copie

	return (0);
}*/
