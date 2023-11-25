/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 08:41:22 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/25 15:33:01 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	finalize_args(char *token, char *args[], int *i, char *input)
{
	if (token != input)
		args[(*i)++] = token;
	args[*i] = NULL;
	if (*i == 0)
	{
		printf("Erreur : Aucune commande entrée.\n");
		exit(EXIT_FAILURE);
	}
}

void	initialize_state(int *inside_double_quotes, int *inside_single_quotes,
		int *i, int state[])
{
	state[0] = *inside_double_quotes;
	state[1] = *inside_single_quotes;
	state[2] = *i;
	*inside_double_quotes = 0;
	*inside_single_quotes = 0;
	*i = 0;
}

void	handle_whitespace(char *input, char *args[], char **token, int state[])
{
	if (is_whitespace(*input) && !(state[0]) && !(state[1]))
	{
		*input = '\0';
		finalize_args(*token, args, &(state[2]), input);
		*token = input + 1;
	}
}

void	handle_character_state(char **input, char **args,
		char **token, int *state)
{
	if (**input == '"')
		state[0] = !state[0];
	else if (**input == '\'')
		state[1] = !state[1];
	handle_whitespace(*input, args, token, state);
}

int	split_command(char *input, char *args[], int max_args, char *file_info[4])
{
	char	*token;
	int		state[3];

	token = input;
	initialize_file_info(file_info);
	initialize_state(&state[0], &state[1], &state[2], state);
	while (*input != '\0' && state[2] < max_args - 1)
	{
		handle_character_state(&input, args, &token, state);
		if (*input == '|' && !state[0] && !state[1])
		{
			finalize_args(token, args, &(state[2]), input);
			token = input + 1;
		}
		else if (detect_redirection(token, file_info) && !state[0] && !state[1])
		{
			finalize_args(token, args, &(state[2]), input);
			token = input + 1;
		}
		input++;
	}
	finalize_args(token, args, &(state[2]), input);
	return (state[2]);
}
