/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:14:44 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:03:57 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction ft_lstadd_back ajoute l'élément 'new' 
** à la fin de la liste chaînée '*lst'.
** Si '*lst' est NULL, alors la liste chaînée est initialisée 
** avec l'élément 'new'.
*/
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*elem;

	// Si la liste chaînée n'est pas NULL, on recherche 
	// le dernier élément de la liste et on ajoute 'new' à la suite.
	if (lst != NULL && *lst != NULL)
	{
		elem = *lst;
		// On récupère le dernier élément de la liste chaînée.
		elem = ft_lstlast(*lst);
		// On ajoute 'new' à la suite du dernier élément. 
		elem->next = new; 
		return ;
	}
	// Si la liste chaînée est NULL, 
	// on initialise la liste avec 'new'.
	*lst = new;
}

/*#include "libft.h"
#include <stdio.h>

void	ft_lstadd_back(t_list **lst, t_list *new);

int main(void)
{
	// Création de la liste chaînée initiale avec deux éléments
	t_list *lst = ft_lstnew((void *)5);
	ft_lstadd_back(&lst, ft_lstnew((void *)10));
	
	// Ajout d'un nouvel élément à la fin de la liste
	ft_lstadd_back(&lst, ft_lstnew((void *)15));
	
	// Affichage de la liste mise à jour
	t_list *curr = lst;
	while (curr != NULL)
	{
		printf("%d\n", (int)curr->content);
		curr = curr->next;
	}
	
	// Libération de la mémoire allouée pour la liste chaînée
	ft_lstclear(&lst, free);
	
	return (0);
}*/
