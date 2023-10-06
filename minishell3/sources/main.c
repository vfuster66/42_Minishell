/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 08:41:22 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/06 14:54:19 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*read_user_input(void)
{
	char	*input;

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

bool	redirect_input(char *input_file)
{
	int	fd_input;

	if (input_file != NULL)
	{
		fd_input = open(input_file, O_RDONLY);
		if (fd_input == -1)
		{
			perror("open");
			return (false);
		}
		if (dup2(fd_input, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd_input);
			return (false);
		}
		close(fd_input);
	}
	return (true);
}

bool	redirect_output(char *output_file)
{
	int	fd_output;

	if (output_file != NULL)
	{
		fd_output = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_output == -1)
		{
			perror("open");
			return (false);
		}
		if (dup2(fd_output, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd_output);
			return (false);
		}
		close(fd_output);
	}
	return (true);
}

void	execute_external_command(char *command, char **arguments, char **envp)
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (child_pid == 0)
		execute_child_process(command, arguments, envp);
	else
		execute_parent_process(child_pid);
}

void	execute_command(char *command, char **arguments, char **envp)
{
	char	*full_path;

	full_path = find_executable_path(command);
	if (full_path != NULL)
	{
		printf("Executing: %s\n", full_path);
		if (execve(full_path, arguments, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		printf("Command not found: %s\n", command);
		exit(EXIT_FAILURE);
	}
}

void	execute_child_process(char *command, char **arguments, char **envp)
{
	char	*output_file;
	char	*input_file;
	int		i;

	output_file = NULL;
	input_file = NULL;
	i = 0;
	while (arguments[i] != NULL) 
	{
		if (strcmp(arguments[i], ">") == 0)
		{
			output_file = arguments[i + 1];
			arguments[i] = NULL;
			break ;
		}
		i++;
	}
	i = 0;
	while (arguments[i] != NULL)
	{
		if (strcmp(arguments[i], "<") == 0)
		{
			input_file = arguments[i + 1];
			arguments[i] = NULL;
			break ;
		}
		i++;
	}
	if (!redirect_output(input_file, output_file))
		exit(EXIT_FAILURE);
	execute_command(command, arguments, envp);
}

void	execute_parent_process(pid_t child_pid)
{
	int	status;

	if (waitpid(child_pid, &status, 0) == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
}

char	*find_executable_path(char *command)
{
	char	*path_env;
	char	*path_copy;
	char	*path_token;
	char	full_path[1024];

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
			free(path_copy);
			return (strdup(full_path));
		}
		path_token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

int	split_command(char *input, char *args[], int max_args, char *redirections[])
{
	int		i;
	char	*token;

	i = 0;
	redirections[0] = NULL;
	redirections[1] = NULL;
	token = strtok(input, " ");
	while (token != NULL && i < max_args - 1)
	{
		if (strcmp(token, "<") == 0 || strcmp(token, "<<") == 0)
		{
			if (i == 0)
			{
				printf("Erreur : Aucune commande avant la redirection.\n");
				return (-1);
			}
			redirections[0] = strtok(NULL, " ");
			if (redirections[0] == NULL)
			{
				printf("Erreur : Nom de fichier d'entrée manquant.\n");
				return (-1);
			}
		}
		else if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0)
		{
			if (i == 0)
			{
				printf("Erreur : Aucune commande avant la redirection.\n");
				return (-1);
			}
			redirections[1] = strtok(NULL, " ");
			if (redirections[1] == NULL)
			{
				printf("Erreur : Nom de fichier de sortie manquant.\n");
				return (-1);
			}
			break ;
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
		printf("Erreur : Aucune commande entrée.\n");
		return (-1);
	}
	return (i);
}

bool	handle_input_redirection(char *input_file)
{
	int	stdin_backup;
	int	fd;

	if (input_file != NULL)
	{
		stdin_backup = dup(STDIN_FILENO);
		if (stdin_backup == -1)
		{
			perror("dup");
			return (false);
		}
		fd = open(input_file, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			return (false);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (false);
		}
		close(fd);
		return (true);
	}
	return (false);
}

bool	restore_io(int stdin_backup, int stdout_backup)
{
	bool	ret;

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

void	redirect_io(const char* input_file, const char* output_file, char** args, int* stdin_backup, int* stdout_backup)
{
	if (input_file != NULL)
	{
		if (!handle_input_redirection(input_file))
		{
			perror("handle_input_redirection");
			exit(EXIT_FAILURE);
		}
	}
	if (output_file != NULL)
	{
		if (strcmp(output_file, "") == 0)
		{
			if (!handle_output_redirection(args, output_file,
				stdin_backup, stdout_backup))
			{
                perror("handle_output_redirection");
                exit(EXIT_FAILURE);
            }
        } else {
            if (!handle_output_append_redirection(args, output_file, stdin_backup, stdout_backup)) {
                perror("handle_output_append_redirection");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void free_environment(char** my_environment) {
    if (my_environment != NULL) {
        int i = 0;

        while (my_environment[i] != NULL) {
            free(my_environment[i]);
            i++;
        }
        free(my_environment);
    }
}

int	main(void) 
{
	int	i;
	int	child_running = 0;
	int	exit_requested = 0;
	extern char	**environ;
	char	*output_file = NULL;
	char	*input_file = NULL;
	char	*input;
	char	*args[32];
	char	*processed_input;
	int	stdin_backup = -1;
	int	stdout_backup = -1;

	toggle_ctrl_backslash(0);
	my_environment = ft_dupenv(environ);
	setup_signal_handlers(&child_running, &exit_requested);
	while (!exit_requested)
	{
		input = read_user_input();
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		processed_input = process_quote(input);
		if (strcmp(processed_input, "clear_history") == 0)
		{
			clear_history_command();
			free(processed_input);
			free(input);
			continue ;
		}
		split_command(processed_input, args, 32);
		if (input_file != NULL)
		{
			if (!handle_input_redirection(input_file))
			{
				perror("handle_input_redirection");
				exit(EXIT_FAILURE);
			}
		}
		if (output_file != NULL)
		{
			if (strcmp(output_file, "") == 0)
			{
				if (!handle_output_redirection(args, output_file,
					&stdin_backup, &stdout_backup))
				{
					perror("handle_output_redirection");
					exit(EXIT_FAILURE);
				}
			}
			else
			{

				if (!handle_output_append_redirection(args, output_file, 						&stdin_backup, &stdout_backup))
				{
					perror("handle_output_append_redirection");
					exit(EXIT_FAILURE);
				}
			}
		}
		if (est_builtin(args[0]))
			execute_builtin(args[0], args);
		else
		{
			if (output_file != NULL)
			{

			}
			execute_external_command(args[0], args, my_environment);
		}
		if (input_file != NULL)
		{
			if (!restore_io(stdin_backup, -1))
			{
				perror("restore_io");
				exit(EXIT_FAILURE);
			}
		}
		if (output_file != NULL)
		{
			if (!restore_io(stdin_backup, stdout_backup))
			{
				perror("restore_io");
				exit(EXIT_FAILURE);
			}
		}
		free(processed_input);
		free(input);
	}
	free_environment(my_environment);
	return (0);
}
