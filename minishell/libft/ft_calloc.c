/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:13:28 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:46:43 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction calloc alloue de la mémoire pour un tableau de count éléments, chacun de taille size.
** Cette implémentation utilise la fonction memset pour initialiser la mémoire allouée à zéro.
*/

void	*ft_calloc(size_t count, size_t size)
{
	// Pointeur vers le début de la mémoire allouée
	void	*pnt; 

	// Vérifier les arguments d'entrée et les ajuster si nécessaire
	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}

	// Vérifier si la multiplication de count et size dépasse la taille maximale d'un size_t
	if (count > (count * size) / size)
		return (NULL);

	// Allouer de la mémoire avec malloc
	pnt = malloc(count * size);
	if (pnt == NULL)
		return (NULL);

	// Initialiser la mémoire allouée à zéro avec memset
	ft_memset((unsigned char *)pnt, 0, count * size);
	return (pnt);
}

/*#include <stdio.h>
#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size);

int	main(void)
{
	int	*ptr;
	size_t	size = 5;

	ptr = (int *)ft_calloc(size, sizeof(int));
	if (ptr == NULL)
	{
		printf("Erreur d'allocation de mémoire.\n");
		return (1);
	}

	for (size_t i = 0; i < size; i++)
	{
		printf("%d ", ptr[i]); // On s'attend à ce que tous les éléments soient initialisés à 0.
	}
	printf("\n");

	free(ptr);
	return (0);
}*/
