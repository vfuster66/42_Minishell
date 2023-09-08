/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:18:30 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 14:41:11 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char const *s, int fd)
{
	// Vérifie si la chaine de caractères n'est pas NULL
	if (s != NULL)
	{
		// Écrit la chaine de caractères dans le fichier 'fd'
		ft_putstr_fd(s, fd);
		// Écrit un retour à la ligne ('\n') dans le fichier 'fd'
		ft_putchar_fd('\n', fd);
	}
}

/*#include <unistd.h>

void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char const *s, int fd);
void	ft_putendl_fd(char const *s, int fd);

int		main(void)
{
	int fd = STDOUT_FILENO; // On utilise la sortie standard comme exemple

	ft_putendl_fd("Bonjour, monde !", fd);
	ft_putendl_fd(NULL, fd); // Test avec une chaine de caractères NULL

	return (0);
}*/
