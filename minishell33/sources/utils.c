/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:17:18 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 17:32:47 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void clear_history_command(void) {
    clear_history();
}

void toggle_ctrl_backslash(int enable) {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    if (enable) {
        term.c_cc[VQUIT] = '\\';
    } else {
        term.c_cc[VQUIT] = _POSIX_VDISABLE;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

bool restore_io(t_data *shell_data) {
    bool ret = true;
    if (shell_data->stdin_backup != -1) {
        if (dup2(shell_data->stdin_backup, STDIN_FILENO) == -1) {
            perror("dup2");
            ret = false;
        }
        close(shell_data->stdin_backup);
    }
    if (shell_data->stdout_backup != -1) {
        if (dup2(shell_data->stdout_backup, STDOUT_FILENO) == -1) {
            perror("dup2");
            ret = false;
        }
        close(shell_data->stdout_backup);
    }
    return ret;
}

void free_environment(char **my_environment) {
    int i;
    if (g_environment != NULL) {
        i = 0;
        while (my_environment[i] != NULL) {
            free(my_environment[i]);
            i++;
        }
        free(my_environment);
    }
}

char *read_user_input(void) {
    char *input;
    if (isatty(STDOUT_FILENO)) {
        input = readline("minishell> ");
    } else {
        input = readline(NULL);
    }
    if (!input) {
        return NULL;
    }
    if (input[0] != '\0') {
        add_history(input);
    }
    return input;
}

