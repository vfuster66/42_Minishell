/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 08:41:22 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 15:55:32 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool detect_redirection(t_data *shell_data, char *token) {
    if (strcmp(token, "<<") == 0) {
        shell_data->input_delimiter = strtok(NULL, " ");
        return true;
    } else if (strcmp(token, "<") == 0) {
        shell_data->input_file = strtok(NULL, " ");
        return true;
    } else if (strcmp(token, ">") == 0) {
        shell_data->output_file = strtok(NULL, " ");
        return true;
    } else if (strcmp(token, ">>") == 0) {
        shell_data->output_file = strtok(NULL, " ");
        shell_data->append_output = true;
        return true;
    }
    return false;
}

int split_arguments(t_data *shell_data, char *input) {
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        if (detect_redirection(shell_data, token)) {
            break;
        } else {
            shell_data->args[i] = token;
            i++;
        }
        token = strtok(NULL, " ");
    }
    shell_data->args[i] = NULL;
    if (i == 0) {
        printf("Erreur : Aucune commande entrée.\n");
        return -1;
    }
    return i;
}

int split_command(t_data *shell_data, char *input) {
    int i = split_arguments(shell_data, input);
    if (i == -1) {
        return -1;
    }
    return i;
}

