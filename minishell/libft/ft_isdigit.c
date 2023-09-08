/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:52:50 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:42:52 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

/*
** La fonction isdigit vérifie si le caractère c est un chiffre.
** Cette implémentation vérifie si c est compris entre les codes ASCII des chiffres '0' et '9'.
*/

int	ft_isdigit(int c)
{
	// Vérifier si c est un chiffre (codes ASCII entre '0' et '9' inclus)
	if (c >= '0' && c <= '9')
		return (1);
	// Si c n'est pas un chiffre, retourner 0
	return (0);
}

/*int main(void)
{
    int test_char = '5';
    int result = ft_isdigit(test_char);

    printf("Test caractère : %c\n", test_char);
    printf("Résultat : %d\n", result);

    return (0);
}*/
