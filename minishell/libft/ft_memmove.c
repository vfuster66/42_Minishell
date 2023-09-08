/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:54:14 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 14:01:19 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// copie une zone de memoire de taille 'n' depuis src vers dest
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	// Pointeur vers la zone de destination
	unsigned char	*new_dest;
	// Pointeur vers la zone source 
	unsigned char	*new_src; 

	// Si la zone source et la zone de destination sont les mêmes, 
	// ou si la taille est de 0, on ne fait rien
	if (dest == src || n == 0) 
		return (dest);
	// Si la zone de destination est après la zone source dans la mémoire, 
	// on copie simplement de gauche à droite	
	if (dest < src) 
	{
		new_dest = (unsigned char *)dest;
		new_src = (unsigned char *)src;
		while (n--)
			*new_dest++ = *new_src++;
	}
	// Si la zone de destination est avant la zone source dans la mémoire, 
	// on copie de droite à gauche pour éviter les conflits de mémoire
	else 
	{
		new_dest = (unsigned char *)dest + (n - 1);
		new_src = (unsigned char *)src + (n - 1);
		while (n--)
			*new_dest-- = *new_src--;
	}
	// On retourne un pointeur vers le début de la zone de destination
	return (dest); 
}

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	*ft_memmove(void *dest, const void *src, size_t n);

int	main(void)
{
	char	src[] = "Hello World!";
	char	dest[sizeof(src)] = {0};
	
	printf("Avant : src = %s, dest = %s\n", src, dest);
	ft_memmove(dest, src, sizeof(src));
	printf("Après : src = %s, dest = %s\n", src, dest);
	
	return (0);
}*/
