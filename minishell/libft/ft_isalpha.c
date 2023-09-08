/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:52:20 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:44:22 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

/*
** La fonction isalpha vérifie si le caractère c est une lettre alphabétique.
** Cette implémentation vérifie si c est compris entre les codes ASCII des lettres majuscules ou minuscules.
*/

int	ft_isalpha(int c)
{
	// Vérifier si c est une lettre majuscule 
	//(codes ASCII entre 65 et 90 inclus)
	if (c >= 'A' && c <= 'Z')
		return (1);
	// Vérifier si c est une lettre minuscule 
	//(codes ASCII entre 97 et 122 inclus)
	if (c >= 'a' && c <= 'z')
		return (1);
	// Si c n'est ni une lettre majuscule ni minuscule, retourner 0
	return (0);
}

/*int main(void)
{
    char test_char = 'A';
    int result = ft_isalpha(test_char);

    printf("Test caractère : %c\n", test_char);
    printf("Résultat : %d\n", result);

    return (0);
}*/
