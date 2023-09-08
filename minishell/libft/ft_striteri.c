/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:19:59 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 15:20:33 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Applique la fonction `f` à chaque caractère de la chaîne de caractères `s`.
** La fonction `f` prend deux arguments : l'indice du caractère dans la chaîne,
** et un pointeur vers le caractère en question.
** L'indice de chaque caractère est passé à la fonction `f` en utilisant une variable
** `i2` initialisée à zéro et incrémentée à chaque itération de la boucle.
** La fonction `f` est appelée avec l'indice et un pointeur vers le caractère,
** puis le pointeur vers la chaîne de caractères est incrémenté pour passer au caractère suivant.
** La fonction s'arrête lorsque le pointeur atteint la fin de la chaîne de caractères.
** Si `s` ou `f` sont NULL, la fonction ne fait rien.
*/

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;
	unsigned int	i2;

	i = 0;
	i2 = 0;
	if (s != NULL || f != NULL)
	{
		// Récupère la longueur de la chaîne de caractères
		i = ft_strlen(s);
		while (i2 < i)
		{
			// Appelle la fonction `f` avec l'indice et un pointeur vers le caractère
			(*f)(i2, s);
			// Incrémente le pointeur vers la chaîne de caractères pour passer au caractère suivant
			s++;
			// Incrémente l'indice pour le prochain caractère
			i2++;
		}
	}
}

/*#include <stdio.h>

void	print_char(unsigned int i, char *c)
{
	printf("Caractere %d : %c\n", i, *c);
}

int		main(void)
{
	char	str[] = "Hello, world!";

	printf("Avant : %s\n", str);
	ft_striteri(str, &print_char);
	printf("Apres : %s\n", str);

	return (0);
}*/

