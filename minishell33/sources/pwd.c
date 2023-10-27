/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:20:43 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 17:12:15 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void builtin_pwd(void)
{
    char current_directory[1024];

    if (getcwd(current_directory, sizeof(current_directory)) != NULL)
        printf("%s\n", current_directory);
    else
        perror("pwd");
}

