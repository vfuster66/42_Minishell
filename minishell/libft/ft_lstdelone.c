/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:15:54 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:12:07 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction ft_lstdelone supprime l'élément 'lst' de la liste 
** chaînée et libère la mémoire allouée pour cet élément. Le pointeur
** 'lst' est mis à NULL.
*/
void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	// Si 'lst' ou 'del' est NULL, la fonction
	// se termine sans rien faire.
	if (!lst || !del)
		return ;

	// On libère la mémoire de 'lst->content' en utilisant 'del'.
	del(lst->content);

	// On libère la mémoire allouée pour l'élément 'lst'.
	free(lst);

	// On met le pointeur 'lst' à NULL pour éviter les accès 
	// indésirables.
	lst = NULL;
}

/*#include "libft.h"
#include <stdlib.h>

void	ft_lstdelone(t_list *lst, void (*del)(void *));

void	free_int(void *data)
{
	int *ptr;

	ptr = (int *)data;
	free(ptr);
}

int	main(void)
{
	t_list	*lst;
	int		*data;

	// On crée un élément de liste chaînée.
	data = malloc(sizeof(int));
	*data = 42;
	lst = ft_lstnew(data);

	// On affiche la valeur de l'élément avant sa suppression.
	printf("Valeur de l'élément avant suppression : %d\n", *(int *)(lst->content));

	// On supprime l'élément de la liste chaînée et on libère la mémoire allouée pour l'élément.
	ft_lstdelone(lst, free_int);

	// On vérifie que l'élément a bien été supprimé en affichant sa valeur (ce qui devrait causer une erreur de segmentation).
	printf("Valeur de l'élément après suppression : %d\n", *(int *)(lst->content));

	return (0);
}*/
