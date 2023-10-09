/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 09:28:42 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/08 22:43:30 by vfuster-         ###   ########.fr       */
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

# include "../libft/libft.h"

# define MAX_ARGS 32
# define MAX_INPUT_SIZE 1024

/*-- DÉCLARATION DE LA VARIABLE GLOBALE --*/
extern char **my_environment;

/*-- FONCTIONS --*/

// GESTION BUILTIN
// builtin.c
int		est_builtin(const char *command);
void	execute_export_builtin(char **arguments);
void	execute_unset_builtin(char **arguments);
void	execute_builtin(const char *command, char **arguments);

// cd.c
void	print_current_directory(void);
void	handle_absolute_path(const char *path);
void	handle_relative_path(const char *path);
void	handle_home_directory(void);
void	builtin_cd(char *path);

// echo.c
void	handle_quoted_string(const char *arg, char quote);
void	handle_escape_characters(const char *arg);
void	builtin_echo(char *args[]);

// env.c
void	builtin_env(void);

// exit.c
void	builtin_exit(int exit_status);

// export.c
char	*create_env_variable(const char *var_name, const char *var_value);
void	update_env_variable(char *var_name, const char *var_value);
void	add_env_variable(const char *var_name, const char *var_value);
void	builtin_export(char *var_name, char *var_value);

// pwd.c
void	builtin_pwd(void);

// unset.c
void	remove_env_variable(char *var_name);
void	builtin_unset(char *var_name);

//GESTION QUOTES
// quotes.c
int		is_quote_char(char c);
char	*copy_without_quotes(const char *input);
char    *process_quote(char *input);

// GESTION SIGNAUX
// signaux.c
void sigchld_handler(int signo);
void sigint_handler(int signo);
void sigquit_handler(int signo);
void sigterm_handler(int signo);
void setup_signal_handlers(void);

// UTILS
//utils.c
void    clear_history_command(void);
void	trim_spaces(char *str);
void	toggle_ctrl_backslash(int enable);

// GESTION REDIRECTIONS
// output_redirections.c
bool	open_output_file(char *output_file, int *stdout_backup,
		int *fd, int flags);
void	find_output_file(char *args[], char **output_file,
		int *i, const char *redirection);
bool	handle_output_redirection(char *args[], char **output_file, int *stdin_backup, int *stdout_backup);
bool	handle_output_append_redirection(char *args[], char **output_file, int *stdin_backup, int *stdout_backup);

// input_redirections.c
bool	handle_input_redirection(char *input_file);

// FONCTION PRINCIPALE
// main.c
// Signature de la fonction handle_redirections
bool handle_redirections(char *output_file, char *input_file, bool append_output);

// Signature de la fonction configure_redirections
bool configure_redirections(char **arguments, char **output_file, char **input_file, bool *append_output);

char    *read_user_input(void);
void	execute_external_command(char *command, char **arguments, char **envp);
int		split_command(char *input, char *args[], int max_args, char **input_file, char **output_file, bool *append_output);
bool	restore_io(int stdin_backup, int stdout_backup);
#endif
