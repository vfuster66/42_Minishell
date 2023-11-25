/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:28:03 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 07:36:11 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_exit(char **arguments)
{
	int	i;
	int	exit_status;

	if (arguments[1] != NULL)
	{
		i = 0;
		while (arguments[1][i] != '\0')
		{
			if (!ft_isdigit(arguments[1][i]))
				exit(1);
			i++;
		}
		exit_status = ft_atoi(arguments[1]);
		if (exit_status >= 0 && exit_status <= 255)
			exit(exit_status);
		else
		{
			printf
				("minishell: exit: %d: numeric argument required\n",
				exit_status);
			exit(1);
		}
	}
	else
		exit(1);
}
