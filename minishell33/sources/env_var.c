/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:41:23 by vfuster-          #+#    #+#             */
/*   Updated: 2023/10/27 16:19:12 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_environment_variable(const char *env_var, const char *var_name)
{
    return (strncmp(env_var, var_name, strlen(var_name)) == 0 && env_var[strlen(var_name)] == '=');
}

void substitute_variable_value(char *dollar_sign, const char *var_name, const char *var_value, const char *end)
{
    size_t var_name_length = strlen(var_name);
    size_t var_value_length = strlen(var_value);
    memmove(dollar_sign + var_name_length + 1, end, strlen(end) + 1);
    strncpy(dollar_sign, var_value, var_value_length);
}

char *find_environment_variable(const char *var_name, char *envp[])
{
    int i = 0;
    while (envp[i] != NULL)
    {
        if (is_environment_variable(envp[i], var_name))
            return (envp[i] + strlen(var_name) + 1);
        i++;
    }
    return (NULL);
}

const char *find_variable_end(const char *start)
{
    const char *end = start + 1;
    while (*end != '\0' && (*end == '_' || isalnum(*end)))
        end++;
    return end;
}

char *extract_variable_name(const char *start, const char *end)
{
    size_t var_name_length = end - start - 1;
    char *var_name = (char *)malloc(var_name_length + 1);
    if (var_name == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strncpy(var_name, start + 1, var_name_length);
    var_name[var_name_length] = '\0';
    return var_name;
}

char *substitute_environment_variables(const char *input, t_data *shell_data)
{
    char *output = strdup(input);
    if (output == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    char *dollar_sign = strchr(output, '$');
    while (dollar_sign != NULL)
    {
        const char *end = find_variable_end(dollar_sign);
        char *var_name = extract_variable_name(dollar_sign, end);
        char *var_value = find_environment_variable(var_name, shell_data->environ);
        if (var_value != NULL)
            substitute_variable_value(dollar_sign, var_name, var_value, end);
        dollar_sign = strchr(dollar_sign + 1, '$');
        free(var_name);
    }
    return output;
}

