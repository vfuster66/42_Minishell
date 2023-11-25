/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 09:56:34 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 09:56:56 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initialize_file_info(char *file_info[4])
{
	char	*input_file;
	char	*output_file;
	char	*input_delimiter;
	char	*append_output;

	input_file = NULL;
	output_file = NULL;
	input_delimiter = NULL;
	append_output = NULL;
	file_info[0] = input_file;
	file_info[1] = output_file;
	file_info[2] = input_delimiter;
	file_info[3] = append_output;
}

bool	detect_redirection(char *token, char *file_info[4])
{
	if (ft_strcmp(token, "<<") == 0)
	{
		file_info[2] = ft_strtok(NULL, " ");
		return (true);
	}
	else if (ft_strcmp(token, "<") == 0)
	{
		file_info[0] = ft_strtok(NULL, " ");
		return (true);
	}
	else if (ft_strcmp(token, ">") == 0)
	{
		file_info[1] = ft_strtok(NULL, " ");
		return (true);
	}
	else if (ft_strcmp(token, ">>") == 0)
	{
		file_info[1] = ft_strtok(NULL, " ");
		file_info[3] = "true";
		return (true);
	}
	return (false);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

int	finalize_command(char *args[], int i)
{
	if (i == 0)
	{
		printf("Erreur : Aucune commande entrée.\n");
		return (-1);
	}
	args[i] = NULL;
	return (i);
}
