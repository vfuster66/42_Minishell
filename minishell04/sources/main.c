/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 08:41:22 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/08 22:57:24 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool configure_redirections(char **arguments, char **output_file, char **input_file, bool *append_output)
{
    int i = 0;
    while (arguments[i] != NULL)
    {
        if (strcmp(arguments[i], ">") == 0)
        {
            *output_file = arguments[i + 1];
            arguments[i] = NULL;
            return true;
        }
        else if (strcmp(arguments[i], ">>") == 0)
        {
            *output_file = arguments[i + 1];
            arguments[i] = NULL;
            *append_output = true;
            return true;
        }
        else if (strcmp(arguments[i], "<") == 0)
        {
            *input_file = arguments[i + 1];
            arguments[i] = NULL;
            return true;
        }
        i++;
    }
    return false;
}


char *search_command_path(char *command)
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
            return strdup(full_path);
        path_token = strtok(NULL, ":");
    }
    return (NULL);
}

bool execute_command_from_path(char *full_path, char **arguments, char **envp)
{
    printf("Executing command: %s\n", full_path);

    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        return false; // Indique une erreur
    }
    else if (child_pid == 0)
    {
        // Ce bloc sera exécuté dans le processus enfant
        if (execve(full_path, arguments, envp) == -1)
        {
            perror("execve");
            return false; // Indique une erreur
        }
        // Le code ici ne sera atteint que s'il y a une erreur avec execve
        // Vous pouvez afficher un message d'erreur supplémentaire si nécessaire.
        exit(EXIT_FAILURE); // Cette ligne peut être supprimée
    }
    else
    {
        // Ce bloc sera exécuté dans le processus parent
        int status;
        if (waitpid(child_pid, &status, 0) == -1)
        {
            perror("waitpid");
            return false; // Indique une erreur
        }
        // Ici, vous pouvez vérifier le code de retour (status) du processus enfant si nécessaire.
        return true; // Indique que l'exécution s'est terminée avec succès
    }
}

void search_and_execute_command(char *command, char **arguments, char **envp)
{
    char *full_path;

    full_path = search_command_path(command);
    if (full_path != NULL)
    {
        bool execution_success = execute_command_from_path(full_path, arguments, envp);
        free(full_path); // Libérez la mémoire ici
        if (!execution_success)
        {
            printf("Execution of command '%s' failed.\n", command);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("Command not found: %s\n", command);
        exit(EXIT_FAILURE);
    }
}

void execute_external_command(char *command, char **arguments, char **envp)
{
    printf("Execution of command '%s' started.\n", command);
    bool execution_success = execute_command_from_path(command, arguments, envp);
    if (!execution_success)
    {
        printf("Execution of command '%s' failed.\n", command);
        exit(EXIT_FAILURE);
    }
    printf("Execution of command '%s' completed.\n", command);
}


int split_command(char *input, char *args[], int max_args, char **input_file, char **output_file, bool *append_output)
{
    int i;
    char *token;

    i = 0;
    *input_file = NULL;
    *output_file = NULL;
    *append_output = false;

    token = strtok(input, " ");
    while (token != NULL && i < max_args - 1)
    {
        if (strcmp(token, "<") == 0 || strcmp(token, "<<") == 0)
        {
            if (i == 0)
            {
                fprintf(stderr, "Erreur : Aucune commande avant la redirection.\n");
                return -1;
            }
            *input_file = strtok(NULL, " ");
            if (*input_file == NULL)
            {
                fprintf(stderr, "Erreur : Nom de fichier d'entrée manquant.\n");
                return -1;
            }
            printf("Input redirection detected: %s\n", *input_file); // Ajout de ce printf
        }
        else if (strcmp(token, ">") == 0)
        {
            // Redirection de la sortie détectée (simple)
            if (i == 0)
            {
                fprintf(stderr, "Erreur : Aucune commande avant la redirection.\n");
                return -1;
            }
            *output_file = strtok(NULL, " ");
            if (*output_file == NULL)
            {
                fprintf(stderr, "Erreur : Nom de fichier de sortie manquant.\n");
                return -1;
            }
            printf("Output redirection detected: %s\n", *output_file); // Ajout de ce printf
            break;
        }
        else if (strcmp(token, ">>") == 0)
        {
            // Redirection de la sortie détectée (d'ajout)
            if (i == 0)
            {
                fprintf(stderr, "Erreur : Aucune commande avant la redirection.\n");
                return -1;
            }
            *output_file = strtok(NULL, " ");
            if (*output_file == NULL)
            {
                fprintf(stderr, "Erreur : Nom de fichier de sortie manquant.\n");
                return -1;
            }
            printf("Append output redirection detected: %s\n", *output_file); // Ajout de ce printf
            *append_output = true; // Indiquer la redirection d'ajout
            break;
        }
        else
        {
            args[i] = token;
            i++;
        }
        token = strtok(NULL, " ");
    }

    args[i] = NULL;

    if (i == 0)
    {
        fprintf(stderr, "Erreur : Aucune commande entrée.\n");
        return -1;
    }

    return i;
}


bool handle_input_redirection(char *input_file)
{
    if (input_file != NULL)
    {
        // Sauvegardez le descripteur de fichier standard d'entrée
        int stdin_backup = dup(STDIN_FILENO);

        if (stdin_backup == -1)
        {
            perror("dup");
            return false;
        }

        // Ouvrez le fichier d'entrée en mode lecture
        int fd = open(input_file, O_RDONLY);

        if (fd == -1)
        {
            perror("open");
            return false;
        }

        // Redirigez l'entrée standard depuis le fichier
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            return false;
        }

        // Fermez le descripteur de fichier du fichier d'entrée après l'avoir dupliqué
        close(fd);
        return true;
        printf("Input redirection complete.\n");
    }
    return false;
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



char **my_environment = NULL;

int main() {
    int exit_requested = 0;
    extern char **environ;
    char *output_file = NULL;
    char *input_file = NULL;
    char *input;
    char *args[MAX_ARGS];
    char *processed_input;
    int stdin_backup = -1;
    int stdout_backup = -1;

    toggle_ctrl_backslash(0);

    my_environment = ft_dupenv(environ);

    // Configure signal handlers
    setup_signal_handlers();

    bool append_output = false; // Ajoutez une variable pour indiquer la redirection d'ajout

    while (!exit_requested) {
        input = read_user_input();
        if (input == NULL) {
            printf("exit\n");
            break;
        }
        processed_input = process_quote(input);
        if (strcmp(processed_input, "clear_history") == 0) {
            clear_history_command();
            free(processed_input);
            free(input);
            // Continue to the next command
            continue;
        }

        // Check for "exit" command
        if (strcmp(processed_input, "exit") == 0) {
            printf("Exiting the shell...\n");
            exit_requested = 1;
            free(processed_input);
            free(input);
            break;
        }

        // Split the processed_input into args and get the output file (if any)
        int num_args = split_command(processed_input, args, 32, &input_file, &output_file, &append_output);
        if (num_args == -1) {
            // Error occurred while splitting the command
            free(processed_input);
            free(input);
            continue; // Move to the next command
        }

        if (input_file != NULL) {
            printf("main Input redirection detected: %s\n", input_file);
            if (!handle_input_redirection(input_file)) {
                perror("handle_input_redirection");
                exit(EXIT_FAILURE);
            }
        }

        // Check for and handle output redirection
        if (output_file != NULL) {
            if (!append_output) {
                printf("Output redirection detected: %s\n", output_file);
                if (!handle_output_redirection(args, &output_file, &stdin_backup, &stdout_backup)) {
                    perror("handle_output_redirection");
                    exit(EXIT_FAILURE);
                }
            } else {
                printf("Append output redirection detected: %s\n", output_file);
                if (!handle_output_append_redirection(args, &output_file, &stdin_backup, &stdout_backup)) {
                    perror("handle_output_append_redirection");
                    exit(EXIT_FAILURE);
                }
            }
        }

        if (est_builtin(args[0])) {
            execute_builtin(args[0], args);
        } else {
            execute_external_command(args[0], args, my_environment);
        }
        if (input_file != NULL) {
            if (!restore_io(stdin_backup, -1)) // Restore only standard input
            {
                perror("restore_io");
                exit(EXIT_FAILURE);
            }
        }
        if (output_file != NULL) {
            // Make sure to restore standard file descriptors after executing the external command
            if (!restore_io(stdin_backup, stdout_backup)) {
                perror("restore_io");
                exit(EXIT_FAILURE);
            }
        }

        free(processed_input);
        free(input);
    }

    // Free memory for the environment copy at the end of the program
    if (my_environment != NULL) {
        int i = 0;
        while (my_environment[i] != NULL) {
            free(my_environment[i]);
            i++;
        }
        free(my_environment);
    }
    return 0;
}



