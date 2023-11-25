/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:21:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 07:37:27 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		if (*rl_line_buffer == '\0' || rl_line_buffer[rl_end - 1] != '\n')
			printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
		fflush(stdout);
		rl_forced_update_display();
		rl_already_prompted = 1;
	}
	else if (signo == SIGQUIT)
	{
	}
}

void	sigterm_handler(int signo)
{
	t_status	*status;

	(void)signo;
	status = get_status();
	status->exit_requested = 1;
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror
			("Erreur SIGINT");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("Erreur SIGQUIT");
		exit(EXIT_FAILURE);
	}
	sa.sa_handler = sigterm_handler;
	sa.sa_flags = 0;
	sigaction(SIGTERM, &sa, NULL);
}
