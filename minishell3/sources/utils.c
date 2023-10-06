/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:17:18 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/04 10:17:46 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_history_command()
{
	clear_history();
}

void	trim_spaces(char *str)
{
	int	length;
	int	start;
	int	end;

	length = strlen(str);
	start = 0;
	end = length - 1;
	while (start <= end && (str[start] == ' ' || str[start] == '\t'))
		start++;
	while (end >= start && (str[end] == ' ' || str[end] == '\t'))
		end--;
	if (start > 0 || end < length - 1)
	{
		memmove(str, str + start, end - start + 2);
		str[end - start + 1] = '\0';
	}
}

void	toggle_ctrl_backslash(int enable)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	if (enable)
		term.c_cc[VQUIT] = '\\';
	else
		term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
