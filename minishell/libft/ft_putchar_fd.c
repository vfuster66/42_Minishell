/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:18:16 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 14:10:01 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	// Écrit le caractère 'c' dans le fichier dont le descripteur 
	// est 'fd'
	write(fd, &c, 1);
}

/*#include <unistd.h>
#include <fcntl.h>

void	ft_putchar_fd(char c, int fd);

int		main(void)
{
	int fd;

	// Ouverture d'un fichier en écriture
	fd = open("output.txt", O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		write(2, "Erreur: impossible d'ouvrir le fichier\n", 39);
		return (1);
	}

	// Écriture de caractères dans le fichier
	ft_putchar_fd('H', fd);
	ft_putchar_fd('e', fd);
	ft_putchar_fd('l', fd);
	ft_putchar_fd('l', fd);
	ft_putchar_fd('o', fd);
	ft_putchar_fd('!', fd);

	// Fermeture du fichier
	close(fd);

	return (0);
}*/
