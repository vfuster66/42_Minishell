/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:17:15 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:22:01 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction ft_lstnew alloue de la mémoire pour un nouvel élément 
** de liste chaînée et initialise ses champs 'content' et 'next'.
*/
t_list	*ft_lstnew(void *content)
{
	// Le pointeur vers le nouvel élément de liste chaînée.
	t_list	*list;  
	// On alloue de la mémoire pour le nouvel élément de liste chaînée.
	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);

	// On initialise le champ 'content' de l'élément avec le contenu passé en paramètre.
	list->content = content;

	// On initialise le champ 'next' de l'élément à NULL, car c'est le dernier élément de la liste.
	list->next = NULL;

	// On retourne le pointeur vers le nouvel élément de liste chaînée.
	return (list);
}

/*#include "libft.h"
#include <stdio.h>

int main(void)
{
    t_list *list;

    // On crée un élément de la liste chaînée avec un entier comme contenu.
    list = ft_lstnew((void *)42);
    if (!list)
    {
        printf("Error: Memory allocation failed\n");
        return (1);
    }

    // On affiche le contenu de l'élément de la liste chaînée.
    printf("Content: %d\n", (int)list->content);

    // On affiche le pointeur vers l'élément suivant, qui doit être NULL.
    printf("Next: %p\n", (void *)list->next);

    // On libère la mémoire allouée pour l'élément de la liste chaînée.
    free(list);

    return (0);
}*/
