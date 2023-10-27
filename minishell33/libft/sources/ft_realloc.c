/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 09:17:31 by vfuster-          #+#    #+#             */
/*   Updated: 2023/09/18 09:17:34 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_realloc(char *dest, int size)
{
	char	*tmp;
	int		len;

	len = ft_strlen(dest);
	tmp = malloc(len + size + 1);
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, dest, len + 1);
	free(dest);
	return (tmp);
}
