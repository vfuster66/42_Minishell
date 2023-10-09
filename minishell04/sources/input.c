/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:09:58 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/08 20:10:39 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Fonction pour initialiser le prompt et l'historique
char    *read_user_input(void)
{
    char    *input;

    if (isatty(STDOUT_FILENO))
        // La sortie standard est un terminal interactif, afficher le prompt
        input = readline("minishell> ");
    else
        // Si la sortie est redirigée, n'affichez pas le prompt
        input = readline(NULL);
    if (!input)
        // Gestion de l'erreur ou de la fin de fichier (Ctrl-D)
        return (NULL);
    if (input[0] != '\0')
        add_history(input);
    return (input);
}
