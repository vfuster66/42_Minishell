/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 14:04:08 by vfuster-          #+#    #+#             */
/*   Updated: 2023/09/18 10:34:29 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int is_delimiter(char c, const char *delimiters)
{
    while (*delimiters != '\0')
	{
        if (c == *delimiters)
		{
            return (1);
        }
        delimiters++;
    }
    return (0);
}

char *ft_strtok(char *str, const char *delimiters)
{
    static char *next_token;
    char *start;

	next_token = NULL;
    if (str == NULL)
	{
        if (next_token == NULL)
		{
            return (NULL);
        }
        str = next_token;
    }
    while (*str != '\0' && is_delimiter(*str, delimiters))
	{
        str++;
    }
    if (*str == '\0')
	{
        next_token = NULL;
        return (NULL);
    }
    start = str;

    while (*str != '\0' && !is_delimiter(*str, delimiters))
	{
        str++;
    }

    if (*str != '\0')
	{
        *str = '\0';
        next_token = str + 1;
    }
	else
	{
        next_token = NULL;
    }

    return (start);
}
