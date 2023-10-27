/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 11:12:23 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 15:58:24 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool open_input_redirection(t_data *shell_data) {
    shell_data->stdin_backup = dup(STDIN_FILENO);
    shell_data->stdout_backup = dup(STDOUT_FILENO);
    if (shell_data->stdin_backup == -1 || shell_data->stdout_backup == -1) {
        perror("dup");
        return false;
    }
    int fd = open(shell_data->input_file, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return true;
}

bool handle_input_redirection(t_data *shell_data) {
    bool redirection = false;
    int i = 0;
    while (shell_data->args[i] != NULL) {
        if (strcmp(shell_data->args[i], "<") == 0) {
            redirection = true;
            continue;
        }
        if (redirection) {
            shell_data->input_file = shell_data->args[i];
            redirection = false;
        }
        i++;
    }
    if (shell_data->input_file != NULL) {
        return open_input_redirection(shell_data);
    }
    return false;
}

bool configure_input_redirection(t_data *shell_data) {
    int i = 0;
    while (shell_data->args[i] != NULL) {
        if (strcmp(shell_data->args[i], "<") == 0) {
            shell_data->input_file = shell_data->args[i + 1];
            shell_data->args[i] = NULL;
            return true;
        }
        i++;
    }
    return false;
}

