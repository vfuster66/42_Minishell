/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:51:59 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:45:09 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

/*
** La fonction isalnum vérifie si le caractère c est un caractère alphanumérique.
** Cette implémentation appelle les fonctions ft_isalpha et ft_isdigit pour vérifier si c est une lettre ou un chiffre.
*/

int	ft_isalnum(int c)
{
	// Vérifier si c est une lettre alphabétique
	if (ft_isalpha(c))
		return (1);
	// Vérifier si c est un chiffre
	if (ft_isdigit(c))
		return (1);
	// Si c n'est ni une lettre ni un chiffre, retourner 0
	return (0);
}

/*int main()
{
    char c;

    // Test de caractères alphanumériques
    c = 'a';
    printf("ft_isalnum('%c') = %d\n", c, ft_isalnum(c));
    c = '5';
    printf("ft_isalnum('%c') = %d\n", c, ft_isalnum(c));
    c = 'Z';
    printf("ft_isalnum('%c') = %d\n", c, ft_isalnum(c));
    c = '@';
    printf("ft_isalnum('%c') = %d\n", c, ft_isalnum(c));

    // Test de caractères non alphanumériques
    c = '$';
    printf("ft_isalnum('%c') = %d\n", c, ft_isalnum(c));
    c = '\n';
    printf("ft_isalnum('\\n') = %d\n", ft_isalnum(c));

    return (0);
}*/
