/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:19:18 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 14:45:25 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Écrit la chaîne de caractères 's' dans le fichier associé au 
** descripteur 'fd'.
*/
void	ft_putstr_fd(char const *s, int fd)
{
	// Vérifie si la chaîne de caractères n'est pas NULL.
	if (s != NULL)
		// Écrit la chaîne de caractères dans le fichier 
		// associé à 'fd'.
		write(fd, s, ft_strlen(s));
}

/*#include <unistd.h>
#include "libft.h"

int	main(void)
{
	int	fd;

	// Ouvre le fichier "test.txt" en écriture
	fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putendl_fd("Erreur lors de l'ouverture du fichier", 2);
		return (1);
	}

	// Écrit la chaine de caractères "Bonjour, monde !" dans le fichier "test.txt"
	ft_putstr_fd("Bonjour, monde !", fd);

	// Ferme le fichier
	close(fd);

	return (0);
}*/
