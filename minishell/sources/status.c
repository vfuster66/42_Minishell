/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:28:42 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 07:48:03 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	toggle_ctrl_backslash(int enable)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	if (enable)
		term.c_cc[VQUIT] = '\\';
	else
		term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static t_status	*g_status = NULL;

void	set_status(t_status *status)
{
	g_status = status;
}

t_status	*get_status(void)
{
	return (g_status);
}
