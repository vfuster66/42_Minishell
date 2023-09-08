/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:54:25 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 14:07:22 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// La fonction ft_memset remplit les n premiers octets de la zone de 
// mémoire pointée par s avec l'octet c.
void	*ft_memset(void *s, int c, size_t n)
{
	// Pointeur vers la zone de destination
	unsigned char	*new_dest;
	// Caractère à copier dans la zone de destination
	unsigned char	src;
	// Compteur pour le nombre d'octets à copier
	size_t			i;          

	i = 0;
	// Initialisation du pointeur vers la zone de destination
	new_dest = s;
	// Initialisation du caractère à copier dans la zone de destination
	src = c;
	// Boucle pour copier n octets depuis la zone source vers la zone destination
	while (i++ < n)
		// Copie du caractère src dans la zone de destination
		*new_dest++ = src;
	// On retourne un pointeur vers le début de la zone de destination	    
	return (s);                 
}

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	*ft_memset(void *s, int c, size_t n);

int	main(void)
{
	char	str[20];

	// Initialiser le tableau str avec des 'A'
	ft_memset(str, 'A', sizeof(str));
	printf("str = %s\n", str);

	return (0);
}*/
