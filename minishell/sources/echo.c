/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:23:14 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/21 16:25:37 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	output_character(char c, int *first_arg)
{
	if (!(*first_arg))
	{
		ft_putchar(' ');
		*first_arg = 1;
	}
	ft_putchar(c);
}

void	handle_new_line_option(char *args[], int *i, int *new_line)
{
	if (args[*i] != NULL && ft_strcmp(args[*i], "-n") == 0)
	{
		*new_line = 0;
		(*i)++;
	}
}

void	is_single_quote(char c, int *in_single_quotes, int *first_arg)
{
	if (*in_single_quotes)
		*in_single_quotes = 0;
	else
	{
		*in_single_quotes = 1;
		output_character(c, first_arg);
	}
}

void	process_argument(char *arg, int *first_arg)
{
	int	j;
	int	in_single_quotes;
	int	in_double_quotes;

	j = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	if (arg[j] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
	{
		in_single_quotes = 1;
		j++;
		arg[ft_strlen(arg) - 1] = '\0';
	}
	while (arg[j] != '\0')
	{
		if (arg[j] == '"')
			in_double_quotes = !in_double_quotes;
		else if (arg[j] == '\'' && !in_double_quotes)
			is_single_quote(arg[j], &in_single_quotes, first_arg);
		else
			output_character(arg[j], first_arg);
		j++;
	}
}

void	builtin_echo(char *args[])
{
	int	i;
	int	new_line;
	int	first_arg;

	i = 1;
	new_line = 1;
	first_arg = 1;
	handle_new_line_option(args, &i, &new_line);
	while (args[i] != NULL)
	{
		process_argument(args[i], &first_arg);
		i++;
		first_arg = 0;
	}
	if (new_line)
		printf("\n");
}
