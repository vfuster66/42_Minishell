/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 08:41:22 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/21 10:10:17 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**g_environment = 0;

void	initialize_environment(t_status *status)
{
	extern char	**environ;
	char		*file_info[4];

	initialize_file_info(file_info);
	toggle_ctrl_backslash(0);
	g_environment = ft_dupenv(environ);
	status->exit_requested = 0;
	status->running_child = 0;
	status->previous_command_status = -1;
	setup_signal_handlers();
}

char	*read_user_input(void)
{
	char	*input;

	if (isatty(STDOUT_FILENO))
		input = readline("minishell> ");
	else
		input = readline(NULL);
	rl_already_prompted = 0;
	if (!input)
		return (NULL);
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

char	*read_and_process_input(t_status *status)
{
	char	*input;

	input = read_user_input();
	if (input == NULL)
	{
		printf("exit\n");
		status->exit_requested = 1;
		return (NULL);
	}
	return (process_user_input(input, status));
}

void	free_environment(char **g_environment, char *input)
{
	int	i;

	i = 0;
	if (g_environment != NULL)
	{
		while (g_environment[i])
		{
			free(g_environment[i]);
			i++;
		}
		free(g_environment);
		free(input);
	}
}

int	main(void)
{
	t_status	status;
	char		*input;

	initialize_environment(&status);
	while (!status.exit_requested)
	{
		input = read_and_process_input(&status);
		if (input != NULL)
			execute_command(input, &status);
	}
	if (g_environment != NULL)
		free_environment(g_environment, input);
	return (0);
}
