/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:29:07 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/14 17:38:21 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int est_builtin(const char *command)
{
    char *command_copy;
    char *token;

    command_copy = strdup(command);
    if (command_copy == NULL)
    {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    token = strtok(command_copy, " \t\n");
    if (strcmp(token, "echo") == 0
            || strcmp(token, "cd") == 0
            || strcmp(token, "pwd") == 0
            || strcmp(token, "export") == 0
            || strcmp(token, "unset") == 0
            || strcmp(token, "env") == 0
            || strcmp(token, "exit") == 0)
    {
        free(command_copy);
        return (1);
    }
    return (0);
}

void execute_builtin(const char *command, char **arguments)
{
    if (strcmp(command, "echo") == 0)
        builtin_echo(arguments);
    else if (strcmp(command, "cd") == 0)
        builtin_cd(arguments[1]);
    else if (strcmp(command, "pwd") == 0)
        builtin_pwd();
    else if (strcmp(command, "export") == 0)
        execute_export(arguments);
    else if (strcmp(command, "unset") == 0)
        execute_unset(arguments);
    else if (strcmp(command, "env") == 0)
        builtin_env();
    else if (strcmp(command, "exit") == 0)
        builtin_exit(0);
}
