/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:54:25 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 14:07:22 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*new_dest;
	unsigned char	src;
	size_t			i;          

	i = 0;
	new_dest = s;
	src = c;
	while (i++ < n)
		*new_dest++ = src;	    
	return (s);                 
}

