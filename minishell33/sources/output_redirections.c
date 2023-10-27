/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:38:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 15:57:49 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool open_output_file(t_data *shell_data, int flags) {
    shell_data->stdout_backup = dup(STDOUT_FILENO);
    if (shell_data->stdout_backup == -1) {
        perror("dup");
        return false;
    }

    shell_data->stdout_fd = open(shell_data->output_file, flags, 0666);
    if (shell_data->stdout_fd == -1) {
        perror("open");
        return false;
    }

    if (dup2(shell_data->stdout_fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        return false;
    }

    return true;
}

void find_output_file(t_data *shell_data, int *i, const char *redirection) {
    while (shell_data->args[*i] != NULL) {
        if (strcmp(shell_data->args[*i], redirection) == 0) {
            (*i)++;
            if (shell_data->args[*i] != NULL) {
                shell_data->output_file = shell_data->args[*i];
            }
            break;
        }
        (*i)++;
    }
}

bool handle_output_redirection(t_data *shell_data) {
    int i = 0;
    find_output_file(shell_data, &i, ">");
    if (shell_data->output_file != NULL) {
        if (!open_output_file(shell_data, O_WRONLY | O_CREAT | O_TRUNC)) {
            exit(EXIT_FAILURE);
        }
        close(shell_data->stdout_fd);
        return true;
    }
    return false;
}

bool handle_output_append_redirection(t_data *shell_data) {
    int i = 0;
    find_output_file(shell_data, &i, ">>");
    if (shell_data->output_file != NULL) {
        if (!open_output_file(shell_data, O_WRONLY | O_CREAT | O_APPEND)) {
            exit(EXIT_FAILURE);
        }
        close(shell_data->stdout_fd);
        return true;
    }
    return false;
}

bool configure_output_redirection(t_data *shell_data) {
    int i = 0;
    while (shell_data->args[i] != NULL) {
        if (strcmp(shell_data->args[i], ">") == 0) {
            shell_data->output_file = shell_data->args[i + 1];
            shell_data->args[i] = NULL;
            return true;
        } else if (strcmp(shell_data->args[i], ">>") == 0) {
            shell_data->output_file = shell_data->args[i + 1];
            shell_data->args[i] = NULL;
            shell_data->append_output = true;
            return true;
        }
        i++;
    }
    return false;
}

