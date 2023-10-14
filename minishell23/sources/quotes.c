/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:15:59 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/14 17:37:06 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_quote_char(char c)
{
    return (c == '\'' || c == '"');
}

char *remove_quotes(const char *input)
{
    char *output;
    const char *input_ptr;
    char *output_ptr;

    output = (char *)malloc(strlen(input) + 1);
    if (!output)
    {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }
    input_ptr = input;
    output_ptr = output;
    while (*input_ptr != '\0')
    {
        if (is_quote_char(*input_ptr))
            input_ptr++;
        else
        {
            *output_ptr = *input_ptr;
            input_ptr++;
            output_ptr++;
        }
    }
    *output_ptr = '\0';
    return (output);
}

char *process_quote(char *input)
{
    char *output;
    output = remove_quotes(input);
    return (output);
}



