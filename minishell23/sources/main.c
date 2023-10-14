/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 08:41:22 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/14 19:43:40 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **my_environment = NULL;

char    *read_user_input(void)
{
    char    *input;

    if (isatty(STDOUT_FILENO))
        input = readline("minishell> ");
    else
        input = readline(NULL);
    if (!input)
        return (NULL);
    if (input[0] != '\0')
        add_history(input);
    return (input);
}

bool is_redirection(char *token, char **output_file, char **input_delimiter, char **input_file)
{
    if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0)
    {
        *output_file = strtok(NULL, " ");
        return (true);
    }
    else if (strcmp(token, "<<") == 0)
    {
        *input_delimiter = strtok(NULL, " ");
        return (true);
    }
    else if (strcmp(token, "<") == 0)
    {
        *input_file = strtok(NULL, " ");
        return (true);
    }
    return (false);
}

void parse_redirections(char *redirections[3], char **input_file, char **output_file, char **input_delimiter)
{
    char    *token;

    token = strtok(redirections[0], " ");
    while (token != NULL)
    {
        if (is_redirection(token, output_file, input_delimiter, input_file))
        {

        }
        token = strtok(NULL, " ");
    }
}

bool handle_redirections(char **arguments, char *input_file, char *output_file)
{
    int stdin_backup;
    int stdout_backup;

    if (input_file && !handle_input_redirection(arguments, &input_file, &stdin_backup, &stdout_backup))
    {
        printf("Failed to handle input redirection\n");
        return (false);
    }
    if (output_file && !handle_output_redirection(arguments, &stdin_backup, &stdout_backup))
    {
        printf("Failed to handle output redirection\n");
        return (false);
    }
    return (true);
}

void exec_command_with_path(char *command, char **arguments, char **envp)
{
    char *path_env;
    char *path_copy;
    char *path_token;
    char full_path[1024];

    path_env = getenv("PATH");
    path_copy = strdup(path_env);
    path_token = strtok(path_copy, ":");
    while (path_token != NULL)
    {
        strcpy(full_path, path_token);
        if (full_path[strlen(full_path) - 1] != '/')
            strcat(full_path, "/");
        strcat(full_path, command);
        if (access(full_path, X_OK) == 0)
        {
            if (execve(full_path, arguments, envp) == -1) {
                perror("execve");
                return ;
            }
        }
        path_token = strtok(NULL, ":");
    }
    printf("Command not found: %s\n", command);
}

void execute_child(char *command, char **arguments, char **envp, char *redirections[3])
{
    char *input_file;
    char *output_file;
    char *input_delimiter;

    input_file = redirections[2];
    output_file = redirections[1];
    input_delimiter = redirections[0];
    parse_redirections(redirections, &input_file, &output_file, &input_delimiter);
    if (handle_redirections(arguments, input_file, output_file))
    {
        exec_command_with_path(command, arguments, envp);
        exit(EXIT_FAILURE);
    }
}

void execute_external_command(char *command, char **arguments, char **envp, char *redirections[3])
{
    pid_t child_pid;
    int status;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0)
        execute_child(command, arguments, envp, redirections);
    else
    {
        if (waitpid(child_pid, &status, 0) == -1)
        {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }
}

int split_command(char *input, char *args[], int max_args, char *redirections[3])
{
    int i = 0;
    char *token;
    char *input_delimiter;
    char *output_file;
    char *input_file;

    input_delimiter = NULL;
    output_file = NULL;
    input_file = NULL;
    token = strtok(input, " ");
    while (token != NULL && i < max_args - 1)
    {
        if (is_redirection(token, &output_file, &input_delimiter, &input_file))
            break;
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    redirections[0] = input_delimiter;
    redirections[1] = output_file;
    redirections[2] = input_file;
    return i;
}

bool restore_io(int stdin_backup, int stdout_backup)
{
    bool    ret;

    ret = true;
    if (stdin_backup != -1)
    {
        if (dup2(stdin_backup, STDIN_FILENO) == -1)
        {
            perror("dup2");
            ret = false;
        }
        close(stdin_backup);
    }
    if (stdout_backup != -1)
    {
        if (dup2(stdout_backup, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            ret = false;
        }
        close(stdout_backup);
    }
    return (ret);
}

void setup_environment(void)
{
    extern char **environ;
    my_environment = ft_dupenv(environ);
}

void cleanup_environment(void)
{
    int i;
    if (my_environment != NULL)
    {
        i = 0;
        while (my_environment[i] != NULL)
        {
            free(my_environment[i]);
            i++;
        }
        free(my_environment);
    }
}

void process_and_execute_input(char *input)
{
    char *processed_input;
    char *args[32];
    char *redirections[3];

    processed_input = process_quote(input);
    if (strcmp(processed_input, "clear_history") == 0)
        clear_history_command();
    else
    {
        split_command(processed_input, args, 32, redirections);
        if (est_builtin(args[0]))
            execute_builtin(args[0], args);
        else
            execute_external_command(args[0], args, my_environment, redirections);
    }
    free(processed_input);
}

int main(void)
{
    char    *input;
    int exit_requested;

    exit_requested = 0;
    toggle_ctrl_backslash(0);
    setup_environment();
    setup_signal_handlers();

    while (!exit_requested)
    {
        input = read_user_input();
        if (input == NULL)
        {
            printf("exit\n");
            break ;
        }
        process_and_execute_input(input);
        free(input);
    }
    cleanup_environment();
    return (0);
}





