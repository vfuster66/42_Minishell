/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:15:48 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/08 22:57:18 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool redirect_output(char *output_file, bool append_output)
{
	int fd_output;
    int flags;

    if (output_file != NULL)
	{
        if (append_output)
            flags = O_WRONLY | O_CREAT | O_APPEND;
        else
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        fd_output = open(output_file, flags, 0666);
        if (fd_output == -1)
		{
            perror("open");
            return (false);
        }
        if (dup2(fd_output, STDOUT_FILENO) == -1)
		{
            perror("dup2");
            return (false);
        }
        close(fd_output);
    }
    return (true);
}

bool redirect_input(char *input_file)
{
    int fd_input;

    if (input_file != NULL)
    {
        printf("Redirecting input from file: %s\n", input_file); // Ajoutez cette ligne de débogage
        fd_input = open(input_file, O_RDONLY);
        if (fd_input == -1)
        {
            perror("open");
            return (false);
        }
        if (dup2(fd_input, STDIN_FILENO) == -1)
        {
            perror("dup2");
            return (false);
        }
        close(fd_input);
    }
    return (true);
}


bool handle_redirections(char *output_file, char *input_file, bool append_output)
{
    bool output_success = redirect_output(output_file, append_output);
    bool input_success = redirect_input(input_file);

    if (output_success)
    {
        printf("Output redirection started.\n");
    }

    if (input_success)
    {
        printf("Input redirection started.\n");
    }

    return (output_success && input_success);
}
