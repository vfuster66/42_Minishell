/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 07:34:39 by vfuster-          #+#    #+#             */
/*   Updated: 2023/09/11 12:05:15 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*-- Bibliotheques --*/
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <sys/wait.h>
# include <errno.h>

# include "../libft/libft.h"

# define MAX_TOKENS 64

/*-- Structures --*/
typedef struct s_command
{
    char 	*command_name;          // Nom de la commande
    char 	*arguments[MAX_TOKENS]; // Tableau d'arguments
    int 	argument_count;         // Nombre d'arguments
	int		last_exit_status;		// Stocker le statut de sortie
    char 	*input_redirection;     // Redirection de l'entrée
    char 	*output_redirection;    // Redirection de la sortie
    int 	append_output;          // Indicateur de redirection en mode append
    int 	has_pipe;               // Indicateur de pipe
    char 	*pipe_command;          // Commande pour le pipe
} 			t_command;

/*-- Variable globale --*/
int	ctrl_c_pressed = 0;

/*-- Fonctions --*/
char	*read_user_input();
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

#endif
