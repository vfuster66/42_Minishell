/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:15:59 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/06 10:59:04 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

void	handle_quotes(const char **input_ptr, char **output_ptr,
		int *inside_quotes, char *quote_type)
{
	(void)output_ptr;
	if (is_quote_char(**input_ptr))
	{
		if (*inside_quotes && **input_ptr == *quote_type)
		{
			*inside_quotes = 0;
			*quote_type = '\0';
		}
		else if (!*inside_quotes)
		{
			*inside_quotes = 1;
			*quote_type = **input_ptr;
		}
		(*input_ptr)++;
	}
}

void	copy_characters(const char *input_ptr, char *output_ptr,
		int *inside_quotes, char *quote_type)
{
	while (*input_ptr != '\0')
	{
		handle_quotes(&input_ptr, &output_ptr, inside_quotes, quote_type);
		*output_ptr = *input_ptr;
		input_ptr++;
		output_ptr++;
	}
	*output_ptr = '\0';
}

char	*process_quote(char *input)
{
	const char	*input_ptr;
	char		*output_ptr;
	int			inside_quotes;
	char		quote_type;
	char		*output;

	output = (char *)malloc(strlen(input) + 1);
	if (!output)
	{
		perror("Erreur d'allocation de mémoire");
		exit(EXIT_FAILURE);
	}
	input_ptr = input;
	output_ptr = output;
	inside_quotes = 0;
	quote_type = '\0';
	copy_characters(input_ptr, output_ptr, &inside_quotes, &quote_type);
	return (output);
}
