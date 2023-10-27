/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strpbrk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 09:17:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/09/18 09:26:44 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strpbrk(const char *s1, const char *s2)
{
	const char	*debut_s2 = s2;

	while (*s1)
	{
		s2 = debut_s2;
		while (*s2)
		{
			if (*s1 == *s2)
				return ((char *)s1);
			s2++;
		}
		s1++;
	}
	return (NULL);
}
