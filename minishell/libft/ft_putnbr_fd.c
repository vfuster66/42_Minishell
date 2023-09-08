/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:18:50 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 14:43:37 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Affiche un entier sur la sortie standard associee au file descriptor
void	ft_putnbr_fd(int n, int fd)
{
	// Variable pour stocker la valeur absolue de 'n'
	unsigned int	nb; 

	// Si 'n' est négatif, on écrit un signe '-' dans le fichier 'fd' et on calcule la valeur absolue de 'n'
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		nb = (unsigned int)(n * -1);
	}
	// Sinon, on stocke simplement la valeur de 'n' dans 'nb'
	else
		nb = (unsigned int)n;
	// Si 'nb' est supérieur ou égal à 10, on continue de diviser 'nb' par 10 pour afficher tous les chiffres
	if (nb >= 10)
		ft_putnbr_fd(nb / 10, fd);
	// On affiche le chiffre correspondant à la division de 'nb' par 10, en ajoutant 48 à sa valeur pour obtenir son équivalent en caractère
	ft_putchar_fd((char)(nb % 10 + 48), fd);
}

/*#include <unistd.h>
#include <stdlib.h>
#include "libft.h"

int main(void)
{
    int n = -12345;
    int fd = 1; // affichage dans la console

    ft_putnbr_fd(n, fd);
    ft_putchar_fd('\n', fd);

    return (0);
}*/
