/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:14:53 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:06:44 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction ft_lstadd_front ajoute l'élément 'new' 
** au début de la liste chaînée '*lst'.
*/
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	// Vérification des pointeurs
	if (lst && new) 
	{
		// Le 'next' de 'new' pointe vers 
		// l'élément suivant de la liste chaînée.
		new->next = *lst;
		// Le premier élément de la liste chaînée 
		// pointe maintenant vers 'new'. 
		*lst = new; 
	}
}

/*#include <stdio.h>
#include <stdlib.h>

void	ft_lstadd_front(t_list **lst, t_list *new);

int	main(void)
{
	t_list	*lst = NULL;
	t_list	*new_elem;

	// Création d'un nouvel élément et initialisation de ses données
	new_elem = (t_list *)malloc(sizeof(t_list));
	if (!new_elem)
		return (1);
	new_elem->data = 42;
	new_elem->next = NULL;

	// Ajout de l'élément au début de la liste chaînée
	ft_lstadd_front(&lst, new_elem);

	// Affichage de la liste chaînée pour vérifier que l'élément a été ajouté correctement
	while (lst)
	{
		printf("%d\n", lst->data);
		lst = lst->next;
	}

	return (0);
}*/
