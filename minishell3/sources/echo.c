/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:23:14 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/06 10:20:20 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_quoted_string(const char *arg, char quote)
{
	int	j;

	j = 1;
	while (arg[j] != '\0' && arg[j] != quote)
	{
		putchar(arg[j]);
		j++;
	}
}

void	handle_escape_characters(const char *arg)
{
	int	j;

	j = 0;
	while (arg[j] != '\0')
	{
		if (arg[j] == '"' || arg[j] == '\'')
			handle_quoted_string(arg + j, arg[j]);
		else
		{
			putchar(arg[j]);
			j++;
		}
	}
}

void	builtin_echo(char *args[])
{
	char	*arg;
	int	i;
	int	new_line;
	int	first_arg;

	new_line = 1;
	i = 1;
	if (args[i] != NULL && strcmp(args[i], "-n") == 0)
	{
		new_line = 0;
		i++;
	}
	first_arg = 1;
	while (args[i] != NULL)
	{
		if (!first_arg)
			putchar(' ');
		first_arg = 0;
		arg = args[i];
		handle_escape_characters(arg);
		i++;
	}
	if (new_line)
		putchar('\n');
}
