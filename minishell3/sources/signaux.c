/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:21:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/06 14:20:37 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigchld_handler(int signo, int *child_running)
{
	(void)signo;
	int	saved_errno;

	saved_errno = errno;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	*child_running = 0;
	errno = saved_errno;
}

void	sigint_handler(int signo, int *child_running)
{
	(void)signo;
	if (*child_running)
		kill(0, SIGINT);
	else if (isatty(fileno(stdin)))
	{
		printf("\nminishell> ");
		fflush(stdout);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sigquit_handler(int signo)
{
	(void)signo;
}

void	sigterm_handler(int signo, int *exit_requested)
{
	(void)signo;
	*exit_requested = 1;
}

void	setup_signal_handlers(int *child_running, int *exit_requested)
{
	struct sigaction sa;

	sa.sa_handler = (void(*)(int))sigchld_handler;
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigaction(SIGCHLD, &sa, NULL);

	sa.sa_handler = (void(*)(int))sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

	sa.sa_handler = sigquit_handler;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);

	sa.sa_handler = (void(*)(int))sigterm_handler;
	sa.sa_flags = 0;
	sigaction(SIGTERM, &sa, NULL);
}
