/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 11:22:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:20:06 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction ft_lstmap applique la fonction 'f' à chaque élément 
** de la liste chaînée 'lst' pour créer une nouvelle liste chaînée 
** à partir des résultats. Si l'allocation de mémoire échoue à tout
** moment, la fonction libère toutes les mémoires allouées précédemment
** et retourne NULL.
*/
t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	// La nouvelle liste chaînée créée à partir des résultats.
	t_list	*out;
	// Le pointeur vers l'élément courant de la nouvelle liste chaînée.
	t_list	*elem;  

	// Si 'lst', 'f' ou 'del' est NULL, la fonction retourne NULL.
	if (!lst || !f || !del)
		return (NULL);

	// On alloue de la mémoire pour le premier élément de la nouvelle liste chaînée.
	out = ft_lstnew(NULL);
	if (!out)
		return (NULL);

	// On initialise 'elem' à 'out', le premier élément de la nouvelle liste chaînée.
	elem = out;

	// On parcourt tous les éléments de la liste chaînée 'lst'.
	while (lst)
	{
		// On applique la fonction 'f' à l'élément courant de la liste chaînée 'lst'
		// et on stocke le résultat dans l'élément courant de la nouvelle liste chaînée.
		elem->content = (*f)(lst->content);

		// Si l'élément courant de la liste chaînée 'lst' a un suivant,
		// on alloue de la mémoire pour le prochain élément de la nouvelle liste chaînée.
		if (lst->next)
		{
			elem->next = ft_lstnew(NULL);
			if (!elem->next)
			{
				// Si l'allocation de mémoire échoue, on libère toutes les mémoires allouées
				// précédemment pour la nouvelle liste chaînée et on retourne NULL.
				ft_lstclear(&out, del);
				return (NULL);
			}
			// On avance 'elem' au prochain élément de la nouvelle liste chaînée.
			elem = elem->next;
		}
		// On avance 'lst' à l'élément suivant de la liste chaînée 'lst'.
		lst = lst->next;
	}
	// On retourne la nouvelle liste chaînée créée.
	return (out);
}

/*#include "libft.h"
#include <stdio.h>

void *double_content(void *content)
{
	int *ptr = (int*)content;
	int *new_ptr = malloc(sizeof(int));
	if (!new_ptr)
		return (NULL);
	*new_ptr = *ptr * 2;
	return (new_ptr);
}

void free_content(void *content)
{
	free(content);
}

int main()
{
	t_list *lst = ft_lstnew(malloc(sizeof(int)));
	*((int*)lst->content) = 1;
	ft_lstadd_back(&lst, ft_lstnew(malloc(sizeof(int))));
	*((int*)lst->next->content) = 2;
	ft_lstadd_back(&lst, ft_lstnew(malloc(sizeof(int))));
	*((int*)lst->next->next->content) = 3;

	t_list *new_lst = ft_lstmap(lst, double_content, free_content);
	if (!new_lst)
	{
		printf("Error: memory allocation failed\n");
		return (1);
	}

	printf("Original list:\n");
	while (lst)
	{
		printf("%d ", *((int*)lst->content));
		lst = lst->next;
	}
	printf("\n");

	printf("New list:\n");
	while (new_lst)
	{
		printf("%d ", *((int*)new_lst->content));
		new_lst = new_lst->next;
	}
	printf("\n");

	ft_lstclear(&lst, free_content);
	ft_lstclear(&new_lst, free_content);

	return (0);
}*/
