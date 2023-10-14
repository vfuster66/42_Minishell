/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 14:38:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/14 19:44:21 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool is_redirection_token(const char *token)
{
    return (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0
            || strcmp(token, "<") == 0);
}

char *get_filename(char *args[], int *index)
{
    (*index)++;
    return (args[(*index)++]);
}

void backup_stdin_stdout(int *stdin_backup, int *stdout_backup)
{
    *stdin_backup = dup(STDIN_FILENO);
    *stdout_backup = dup(STDOUT_FILENO);
    if (*stdin_backup == -1 || *stdout_backup == -1)
    {
        perror("dup");
        exit(EXIT_FAILURE);
    }
}

bool redirect_output(char *output_file, int open_flags)
{
    int fd;

    fd = open(output_file, open_flags, 0666);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return (true);
}

bool handle_output_redirection(char *args[], int *stdin_backup, int *stdout_backup)
{
    int i;
    char    *output_file;

    i = 0;
    while (args[i] != NULL)
    {
        if (is_redirection_token(args[i]))
        {
            output_file = get_filename(args, &i);
            backup_stdin_stdout(stdin_backup, stdout_backup);
            return (redirect_output(output_file, O_WRONLY | O_CREAT | O_TRUNC));
        }
        i++;
    }
    return (false);
}

bool handle_output_append_redirection(char *args[], int *stdin_backup, int *stdout_backup)
{
    int i;
    char    *output_file;

    i = 0;
    while (args[i] != NULL)
    {
        if (is_redirection_token(args[i]))
        {
            output_file = get_filename(args, &i);
            backup_stdin_stdout(stdin_backup, stdout_backup);
            return (redirect_output(output_file, O_WRONLY | O_CREAT | O_APPEND));
        }
        i++;
    }
    return (false);
}

bool redirect_input(char *input_file)
{
    int fd;

    fd = open(input_file, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return (true);
}

bool handle_input_redirection(char *args[], char **input_file, int *stdin_backup, int *stdout_backup)
{
    int i;

    i = 0;
    while (args[i] != NULL)
    {
        if (is_redirection_token(args[i]))
        {
            *input_file = get_filename(args, &i);
            backup_stdin_stdout(stdin_backup, stdout_backup);
            return redirect_input(*input_file);
        }
        i++;
    }
    return (false);
}



