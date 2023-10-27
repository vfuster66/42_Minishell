/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 20:15:48 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 16:03:46 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool redirect_output(t_data *shell_data)
{
	int fd_output;
    int flags;

    if (shell_data->output_file != NULL)
	{
            flags = O_WRONLY | O_CREAT | O_APPEND;
    else
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        fd_output = open(shell_data->output_file, flags, 0666);
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

bool redirect_input(t_data *shell_data)
{
	int fd_input;

    if (shell_data->input_file != NULL)
	{
		fd_input = open(shell_data->input_file, O_RDONLY);
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

    return true;
}

bool handle_redirections(t_data *shell_data)
{
	bool output_success;
	bool input_success;

    output_success = redirect_output(shell_data);
    input_success = redirect_input(shell_data);
    return (output_success && input_success);
}

bool configure_redirections(t_data *shell_data)
{
    return (handle_redirections(shell_data));
}

