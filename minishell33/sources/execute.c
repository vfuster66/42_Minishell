/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:49:18 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 15:45:56 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *search_command_path(t_data *shell_data, char *command) {
    char *path_env;
    char *path_copy;
    char *path_token;
    char full_path[1024];

    path_env = getenv("PATH");
    path_copy = strdup(path_env);
    path_token = strtok(path_copy, ":");
    while (path_token != NULL) {
        strcpy(full_path, path_token);
        if (full_path[strlen(full_path) - 1] != '/')
            strcat(full_path, "/");
        strcat(full_path, command);
        if (access(full_path, X_OK) == 0)
            return strdup(full_path);
        path_token = strtok(NULL, ":");
    }
    return NULL;
}

bool launch_child_process(t_data *shell_data, char *full_path, char **arguments) {
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return false;
    } else if (child_pid == 0) {
        if (execve(full_path, arguments, shell_data->environ) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

bool wait_for_child_process(pid_t child_pid) {
    int status;

    if (waitpid(child_pid, &status, 0) == -1) {
        perror("waitpid");
        return false;
    }
    return true;
}

bool execute_command_from_path(t_data *shell_data, char *full_path, char **arguments) {
    pid_t child_pid;
    int status;

    child_pid = -1;
    if (!launch_child_process(shell_data, full_path, arguments))
        return false;
    if (!wait_for_child_process(child_pid))
        return false;
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return true;
}

void search_and_execute_command(t_data *shell_data, char *command, char **arguments) {
    char *full_path;
    bool execution_success;

    execution_success = false;
    full_path = search_command_path(shell_data, command);
    if (full_path != NULL) {
        execution_success = execute_command_from_path(shell_data, full_path, arguments);
        free(full_path);
        if (!execution_success) {
            printf("Execution of command '%s' failed.\n", command);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Command not found: %s\n", command);
        exit(EXIT_FAILURE);
    }
}

