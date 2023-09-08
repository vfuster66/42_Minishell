/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:52:36 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:42:46 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

/*
** La fonction isascii vérifie si le caractère c est un caractère ASCII.
** Cette implémentation vérifie si c est compris entre 0 et 127 inclus (valeurs ASCII valides).
*/

int	ft_isascii(int c)
{
	// Vérifier si c est compris entre 0 et 127 inclus 
	//(valeurs ASCII valides)
	if (c >= 0 && c <= 127)
		return (1);
	// Si c n'est pas une valeur ASCII valide, retourner 0
	return (0);
}

/*int main(void)
{
    int test_char = 'a';
    int result = ft_isascii(test_char);

    printf("Test caractère : %c\n", test_char);
    printf("Résultat : %d\n", result);

    return (0);
}*/
