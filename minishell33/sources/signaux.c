/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:21:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 17:32:27 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void sigchld_handler(int signo, siginfo_t *info, void *context)
{
    t_data *shell_data = (t_data *)info->si_value.sival_ptr;

    (void)signo;
    (void)context;

    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    shell_data->running_child = 0;
    errno = saved_errno;
}

void sigint_handler(int signo)
{
    t_data *shell_data = NULL; // Vous devrez obtenir une référence à t_data ici.

    (void)signo;

    if (shell_data && shell_data->running_child)
        kill(0, SIGINT);
    else if (isatty(fileno(stdin)))
    {
        printf("\nminishell> ");
        fflush(stdout);
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void sigquit_handler(int signo)
{
    (void)signo;
}

void sigterm_handler(int signo)
{
    t_data *shell_data = NULL; // Vous devrez obtenir une référence à t_data ici.

    (void)signo;

    if (shell_data)
        shell_data->exit_requested = 1;
}

void setup_signal_handlers(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sa.sa_sigaction = sigchld_handler;
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



