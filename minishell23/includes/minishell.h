/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 09:28:42 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/14 19:42:58 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <glob.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include <stdbool.h>

# include "../libft/includes/libft.h"

# define MAX_ARGS 32

/*-- DÉCLARATION DE LA VARIABLE GLOBALE --*/
extern char **my_environment;

/*-- FONCTIONS --*/

// GESTION BUILTIN
// builtin.c
int			est_builtin(const char *command);
void	 execute_builtin(const char *command, char **arguments);

// cd.c
void	print_current_directory();
void	handle_absolute_path(const char *path);
void	handle_relative_path(const char *path);
void	handle_home_directory();
void	builtin_cd(char *path);

// echo.c
void	handle_quoted_string(const char *arg, char quote);
void	handle_escape_characters(const char *arg);
void	builtin_echo(char *args[]);

// env.c
void	builtin_env();

// exit.c
void	builtin_exit(int exit_status);

// export.c
char	*create_env_variable(const char *var_name, const char *var_value);
void	update_env_variable(char *var_name, const char *var_value);
void	add_env_variable(const char *var_name, const char *var_value);
void	builtin_export(char *var_name, char *var_value);
void	execute_export(char **arguments);

// pwd.c
void	builtin_pwd();

// unset.c
void	builtin_unset(char *var_name);
void	execute_unset(char **arguments);

//GESTION QUOTES
// quotes.c
int is_quote_char(char c);
char *remove_quotes(const char *input);
char *process_quote(char *input);

// GESTION SIGNAUX
// signaux.c
void	sigchld_handler(int signo);
void	sigint_handler(int signo);
void	sigquit_handler(int signo);
void    sigterm_handler(int signo);
void	setup_signal_handlers();

// UTILS
//utils.c
void    clear_history_command();
void	trim_spaces(char *str);
void	toggle_ctrl_backslash(int enable);

// GESTION REDIRECTIONS
// redirections.c
bool is_redirection_token(const char *token);
char *get_filename(char *args[], int *index);
void backup_stdin_stdout(int *stdin_backup, int *stdout_backup);
bool redirect_output(char *output_file, int open_flags);
bool handle_output_redirection(char *args[], int *stdin_backup, int *stdout_backup);
bool handle_output_append_redirection(char *args[], int *stdin_backup, int *stdout_backup);
bool redirect_input(char *input_file);
bool handle_input_redirection(char *args[], char **input_file, int *stdin_backup, int *stdout_backup);

// FONCTION PRINCIPALE
// main.c
char    *read_user_input();
bool is_redirection(char *token, char **output_file, char **input_delimiter, char **input_file);
void parse_redirections(char *redirections[3], char **input_file, char **output_file, char **input_delimiter);
bool handle_redirections(char **arguments, char *input_file, char *output_file);
void exec_command_with_path(char *command, char **arguments, char **envp);
void execute_child(char *command, char **arguments, char **envp, char *redirections[3]);
void execute_external_command(char *command, char **arguments, char **envp, char *redirections[3]);
int	split_command(char *input, char *args[], int max_args, char *redirections[3]);
bool restore_io(int stdin_backup, int stdout_backup);
void setup_environment(void);
void cleanup_environment(void);
void process_and_execute_input(char *input);

#endif
