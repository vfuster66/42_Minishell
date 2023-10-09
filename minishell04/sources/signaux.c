/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:21:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/08 16:26:53 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

struct s_processtatus
{	
       	int	exit_requested;
	int	running_child;
}		t_processstatus;

struct  s_processtatus	process_status = {0, 0};

void	sigchld_handler(int signo)
{
	(void)signo;
	int	saved_errno;

	saved_errno = errno;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	t_processstatus.running_child = 0;
	errno = saved_errno;
}

void	sigint_handler(int signo)
{
	(void)signo;
	if (t_processstatus.running_child)
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

void	sigterm_handler(int signo)
{
	(void)signo;
	t_processstatus.exit_requested = 1;
}

void	setup_signal_handlers(void)
{
    struct sigaction	sa;

    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    sa.sa_handler = sigint_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = sigquit_handler;
    sa.sa_flags = 0;
    sigaction(SIGQUIT, &sa, NULL);

    sa.sa_handler = sigterm_handler;
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);
}




