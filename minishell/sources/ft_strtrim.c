/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:13:00 by vfuster-          #+#    #+#             */
/*   Updated: 2023/02/08 10:56:34 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strtrim(char const *s, char const *set)
{
	size_t	front;
	size_t	rear;
	char	*str;

	str = 0;
	if (s != 0 && set != 0)
	{
		front = 0;
		rear = ft_strlen(s);
		while (s[front] && ft_strchr(set, s[front]))
			front++;
		while (s[rear - 1] && ft_strchr(set, s[rear - 1]) && rear > front)
			rear--;
		str = (char *)malloc(sizeof(char) * (rear - front +1));
		if (str)
			ft_strlcpy(str, &s[front], rear - front +1);
	}
	return (str);
}
