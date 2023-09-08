/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:13:51 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:51:20 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction isprint vérifie si le caractère c est un caractère imprimable.
** Cette implémentation vérifie si c est compris entre les codes 
** ASCII des caractères imprimables.
*/

int	ft_isprint(int c)
{
	// Vérifier si c est compris entre les codes ASCII 
	//des caractères imprimables (32 à 126 inclus)
	return (c >= 32 && c <= 126);
}

/*#include <stdio.h>
#include <ctype.h>

int ft_isprint(int c);

int main(void)
{
    int c = 'A';
    printf("Le caractère %c est imprimable : %d\n", c, ft_isprint(c));
    
    c = ' ';
    printf("Le caractère espace est imprimable : %d\n", ft_isprint(c));
    
    c = '\n';
    printf("Le caractère de saut de ligne est imprimable : %d\n", ft_isprint(c));
    
    c = '\t';
    printf("Le caractère de tabulation est imprimable : %d\n", ft_isprint(c));
    
    c = 127;
    printf("Le caractère ASCII %d est imprimable : %d\n", c, ft_isprint(c));
    
    return (0);
}*/
