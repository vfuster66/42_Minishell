/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:26:48 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 16:54:05 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void builtin_env(void)
{
    char **env = g_environment;
    int i = 0;

    while (env[i] != NULL)
    {
        printf("%s\n", env[i]);
        i++;
    }
}

