/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 09:28:42 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 17:32:53 by vfuster-         ###   ########.fr       */
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
extern char	**g_environment;

/*-- STRUCTURES --*/
typedef struct s_data
{
    int exit_requested;
    int running_child;
    char **environ;
    char *output_file;
    char *input_file;
    char *input;
    char *args[MAX_ARGS];
    char *processed_input;
    int stdin_backup;
    int stdout_backup;
    int stdin_fd;
    int stdout_fd;
    int pipe_read_fd;
    int pipe_write_fd;
    bool append_output;
    int num_args;
    char *input_delimiter;
    char *substituted_input;
    int previous_command_status;
    int num_commands;
    char *commands[MAX_ARGS];
} t_data;

typedef struct s_signal
{
    t_data *shell_data;

} t_signal;



/*-- FONCTIONS --*/

// GESTION BUILTIN
// builtin.c
int is_builtin(const char *command);
void execute_export_builtin(char **arguments);
void execute_unset_builtin(char **arguments);
void execute_builtin(const char *command, char **arguments);

// cd.c
void print_current_directory(void);
void handle_absolute_path(const char *path);
void handle_relative_path(const char *path);
void handle_home_directory(void);
void builtin_cd(char *path);

// echo.c
void	handle_quoted_string(const char *arg, char quote);
void	handle_escape_characters(const char *arg);
void builtin_echo(char *args[]);

// env.c
void builtin_env(void);

// exit.c
void builtin_exit(int exit_status);

// export.c
char	*create_env_variable(const char *var_name, const char *var_value);
void update_env_variable(const char *var_name, const char *var_value);
void add_env_variable(const char *var_name, const char *var_value);
void builtin_export(char *var_name, char *var_value);

// pwd.c
void builtin_pwd(void);

// unset.c
void remove_env_variable(char *var_name);
void builtin_unset(char *var_name);

//GESTION QUOTES
// quotes.c
int		is_quote_char(char c);
char	*copy_without_quotes(const char *input);
char *process_quote(t_data *shell_data, char *input);

// GESTION SIGNAUX
// status.c
void set_status(t_data *shell_data, int status);
int get_status(t_data *shell_data);

// signaux.c
void sigchld_handler(int signo, siginfo_t *info, void *context);
void sigint_handler(int signo);
void	sigquit_handler(int signo);
void sigterm_handler(int signo);
void setup_signal_handlers(void);

// UTILS
//utils.c
void	clear_history_command(void);
void toggle_ctrl_backslash(int enable);
void	free_environment(char **my_environment);
bool restore_io(t_data *shell_data);
char	*read_user_input(void);

// GESTION REDIRECTIONS
// output_redirections.c
bool open_output_file(t_data *shell_data, int flags);
void find_output_file(t_data *shell_data, int *i, const char *redirection);
bool handle_output_redirection(t_data *shell_data);
bool handle_output_append_redirection(t_data *shell_data);
bool configure_output_redirection(t_data *shell_data);

// input_redirections.c
bool open_input_redirection(t_data *shell_data);
bool handle_input_redirection(t_data *shell_data);
bool configure_input_redirection(t_data *shell_data);

// redirections.c
bool handle_redirections(t_data *shell_data);
bool redirect_input(t_data *shell_data);
bool redirect_output(t_data *shell_data);
bool configure_redirections(t_data *shell_data);

// heredoc.c
char	*read_line(void);
char	*append_to_buffer(char *buffer, const char *line,
		size_t buffer_size);
void	print_buffer(const char *buffer);
void	handle_input_until_delimiter(const char *delimiter, char **buffer,
		size_t *buffer_size, bool *delimiter_found);
bool	handle_input_redirection_delimiter(t_data *shell_data, const char *delimiter);

// GESTION PIPES
// pipes.c
void split_pipe_command(t_data *shell_data, char *input);
void parent_process(int pipe_fds[2]);
void setup_child_process(t_data *shell_data, int i, int num_commands, int pipe_fds[2]);
int parse_and_execute_command(t_data *shell_data, int i, int num_commands);
void execute_pipeline(t_data *shell_data, char *input);

// pipes2.c
void pipe_input_redirection(t_data *shell_data);
void pipe_output_redirection(t_data *shell_data);
int pipe_execute_command(t_data *shell_data, char *command_path, char *args[]);
int execute_pipe_command(t_data *shell_data, char *args[]);

// GESTION VARIABLE D'ENVIRONNEMENT
// env_var.c
int		is_environment_variable(const char *env_var, const char *var_name);
void	substitute_variable_value(char *dollar_sign, const char *var_name,
		const char *var_value, const char *end);
char	*find_environment_variable(const char *var_name, char *envp[]);
const char	*find_variable_end(const char *start);
char	*extract_variable_name(const char *start, const char *end);
char *substitute_environment_variables(const char *input, t_data *shell_data);

// FONCTION PRINCIPALE
// execute.c
char *search_command_path(t_data *shell_data, char *command);
bool launch_child_process(t_data *shell_data, char *full_path, char **arguments);
bool	wait_for_child_process(pid_t child_pid);
bool execute_command_from_path(t_data *shell_data, char *full_path, char **arguments);
void search_and_execute_command(t_data *shell_data, char *command, char **arguments);
void execute_external_command(t_data *shell_data, char *command, char **arguments);

// split_command.c
bool detect_redirection(t_data *shell_data, char *token);
int split_arguments(t_data *shell_data, char *input);
int split_command(t_data *shell_data, char *input);

// main.c
char *substitute_question_mark(t_data *shell_data);

#endif
