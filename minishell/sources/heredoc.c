/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 11:12:23 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/24 09:08:50 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*read_line(void)
{
	char	*line;

	line = readline(NULL);
	if (line != NULL && line[0] != '\0')
		add_history(line);
	return (line);
}

char	*append_to_buffer(char *buffer, const char *line, size_t buffer_size)
{
	size_t	line_len;
	size_t	new_size;
	char	*new_buffer;

	line_len = ft_strlen(line);
	new_size = buffer_size + line_len + 2;
	new_buffer = (char *)malloc(new_size + 1);
	if (new_buffer == NULL)
	{
		printf("ERROR: Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	if (buffer != NULL)
	{
		ft_strcpy(new_buffer, buffer);
		free(buffer);
	}
	else
		new_buffer[0] = '\0';
	ft_strcat(new_buffer, line);
	ft_strcat(new_buffer, "\n");
	return (new_buffer);
}

void	print_buffer(const char *buffer)
{
	if (buffer != NULL)
		printf("%s", buffer);
}

void	handle_input_until_delimiter(const char *delimiter, char **buffer,
		size_t *buffer_size, bool *delimiter_found)
{
	char	*line;

	printf("heredoc> ");
	line = read_line();
	while (line != NULL)
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			*delimiter_found = true;
			free(line);
			break ;
		}
		*buffer = append_to_buffer(*buffer, line, *buffer_size);
		*buffer_size = strlen(*buffer);
		printf("heredoc> ");
		free(line);
		line = read_line();
	}
}

bool	handle_input_redirection_delimiter(const char *delimiter,
		char *file_info[4])
{
	char	*buffer;
	size_t	buffer_size;
	bool	delimiter_found;

	buffer = NULL;
	buffer_size = 0;
	delimiter_found = false;
	handle_input_until_delimiter(delimiter, &buffer, &buffer_size,
		&delimiter_found);
	file_info[2] = buffer;
	print_buffer(buffer);
	free(buffer);
	return (delimiter_found);
}
