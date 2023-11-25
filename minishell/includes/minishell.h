/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 09:28:42 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 17:26:23 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stddef.h>
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
# define INPUT_FILE 2

/*-- DÉCLARATION DE LA VARIABLE GLOBALE --*/
extern char	**g_environment;

/*-- STRUCTURES --*/
typedef struct s_status
{
	int	exit_requested;
	int	running_child;
	int	previous_command_status;
}		t_status;

/*-- FONCTIONS --*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~ COMMANDES INTERNES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// execute_internal_command.c
int			is_builtin(const char *command);
void		execute_export_builtin(char **arguments);
void		execute_unset_builtin(char **arguments);
void		execute_builtin(const char *command,
				char **arguments, t_status *status);

// cd.c
void		builtin_cd(char *path);

// echo.c
void		output_character(char c, int *first_arg);
void		handle_new_line_option(char *args[], int *i, int *new_line);
void		is_single_quote(char c, int *in_single_quotes, int *first_arg);
void		process_argument(char *arg, int *first_arg);
void		builtin_echo(char *args[]);

// env.c
void		builtin_env(void);

// exit.c
void		builtin_exit(char **arguments);

// export_1.c
void		print_all_environment(void);
int			is_matching_variable(const char *env_var, const char *var_name,
				int var_name_len);
void		update_existing_variable(int i, const char *var_name,
				const char *new_var_value);
void		remove_variable(int i);
int			is_valid_identifier(const char *var_name);

// export_2.c
char		*create_new_variable(const char *var_name,
				const char *new_var_value);
void		add_new_variable(const char *var_name, const char *new_var_value);
void		update_or_add_environment_variable(const char *var_name,
				const char *new_var_value);
char		*extract_env_value(const char *var_value);
void		builtin_export(const char *var_name, const char *var_value);

// pwd.c
void		builtin_pwd(void);

// unset.c
void		remove_env_variable(char *var_name);
void		builtin_unset(char *var_name);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~ QUOTES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// quotes.c
int			is_quote_char(char c);
char		*copy_without_quotes(const char *input);
char		*process_quote(char *input);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~ SIGNAUX ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// signaux.c
void		toggle_ctrl_backslash(int enable);
void		signal_handler(int signo);
t_status	*get_status(void);
void		sigterm_handler(int signo);
void		setup_signal_handlers(void);

/*~~~~~~~~~~~~~~~~~~~~~~~~ REDIRECTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// output_redirections.c
bool		open_output_file(char *output_file, int *stdout_backup,
				int *fd, int flags);
void		find_output_file(char *args[], char **output_file, int *i,
				const char *redirection);
bool		handle_output_redirection(char *args[], char *file_info[4],
				int *stdin_backup, int *stdout_backup);
bool		handle_output_append_redirection(char *args[], char *file_info[4],
				int *stdin_backup, int *stdout_backup);
bool		configure_output_redirection(char **arguments, char *file_info[4]);

// input_redirections.c
bool		redirect_input(char *input_file);
bool		open_input_redirection(char *input_file, int *stdin_backup,
				int *stdout_backup);
bool		handle_input_redirection(char *args[], char *file_info[4],
				int *stdin_backup, int *stdout_backup);
bool		configure_input_redirection(char **arguments, char *file_info[4]);

// heredoc.c
char		*read_line(void);
char		*append_to_buffer(char *buffer, const char *line,
				size_t buffer_size);
void		print_buffer(const char *buffer);
void		handle_input_until_delimiter(const char *delimiter, char **buffer,
				size_t *buffer_size, bool *delimiter_found);
bool		handle_input_redirection_delimiter(const char *delimiter,
				char *file_info[4]);

// redirections.c
bool		redirect_output(char *output_file, bool append_output);
void		process_redirection(char *token, char *args[],
				int *i, char *file_info[4]);
int			split_redirections(char *input, char *args[], int max_args,
				char *file_info[4]);

// execute_redirections.c
void		input_case(char *args[], char *file_info[],
				int *stdin_backup, int *stdout_backup);
void		output_case(char *args[], char *file_info[],
				int *stdin_backup, int *stdout_backup);
void		append_case(char *args[], char *file_info[],
				int *stdin_backup, int *stdout_backup);
void		redirections(char *args[], char *file_info[],
				int *stdin_backup, int *stdout_backup);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PIPES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// pipes_1.c
void		parent_process(int pipe_fds[2], int *input_fd);
void		setup_child_process(int i, int num_commands, int pipe_fds[2],
				int input_fd);

// pipes_2.c
void		pipe_input_redirection(const char *input_file);
void		pipe_output_redirection(const char *output_file,
				bool append_output);
int			pipe_execute_command(char *command_path, char *args[]);
int			execute_pipe_command(char *args[], char *input_file,
				char *output_file, bool append_output);

// pipes_3.c
int			has_redirection_symbols(char *command);
void		parse_command(char *command, char *args[],
				int max_args, char *file_info[]);
int			parse_and_execute_command(char *command,
				int cmd_info[4], int *status);
void		exec_commands(char *commands[], int num_commands);

// execute_pipeline.c
int			tokenize_commands(char *input, char *commands[],
				int max_commands);
void		execute_pipeline(char *input);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ $ ET $? ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// env_var.c
int			is_environment_variable(const char *env_var, const char *var_name);
void		substitute_variable_value(char **dollar_sign, const char *var_name,
				const char *var_value, const char *end);
char		*find_environment_variable(const char *var_name,
				char *g_environment[]);
const char	*find_variable_end(const char *start);
char		*extract_variable_name(const char *start, const char *end);

// special_command.c
void		process_variable_occurrence(char **str, char **dollar_sign,
				char *g_environment[], bool *is_literal);
void		substitute_environment_variables_in_string(char **str,
				char *g_environment[]);
char		*substitute_environment_variables(char *input,
				char *g_environment[]);
bool		is_literal_quote(char c);
char		*substitute_question_mark(char *input, int previous_command_status);

/*~~~~~~~~~~~~~~~~~~~~~ COMMANDES EXTERNES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// execute_external_command.c
pid_t		launch_child_process(char *full_path,
				char **arguments, char **g_environment);
bool		wait_for_child_process(pid_t child_pid, int *status);
bool		execute_command_from_path(char *full_path,
				char **arguments, char **g_environment, t_status *status);
void		execute_external_command(char *command,
				char **arguments, char **g_environment, t_status *status);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~ EXECUTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// execute_1.c
char		*process_user_input(char *input, t_status *status);
int			split_and_check_command(char *input, char *args[],
				int max_args, char *file_info[4]);
void		execute_command_internal_or_external(char *args[],
				t_status *status);
int			ft_contains_special_chars(char *input);

// execute_2.c
void		execute_command_with_redirections(char *input,
				t_status *status, int *stdin_backup, int *stdout_backup);
void		execute_command_without_redirections(char *input,
				t_status *status, int *stdin_backup, int *stdout_backup);
void		execute_command(char *input, t_status *status);
void		process_and_handle_special_commands(char *input,
				int *exit_requested, char *processed_input,
				char **g_environment);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PATH ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// path.c
char		*check_access_and_return(char *path, char *command);
char		*search_in_path_and_return(char *path, char *command);
char		*extract_path_from_environment(char **g_environment);
char		*search_command_path(char *command, char **g_environment);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SPLIT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// split_command_1.c
void		initialize_file_info(char *file_info[4]);
bool		detect_redirection(char *token, char *file_info[4]);
int			is_whitespace(char c);

// split_command_2.c
void		finalize_args(char *token, char *args[], int *i, char *input);
void		initialize_state(int *inside_double_quotes,
				int *inside_single_quotes, int *i, int state[]);
void		handle_whitespace(char *input, char *args[], char **token,
				int state[]);
int			split_command(char *input, char *args[], int max_args,
				char *file_info[4]);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ PRINCIPAL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// restore_io.c
bool		restore_io(int stdin_backup, int stdout_backup);
int			restore_io_for_file_info(int stdin_backup,
				int stdout_backup, char *file_info[]);
void		restore_io_and_exit(int stdin_backup,
				int stdout_backup, char *file_info[4]);
void		clear_history_command(void);

// main.c
void		initialize_environment(t_status *status);
char		*read_user_input(void);
char		*read_and_process_input(t_status *status);
void		free_environment(char **g_environment, char *input);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif
