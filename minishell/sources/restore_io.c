/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_io.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:12:41 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/17 15:18:59 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	restore_io(int stdin_backup, int stdout_backup)
{
	bool	ret;

	ret = true;
	if (stdin_backup != -1)
	{
		if (dup2(stdin_backup, STDIN_FILENO) == -1)
		{
			perror("dup2");
			ret = false;
		}
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		if (dup2(stdout_backup, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			ret = false;
		}
		close(stdout_backup);
	}
	return (ret);
}

int	restore_io_for_file_info(int stdin_backup,
		int stdout_backup, char *file_info[])
{
	if (file_info[0] != NULL)
	{
		if (!restore_io(stdin_backup, -1))
		{
			perror("restore_io");
			return (EXIT_FAILURE);
		}
	}
	if (file_info[1] != NULL)
	{
		if (!restore_io(stdin_backup, stdout_backup))
		{
			perror("restore_io");
			return (EXIT_FAILURE);
		}
	}
	return (0);
}

void	restore_io_and_exit(int stdin_backup,
		int stdout_backup, char *file_info[4])
{
	int	result;

	result = restore_io_for_file_info(stdin_backup, stdout_backup, file_info);
	if (result != 0)
		exit(result);
}

void	clear_history_command(void)
{
	clear_history();
}
