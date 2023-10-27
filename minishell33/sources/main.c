/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 08:41:22 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 17:42:22 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void execute_external_command(t_data *shell_data, char *command, char **arguments)
{
    search_and_execute_command(shell_data, command, arguments);
}


char *substitute_question_mark(t_data *shell_data) {
    char *dollar_question;
    char *new_input;
    char *status_str;

    dollar_question = strstr(shell_data->input, "$?");
    if (dollar_question != NULL) {
        new_input = (char *)malloc(strlen(shell_data->input) + 12);
        if (new_input != NULL) {
            strncpy(new_input, shell_data->input, dollar_question - shell_data->input);
            status_str = ft_itoa(shell_data->previous_command_status);
            strcat(new_input, status_str);
            free(status_str);
            strcat(new_input, dollar_question + 2);
            return new_input;
        }
    }
    return shell_data->input;
}


char **g_environment = NULL;

int main(void)
{
    extern char **environ;

    t_data shell_data;
    shell_data.exit_requested = 0;
    shell_data.environ = environ;
    shell_data.output_file = NULL;
    shell_data.input_file = NULL;
    shell_data.input = NULL;
    shell_data.stdin_backup = -1;
    shell_data.stdout_backup = -1;
    shell_data.append_output = false;
    shell_data.num_args = 0;
    shell_data.input_delimiter = NULL;
    shell_data.substituted_input = NULL;
    shell_data.previous_command_status = 0;

    toggle_ctrl_backslash(0);
    g_environment = ft_dupenv(environ);

    setup_signal_handlers();

    while (!shell_data.exit_requested) {
        shell_data.input = read_user_input();
        if (shell_data.input == NULL) {
            printf("exit\n");
            break;
        }

        shell_data.input = substitute_question_mark(&shell_data);
        shell_data.substituted_input = substitute_environment_variables(shell_data.input, &shell_data);

        if (strchr(shell_data.input, '|') != NULL) {
            execute_pipeline(&shell_data, shell_data.input);
        } else {
            shell_data.processed_input = process_quote(&shell_data, shell_data.input);

            if (strcmp(shell_data.processed_input, "clear_history") == 0) {
                clear_history_command();
                free(shell_data.processed_input);
                free(shell_data.input);
                continue;
            }

            if (strcmp(shell_data.processed_input, "exit") == 0) {
                printf("Exiting the shell...\n");
                shell_data.exit_requested = 1;
                free(shell_data.processed_input);
                free(shell_data.input);
                break;
            }

            shell_data.num_args = split_command(&shell_data, shell_data.processed_input);

            if (shell_data.num_args == -1) {
                free(shell_data.processed_input);
                free(shell_data.input);
                continue;
            }

            if (shell_data.input_file != NULL) {
                if (!handle_input_redirection(&shell_data, shell_data.args)) {
                    perror("handle_input_redirection");
                    exit(EXIT_FAILURE);
                }
            }

            if (shell_data.output_file != NULL) {
                if (!shell_data.append_output) {
                    if (!handle_output_redirection(&shell_data, shell_data.args)) {
                        perror("handle_output_redirection");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    if (!handle_output_append_redirection(&shell_data, shell_data.args)) {
                        perror("handle_output_append_redirection");
                        exit(EXIT_FAILURE);
                    }
                }
            }

            if (shell_data.input_delimiter != NULL) {
                if (!handle_input_redirection_delimiter(&shell_data, shell_data.input_delimiter)) {
                    perror("handle_input_redirection_delimiter");
                    exit(EXIT_FAILURE);
                }
            }

            if (is_builtin(shell_data.args[0])) {
                execute_builtin(shell_data.args[0], shell_data.args);
            } else {
                execute_external_command(&shell_data, shell_data.args[0], shell_data.args, g_environment);
            }

            if (shell_data.input_file != NULL) {
                if (!restore_io(&shell_data)) {
                    perror("restore_io");
                    exit(EXIT_FAILURE);
                }
            }

            if (shell_data.output_file != NULL) {
                if (!restore_io(&shell_data)) {
                    perror("restore_io");
                    exit(EXIT_FAILURE);
                }
            }

            free(shell_data.processed_input);
            free(shell_data.input);
        }
    }

    free_environment(g_environment);
    return 0;
}


