/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 07:35:25 by vfuster-          #+#    #+#             */
/*   Updated: 2023/09/18 17:35:35 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void execute_echo(char *arguments[], int argument_count)
{
    int no_newline = 0;
    int i = 1;

    if (argument_count > 1 && strcmp(arguments[1], "-n") == 0)
    {
        no_newline = 1;
        i++;
    }
    while (i < argument_count)
    {
        printf("%s", arguments[i]);
        if (i < argument_count - 1)
            printf(" ");
        i++;
    }
    if (!no_newline)
        printf("\n");
}

void execute_cd(char *path)
{
    if (path == NULL)
    {
        fprintf(stderr, "cd: missing argument\n");
    }
    else
    {
        if (chdir(path) != 0)
        {
            perror("cd");
        }
    }
}

void execute_pwd()
{
    char current_directory[1024];

    if (getcwd(current_directory, sizeof(current_directory)) != NULL)
    {
        printf("%s\n", current_directory);
    }
    else
    {
        perror("pwd");
    }
}

void execute_export(char *var_name, char *var_value)
{
    if (var_name != NULL && var_value != NULL)
    {
        if (setenv(var_name, var_value, 1) != 0)
        {
            perror("export");
        }
    }
    else
    {
        printf("export: missing arguments\n");
    }
}

void execute_unset(char *var_name)
{
    if (var_name != NULL)
    {
        if (unsetenv(var_name) != 0)
        {
            perror("unset");
        }
    }
    else
    {
        printf("unset: missing argument\n");
    }
}

void execute_env(char **env_cpy)
{
    char *env_var;
    int i = 0;

    while (env_cpy[i] != NULL)
    {
        env_var = env_cpy[i];
        printf("%s\n", env_var);
        i++;
    }
}

void execute_exit(int exit_status)
{
    exit(exit_status);
}

int est_builtin(const char *command)
{
    if (strcmp(command, "echo") == 0 ||
        strcmp(command, "cd") == 0 ||
        strcmp(command, "pwd") == 0 ||
        strcmp(command, "export") == 0 ||
        strcmp(command, "unset") == 0 ||
        strcmp(command, "env") == 0 ||
        strcmp(command, "exit") == 0)
    {
        return 1;
    }
    return 0;
}

void execute_builtin(const char *command, char **arguments, int argument_count)
{
    if (strcmp(command, "echo") == 0)
        execute_echo(arguments, argument_count);
    else if (strcmp(command, "cd") == 0)
        execute_cd(arguments[1]);
    else if (strcmp(command, "pwd") == 0)
        execute_pwd();
    else if (strcmp(command, "export") == 0)
        execute_export(arguments[1], arguments[2]);
    else if (strcmp(command, "unset") == 0)
        execute_unset(arguments[1]);
    else if (strcmp(command, "env") == 0)
        execute_env(env_copy);
    else if (strcmp(command, "exit") == 0)
        execute_exit(0);
}


int	is_escape_char(char c)
{
	return (c =='\\');
}

int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

char *process_quote(char *input) {
    char *output;
    char *input_ptr;
    char *output_ptr;
    int escaped;
    int single_quoted;

    output = (char *)malloc(ft_strlen(input) + 1);
    if (!output) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    input_ptr = input;
    output_ptr = output;
    escaped = 0;
    single_quoted = 0;

    while (*input_ptr != '\0') {
        if (is_escape_char(*input_ptr) && !escaped && !single_quoted) {
            escaped = 1;
            input_ptr++;
        } else if (is_quote_char(*input_ptr) && !escaped) {
            if (*input_ptr == '\'') {
                single_quoted = !single_quoted;
            }
            input_ptr++;
        } else {
            *output_ptr = *input_ptr;
            input_ptr++;
            output_ptr++;
            escaped = 0;
        }
    }

    *output_ptr = '\0';
    return (output);
}


int execute_command(t_command *command)
{
    int status;
    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (child_pid == 0)
    {
        char *command_path = "/tmp/.minishell_heredoc_";
        char *const argv[] = {command_path, command->arguments[0], NULL};
        execve(command_path, argv, NULL);
        perror("Erreur lors de l'exécution de la commande");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(&status);
        command->last_exit_status = WEXITSTATUS(status);
    }
    return 0;
}


void	ctrl_c_handler(int signo)
{
	(void)signo;
	//ctrl_c_pressed = 1;
}

void	ctrl_d_handler(int signo)
{
	(void)signo;
	printf("Quit\n");
	exit(EXIT_SUCCESS);
}

void	analyze_tokens(t_command *command)
{
	int	i;
	char	*env_variable;
	char	*exit_status_str;

	i = 0;
	while (i < command->argument_count)
	{
		if (ft_strcmp(command->arguments[i], ">") == 0)
		{
			if (i < command->argument_count - 1)
			{
				command->output_redirection = command->arguments[i + 1];
				command->arguments[i] = NULL;
				command->arguments[i + 1] = NULL;
				i++;
			}
		}
		else if (ft_strcmp(command->arguments[i], "<") == 0)
		{
			if (i < command->argument_count - 1)
			{
				command->input_redirection = command->arguments[i + 1];
				command->arguments[i] = NULL;
				command->arguments[i + 1] = NULL;
				i++;
			}
		}
		else if (ft_strcmp(command->arguments[i], ">>") == 0)
		{
			if (i < command->argument_count - 1)
			{
				command->output_redirection = command->arguments[i + 1];
				command->append_output = 1;
				command->arguments[i] = NULL;
				command->arguments[i + 1] = NULL;
				i++;
			}
		}
		else if (ft_strcmp(command->arguments[i], "<<") == 0)
		{
			if (i < command->argument_count - 1)
			{
				command->input_redirection = command->arguments[i + 1];
				command->append_output = 1;
				command->arguments[i] = NULL;
				command->arguments[i + 1] = NULL;
				i++;
			}
		}
		else if (ft_strcmp(command->arguments[i], "|") == 0)
		{
			command->has_pipe = 1;
			command->pipe_command = command->arguments[i + 1];
			command->arguments[i] = NULL;
			command->arguments[i + 1] = NULL;
			i++;
		}
		else if (command->arguments[i][0] == '$')
		{
			env_variable = getenv(&command->arguments[i][1]);
			if (env_variable != NULL)
			{
				free(command->arguments[i]);
				command->arguments[i] = ft_strdup(env_variable);
			}
		}
		else if (ft_strcmp(command->arguments[i], "$?") == 0)
		{
			exit_status_str = ft_itoa(command->last_exit_status);
			if (exit_status_str != NULL)
			{
				free(command->arguments[i]);
				command->arguments[i] = ft_strdup(exit_status_str);
				free(exit_status_str);
			}
		}
		i++;
	}
}

void tokenize_command(const char *input, t_command *command)
{
	char *input_copy;
	char *token;
	char *processed_token;
	int						 token_count;

    ft_memset(command, 0, sizeof(t_command));
    command->command_name = ft_strdup(input);
    input_copy = ft_strdup(input);
    token = strtok(input_copy, " ");
    token_count = 0;
    while (token != NULL && token_count < MAX_TOKENS)
    {
        processed_token = process_quote(token);
        if (processed_token != NULL)
        {
            command->arguments[token_count] = ft_strdup(processed_token);
            free(processed_token);
            token_count++;
        }
        token = strtok(NULL, " ");
    }
    command->argument_count = token_count;
    free(input_copy);
}


char	*read_user_input()
{
	char	*input = readline("minishell> ");
	size_t	input_len;

	if (input == NULL)
	{
		printf("Erreur lors de la lecture de l'entree\n");
		exit(EXIT_FAILURE);
	}
	input_len = ft_strlen(input);
	if (input_len > 0 && input[input_len - 1] == '\n')
		input[input_len - 1] = '\0';
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

void disable_canonical_mode()
{
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &new_termios);
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

char **copy_tab_strs(char **env)
{
    char    **copie_tab;
    int     i;

    i = 0;
    while (env[i])
        i++;
    copie_tab = (char **)malloc(sizeof(char *) * (i + 1));
    if (!copie_tab)
        return (NULL);
    i = 0;
    while (env[i])
    {
        copie_tab[i] = env[i];
        i++;
    }
    copie_tab = NULL;
    return (copie_tab);
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    // Initialiser le gestionnaire de signaux Ctrl+C (SIGINT) et Ctrl+D (EOF)
    signal(SIGINT, ctrl_c_handler);
    signal(SIGTSTP, ctrl_d_handler);

    // Copier l'environnement dans une variable locale
    env_copy = copy_tab_strs(env);
    if (env_copy == NULL)
    {
        perror("Erreur lors de la copie de l'environnement");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        // Lire l'entrée de l'utilisateur
        char *input = read_user_input();

        // Si l'entrée est vide, continuer à la prochaine itération
        if (input[0] == '\0')
            continue;

        // Analyser la commande pour la redirection, les pipes, etc.
        t_command command;
        tokenize_command(input, &command);
        analyze_tokens(&command);

        // Vérifier si la commande est un built-in
        if (est_builtin(command.command_name))
        {
            execute_builtin(command.command_name, command.arguments, command.argument_count);
        }
        else
        {
            // TODO: Exécuter une commande externe ici (non implémenté dans le code fourni)
        }

        // Libérer la mémoire
        free(command.command_name);
        for (int i = 0; i < command.argument_count; i++)
        {
            free(command.arguments[i]);
        }

        // Libérer la mémoire allouée pour l'entrée utilisateur
        free(input);
    }

    return 0;
}

