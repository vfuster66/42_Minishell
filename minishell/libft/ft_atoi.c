/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:15:39 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:48:43 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** La fonction atoi convertit une chaîne de caractères en un entier.
** Cette implémentation utilise une boucle while pour parcourir la chaîne.
*/

int	ft_atoi(char const *str)
{
	// Variable de boucle
	int		i;
	// Entier résultant     
	long	result;
	// Signe de l'entier résultant 
	long	sign;   

	i = 0;
	sign = 1;
	result = 0;

	// Ignorer les espaces et les caractères de tabulation au début de la chaîne
	while (str[i] == ' ' || ((9 <= str[i]) && (str[i] <= 13)))
		i++;

	// Gérer le signe de l'entier, s'il y en a un
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;

	// Convertir les chiffres de la chaîne en un entier
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}

	// Retourner l'entier résultant multiplié par le signe
	return (result * sign);
}

/*#include <stdio.h>
#include "libft.h"

int	ft_atoi(char const *str);

int	main(void)
{
	char *str1 = "123";
	char *str2 = "   -987";
	char *str3 = "Hello World";
	// Dépassement de capacité pour un int
	char *str4 = "2147483648";
	// Dépassement de capacité pour un int
	char *str5 = "-2147483649"; 
	int num1, num2, num3, num4, num5;

	num1 = ft_atoi(str1);
	num2 = ft_atoi(str2);
	num3 = ft_atoi(str3);
	num4 = ft_atoi(str4);
	num5 = ft_atoi(str5);

	printf("ft_atoi(\"%s\") = %d\n", str1, num1);
	printf("ft_atoi(\"%s\") = %d\n", str2, num2);
	printf("ft_atoi(\"%s\") = %d\n", str3, num3);
	printf("ft_atoi(\"%s\") = %d\n", str4, num4);
	printf("ft_atoi(\"%s\") = %d\n", str5, num5);

	return (0);
}*/
