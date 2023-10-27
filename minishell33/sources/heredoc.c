/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 11:12:23 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 16:36:34 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *read_line(void) {
    char line[1024];
    size_t len;

    if (fgets(line, sizeof(line), stdin) != NULL) {
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        return strdup(line);
    }
    return NULL;
}

char *append_to_buffer(char *buffer, const char *line, size_t buffer_size) {
    size_t line_len;
    size_t new_size;
    char *new_buffer;

    line_len = strlen(line);
    new_size = buffer_size + line_len + 1;
    new_buffer = malloc(new_size);
    if (buffer != NULL) {
        strcpy(new_buffer, buffer);
        free(buffer);
    } else
        new_buffer[0] = '\0';
    strcat(new_buffer, line);
    strcat(new_buffer, "\n");
    return new_buffer;
}

void print_buffer(const char *buffer) {
    if (buffer != NULL) {
        printf("%s", buffer);
        fflush(stdout);
    }
}

void handle_input_until_delimiter(const char *delimiter, char **buffer, size_t *buffer_size, bool *delimiter_found) {
    char *line;

    printf("heredoc> ");
    fflush(stdout);
    line = read_line();
    while (line != NULL) {
        if (strcmp(line, delimiter) == 0) {
            *delimiter_found = true;
            free(line);
            break;
        }
        *buffer = append_to_buffer(*buffer, line, *buffer_size);
        printf("heredoc> ");
        fflush(stdout);
        free(line);
        line = read_line();
    }
}

bool handle_input_redirection_delimiter(t_data *shell_data, const char *delimiter) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    bool delimiter_found = false;
    handle_input_until_delimiter(delimiter, &buffer, &buffer_size, &delimiter_found);
    shell_data->input = buffer; // Assign the buffer to the input member of shell_data
    print_buffer(buffer);
    return delimiter_found;
}

