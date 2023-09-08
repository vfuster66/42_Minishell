/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:16:46 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:15:47 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction ft_lstlast renvoie le dernier élément 
** de la liste chaînée 'lst'.
*/
t_list	*ft_lstlast(t_list *lst)
{
	// Si 'lst' est NULL, on renvoie NULL.
	if (!lst)
		return (NULL);

	// On parcourt la liste chaînée jusqu'au dernier élément.
	while (lst->next)
		lst = lst->next;

	// On renvoie le dernier élément de la liste chaînée.
	return (lst);
}

/*#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

int main()
{
	// On crée une liste chaînée de 4 éléments.
	t_list *lst = ft_lstnew("Element 1");
	ft_lstadd_back(&lst, ft_lstnew("Element 2"));
	ft_lstadd_back(&lst, ft_lstnew("Element 3"));
	ft_lstadd_back(&lst, ft_lstnew("Element 4"));

	// On affiche le contenu du dernier élément de la liste chaînée.
	t_list *last = ft_lstlast(lst);
	if (last != NULL)
		printf("Last element : %s\n", (char *)last->content);

	// On libère la mémoire allouée pour la liste chaînée.
	ft_lstclear(&lst, free);

	return (0);
}*/
