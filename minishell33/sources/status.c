/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:28:42 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 16:06:23 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void set_status(t_data *shell_data, int status)
{
    shell_data->previous_command_status = status;
}

int get_status(t_data *shell_data)
{
    return (shell_data->previous_command_status);
}

