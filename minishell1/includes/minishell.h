/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 07:34:39 by vfuster-          #+#    #+#             */
/*   Updated: 2023/09/18 17:32:51 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*-- Bibliotheques --*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <termios.h>

# include "../libft/libft.h"

# define MAX_TOKENS 100
# define MAX_PATH_LENGTH 1024
# define PATH_MAX 4096

/*-- Structures --*/
typedef struct s_command
{
    char        *command_name;
    char        *arguments[MAX_TOKENS];
    int         argument_count;
    char        *input_redirection;
    char        *output_redirection;
    int         append_output;
    char        *here_document;
    int         has_pipe;
    char        *pipe_command;
    int         last_exit_status;
}               t_command;

typedef struct EnvironmentVariable {
    char *name;
    char *value;
    struct EnvironmentVariable *next;
} EnvironmentVariable;

/*-- Variable globale --*/
char **env_copy = NULL;

/*-- Fonctions --*/
char	*read_user_input();
void    clear_tokens(t_command *command);
void 	tokenize_command(const char *input, t_command *command);
void 	analyze_tokens(t_command *cmd);
void	ctrl_d_handler(int signo);
void	ctrl_c_handler(int signo);
int			execute_command(t_command *command);
int			is_escape_char(char c);
int			is_quote_char(char c);
char	*process_quote(char *input);
void	execute_echo(char *arguments[], int argument_count);
void 	execute_cd(char *path);
void 	execute_pwd();
void	execute_export(char *var_name, char *var_value);
void 	execute_unset(char *var_name);
void 	execute_env();
void 	execute_exit(int exit_status);
int 		est_builtin(const char *command);
void 	execute_builtin(const char *command, char **arguments, int argument_count);
void 	disable_canonical_mode();
char    *trouver_chemin_absolu(const char *command_name);
int         execute_pipeline(char *commands[MAX_TOKENS], int num_commands);
void    substitute_environment_variables(char *command);
char    **copy_tab_strs(char **env);

#endif
