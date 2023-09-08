/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:14:28 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 08:00:41 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// fonction qui convertit un entier en chaine de caracteres
 
// determine la longueur de la chaine de caracteres en calculant
//le nombre de chiffres necessaires pour representer l'entier
static int	get_size(int n)
{
	int	size;

	size = 0;
	// si n est négatif ou zéro
	if (n <= 0)
		// on incrémente la taille pour le signe '-'
		size++;
	// tant que n est différent de zéro 
	while (n != 0) 
	{
		// on divise n par 10
		n = n / 10;
		// on incrémente la taille
		size++; 
	}
	return (size);
}
// remplit la chaine de caracters avec les chiffres de l'entier
// en utilisant le modulo et la division pour extraire les chiffre
// un par un de gauche a droite 
static void	fill_res(int size, int offset, int n, char *res)
{
	while (size > offset)
	{
		// on calcule le chiffre correspondant 
		// à la position et on l'ajoute à la chaîne résultante
		res[size - 1] = n % 10 + '0';
		// on divise n par 10 pour passer au chiffre suivant
		n = n / 10; 
		// on décrémente la taille
		size --; 
	}
}
// alloue de la memoire pour la chaine de caracteres resultante
// gere les cas speciaux (entier negatif) et remplit la chaine de
// caracteres avec fill_res.
char	*ft_itoa(int n)
{
	int		offset;
	int		size;
	char	*res;

	offset = 0;
	// on calcule la taille de la chaîne résultante
	size = get_size(n);
	// on alloue de la mémoire pour la chaîne résultante 
	res = (char *)malloc(sizeof(char) * size + 1); 
	if (!res)
		return (0);
	// si n est la valeur minimale de type int
	if (n == -2147483648)
	{
		res[0] = '-';
		res[1] = '2';
		n = 147483648;
		// on ajoute deux caractères au début pour 
		// le signe '-' et le chiffre '2'
		offset = 2; 
	}
	// si n est négatif
	if (n < 0) 
	{
		res[0] = '-';
		// on ajoute un caractère au début pour le signe '-'
		offset = 1;
		// on travaille avec la valeur absolue de n
		n = -n; 
	}
	// on remplit la chaîne résultante avec les chiffres de n
	fill_res(size, offset, n, res);
	// on ajoute le caractère de fin de chaîne
	res[size] = '\0'; 
	return (res);
}

/*#include <stdio.h>
#include <stdlib.h>

char	*ft_itoa(int n);

int	main(void)
{
	int	n = -12345;
	char *str = ft_itoa(n);

	if (str)
	{
		printf("Integer: %d\n", n);
		printf("String: %s\n", str);
		free(str);
	}
	else
	{
		printf("Memory allocation failed\n");
		return (1);
	}
	return (0);
}*/
