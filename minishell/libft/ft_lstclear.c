/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:15:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:09:41 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction ft_lstclear supprime tous les éléments de 
** la liste chaînée '*lst'.
** Pour chaque élément de la liste, la fonction 'del' est 
** appelée pour supprimer le contenu de l'élément.
** Ensuite, l'élément est supprimé avec la fonction 'free'.
*/
void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;
	t_list	*start;

	if (!lst || !del)
		return ;

	start = *lst;
	while (start)
	{
		// On stocke le prochain élément de la liste 
		// pour ne pas le perdre lors de la suppression.
		tmp = start->next; 
		// On appelle la fonction 'del' pour supprimer 
		// le contenu de l'élément.
		del(start->content);
		// On supprime l'élément avec la fonction 'free'. 
		free(start);
		// On passe à l'élément suivant.
		start = tmp; 
	}
	// On met à jour le pointeur de la liste chaînée avec NULL 
	// pour signifier que la liste est vide.
	*lst = NULL; 
}
/*#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*));

void	my_free(void *ptr)
{
	free(ptr);
}

int		main(void)
{
	t_list	*lst;

	lst = ft_lstnew((void *)ft_strdup("hello"));
	ft_lstadd_back(&lst, ft_lstnew((void *)ft_strdup("world")));
	ft_lstadd_front(&lst, ft_lstnew((void *)ft_strdup("foo")));
	ft_lstadd_front(&lst, ft_lstnew((void *)ft_strdup("bar")));

	printf("Liste initiale :\n");
	ft_lstiter(lst, &ft_putstr_fd);

	ft_lstclear(&lst, &my_free);

	printf("Liste après ft_lstclear :\n");
	ft_lstiter(lst, &ft_putstr_fd);

	return (0);
}*/
