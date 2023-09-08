/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 08:48:40 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 15:14:46 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Cette fonction recherche la première occurrence du caractère 'c' 
// dans la chaîne de caractères 's' et renvoie un pointeur sur cet emplacement.

char	*ft_strchr(char const *s, int c)
{
	// le caractère à rechercher
	char	find;
	// un compteur pour l'itération sur les caractères de la chaîne 's'
	int		i; 

	// cast le caractère à rechercher en unsigned char pour éviter 
	// les problèmes de signe
	find = (unsigned char)c; 
	i = 0;
	// boucle sur chaque caractère de la chaîne 's' jusqu'à la fin 
	// de la chaîne
	while (s[i])
	{
		// vérifie si le caractère à la position actuelle est 
		// égal au caractère recherché
		if (s[i] == find)
			// renvoie un pointeur sur l'emplacement de la 
			// première occurrence du caractère recherché
			return ((char *)s + i);
		// incrémente le compteur pour passer au caractère 
		// suivant de la chaîne 's' 
		i++; 
	}
	// vérifie si le dernier caractère de la chaîne est égal 
	// au caractère recherché
	if (s[i] == find)
		// renvoie un pointeur sur l'emplacement du dernier caractère
		return ((char *)s + i);
	// retourne NULL si le caractère recherché n'est pas présent dans la chaîne 's'	 
	return (0);
}

/*#include <stdio.h>
#include <stdlib.h>
#include "libft.h" 
int	main(void)
{
	char	*str = "Hello World!";
	char	c = 'o';
	char	*ptr;

	// appelle la fonction ft_strchr avec la chaîne 
	// 'str' et le caractère 'c'
	ptr = ft_strchr(str, c);
	// vérifie si la fonction a renvoyé un pointeur valide 
	if (ptr) 
		// affiche l'emplacement du caractère 'c' dans la chaîne 'str'
		printf("Le caractère '%c' a été trouvé à l'emplacement %ld de la chaîne.\n", c, ptr - str); 
	else
		// affiche un message si le caractère 'c' n'a pas été trouvé
		printf("Le caractère '%c' n'a pas été trouvé dans la chaîne.\n", c); 
	return (0);
}*/


