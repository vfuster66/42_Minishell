/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 15:22:00 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 17:34:12 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void pipe_input_redirection(t_data *shell_data) {
    if (shell_data->input_file != NULL) {
        int input_fd = open(shell_data->input_file, O_RDONLY);
        if (input_fd < 0) {
            perror("open");
            exit(1);
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
}

void pipe_output_redirection(t_data *shell_data) {
    if (shell_data->output_file != NULL) {
        int flags = O_WRONLY | O_CREAT;
        if (shell_data->append_output) {
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }
        int output_fd = open(shell_data->output_file, flags, 0666);
        if (output_fd < 0) {
            perror("open");
            exit(1);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
}

int pipe_execute_command(t_data *shell_data, char *command_path, char *args[]) {
    if (command_path != NULL) {
        execve(command_path, args, shell_data->environ);
        perror("execve");
        exit(1);
    } else {
        printf("Command not found: %s\n", args[0]);
        exit(1);
    }
}

int execute_pipe_command(t_data *shell_data, char *args[]) {
    char *path = search_command_path(shell_data, args[0]);
    pipe_input_redirection(shell_data);
    pipe_output_redirection(shell_data);
    int child_status = pipe_execute_command(shell_data, path, args);
    return child_status;
}

