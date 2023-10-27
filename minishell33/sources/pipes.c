/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 15:22:00 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 17:11:13 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void split_pipe_command(t_data *shell_data, char *input)
{
    char *token;

    token = strtok(input, "|");
    shell_data->num_commands = 0;
    while (token != NULL && shell_data->num_commands < MAX_ARGS)
    {
        shell_data->commands[shell_data->num_commands] = token;
        (shell_data->num_commands)++;
        token = strtok(NULL, "|");
    }
}

void parent_process(int pipe_fds[2])
{
    close(pipe_fds[1]);
    wait(NULL);
    close(pipe_fds[0]);
}

void setup_child_process(t_data *shell_data, int i, int num_commands, int pipe_fds[2])
{
    if (i < num_commands - 1)
    {
        close(shell_data->pipe_read_fd);
        dup2(pipe_fds[1], STDOUT_FILENO);
    }
    if (i > 0)
        dup2(shell_data->stdin_fd, STDIN_FILENO);
}

int parse_and_execute_command(t_data *shell_data, int i, int num_commands)
{
    int pipe_fds[2];
    pid_t child_pid;
    int child_status;

    pipe(pipe_fds);
    child_pid = fork();
    if (child_pid == 0) {
        setup_child_process(shell_data, i, num_commands, pipe_fds);
        char *args[MAX_ARGS];
        // Remplissez args avec les arguments nécessaires.
        child_status = execute_pipe_command(shell_data, args);
        shell_data->previous_command_status = child_status;
        exit(0);
    } else
        parent_process(pipe_fds);
    return pipe_fds[0];
}

void execute_pipeline(t_data *shell_data, char *input)
{
    char *token;
    int i;

    shell_data->previous_command_status = 0;
    shell_data->num_commands = 0;
    token = strtok(input, "|");
    while (token != NULL && shell_data->num_commands < MAX_ARGS) {
        shell_data->commands[shell_data->num_commands] = token;
        (shell_data->num_commands)++;
        token = strtok(NULL, "|");
    }
    i = 0;
    while (i < shell_data->num_commands) {
        parse_and_execute_command(shell_data, i, shell_data->num_commands);
        i++;
    }
}





