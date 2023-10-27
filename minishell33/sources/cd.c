/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 09:19:06 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 17:14:52 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void print_current_directory(void)
{
    char current_directory[1024];

    if (getcwd(current_directory, sizeof(current_directory)) != NULL)
        printf("%s\n", current_directory);
    else
        perror("pwd");
}

void handle_absolute_path(const char *path)
{
    if (chdir(path) != 0)
        perror("cd");
    else
        print_current_directory(shell_data);
}

void handle_relative_path(const char *path)
{
    char current_directory[1024];

    if (getcwd(current_directory, sizeof(current_directory)) != NULL)
    {
        strcat(current_directory, "/");
        strcat(current_directory, path);
        if (chdir(current_directory) != 0)
            perror("cd");
        else
            print_current_directory(shell_data);
    }
    else
        perror("pwd");
}

void handle_home_directory(void)
{
    char *home_directory;

    home_directory = getenv("HOME");
    if (home_directory != NULL)
    {
        if (chdir(home_directory) != 0)
            perror("cd");
        else
            printf("%s\n", home_directory);
    }
    else
        printf("cd: HOME directory not set\n");
}

void builtin_cd(char *path)
{
    if (path == NULL)
    {
        printf("cd: missing argument\n");
        return;
    }
    if (strcmp(path, "..") == 0)
        handle_absolute_path(path);
    else if (strcmp(path, "~") == 0)
        handle_home_directory();
    else if (path[0] == '/')
        handle_absolute_path(path);
    else
        handle_relative_path(path);
}

