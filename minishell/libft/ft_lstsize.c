/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:12:30 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 13:45:57 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Cette fonction calcule la taille d'une liste chaînée en parcourant 
// tous ses éléments
int	ft_lstsize(t_list *lst)
{
	// On initialise la variable "size" à 0
	int	size = 0;
	
	// On parcourt la liste tant qu'on n'a pas atteint 
	// la fin (lst == NULL)
	while (lst)
	{
		// Pour chaque élément de la liste, 
		// on incrémente la variable "size"
		size++;
		// Et on passe à l'élément suivant
		lst = lst->next;
	}
	
	// On retourne la taille calculée
	return (size);
}

#include <stdio.h>
#include <stdlib.h>

typedef struct	s_list
{
	int				data;
	struct s_list	*next;
}				t_list;

int	ft_lstsize(t_list *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return (size);
}

/*int	main(void)
{
	// On crée une liste chaînée avec trois éléments
	t_list *lst = malloc(sizeof(t_list));
	lst->data = 42;
	lst->next = malloc(sizeof(t_list));
	lst->next->data = 43;
	lst->next->next = malloc(sizeof(t_list));
	lst->next->next->data = 44;
	lst->next->next->next = NULL;

	// On affiche la taille de la liste
	printf("Taille de la liste : %d\n", ft_lstsize(lst));

	// On libère la mémoire allouée pour la liste
	free(lst->next->next);
	free(lst->next);
	free(lst);

	return (0);
}*/
