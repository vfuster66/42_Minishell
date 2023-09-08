/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:16:21 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:14:30 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction ft_lstiter applique la fonction 'f' à chaque élément 
** de la liste chaînée 'lst'.
** La fonction 'f' prend en paramètre le contenu de chaque élément.
*/
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	// On parcourt tous les éléments de la liste chaînée.
	while (lst != NULL)
	{
		// On applique la fonction 'f' à chaque élément de la liste chaînée.
		f(lst->content);

		// On passe à l'élément suivant de la liste chaînée.
		lst = lst->next;
	}
}

/*#include "libft.h"
#include <stdio.h>

// Fonction à appliquer à chaque élément de la liste chaînée
void	print_content(void *content)
{
	printf("%s\n", (char*)content);
}

int	main(void)
{
	// On crée quelques éléments de la liste chaînée
	t_list *elem1 = ft_lstnew("Premier élément");
	t_list *elem2 = ft_lstnew("Deuxième élément");
	t_list *elem3 = ft_lstnew("Troisième élément");

	// On ajoute les éléments à la liste chaînée
	t_list *list = elem1;
	ft_lstadd_back(&list, elem2);
	ft_lstadd_back(&list, elem3);

	// On applique la fonction print_content à chaque élément de la liste chaînée
	ft_lstiter(list, &print_content);

	// On libère la mémoire allouée pour les éléments de la liste chaînée
	ft_lstclear(&list, &free);

	return (0);
}*/
