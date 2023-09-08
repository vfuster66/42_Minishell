/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:13:28 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 07:46:43 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*pnt; 

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	if (count > (count * size) / size)
		return (NULL);
	pnt = malloc(count * size);
	if (pnt == NULL)
		return (NULL);
	ft_memset((unsigned char *)pnt, 0, count * size);
	return (pnt);
}

