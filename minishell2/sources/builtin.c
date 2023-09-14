/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 13:56:26 by vfuster-          #+#    #+#             */
/*   Updated: 2023/09/14 07:35:17 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*-- Traitement commande unset --*/
/* Permet de supprimer des variables d'environnement */
int	unset_builtin(t_data *data, char **args)
{
	int	i;
	int	index;
	int	result;

	result = EXIT_SUCCESS;
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_var_key(args[i]) || ft_strchr(args[i], '=') != NULL)
		{
			error_command("unset", args[i], "not a valid identifier", false);
			result = EXIT_FAILURE;
		}
		else
		{
			index = get_env_var_index(data->env, args[i]);
			if (index != -1)
				remove_env_var(data, index);
		}
		i++;
	}
	return (result);
}

/*-- Traitement commande pwd --*/
/* Permet d'obtenir le chemin absolu du repertoire actuel */
int	pwd_builtin(t_data *data, char **args)
{
	char	buffer[PATH_MAX];
	char	*current_path;

	(void)args;
	if (data->working_directory)
	{
		ft_putendl_fd(data->working_directory, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	current_path = getcurrent_path(buffer, PATH_MAX);
	if (current_path)
	{
		ft_putendl_fd(current_path, STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	error_command("pwd", NULL, strerror(errno), errno);
	return (EXIT_FAILURE);
}

/*-- Traitement commande export --*/
/* Permet d'extraire la cle et la valeur */
static char	**split_key_value(char *input)
{
	char	**temp;
	char	*split_point;

	split_point = ft_strchr(input, '=');
	temp = malloc(sizeof * temp * (2 + 1));
	temp[0] = ft_substr(input, 0, split_point - input);
	temp[1] = ft_substr(split_point, 1, ft_strlen(split_point));
	temp[2] = NULL;
	return (temp);
}

/* Permet d'ajouter des variables d'environnement*/
int	export_builtin(t_data *data, char **args)
{
	int		i;
	char	**temp;
	int		result;

	result = EXIT_SUCCESS;
	i = 1;
	if (!args[i])
		return (env_builtin(data, NULL));
	while (args[i])
	{
		if (!is_valid_env_var_key(args[i]))
		{
			error_command("export", args[i], "not a valid identifier", false);
			result = EXIT_FAILURE;
		}
		else if (ft_strchr(args[i], '=') != NULL)
		{
			temp = split_key_value(args[i]);
			set_env_var(data, temp[0], temp[1]);
			free_array(temp);
		}
		i++;
	}
	return (result);
}

/*-- Traitement commande exit --*/
/* Verifie si un nombre entier non signe est en dehors des limites*/
static bool	is_out_of_range(int negative, unsigned long long number, bool *error)
{
	if ((negative == 1 && number > LONG_MAX)
		|| (negative == -1 && number > -(unsigned long)LONG_MIN))
		*error = true;
	return (*error);
}

/* Permet de convertir une chaine de caracteres en entier */
static int	ft_atoi_long(const char *str, bool *error)
{
	unsigned long long	result;
	int					sign;
	int					i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] && is_whitespace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		result = (result * 10) + (str[i] - '0');
		if (is_out_of_range(sign, result, error))
			break ;
		i++;
	}
	return (result * sign);
}

/* Extrait et valide un code de sortie */
static int	get_exit_code(char *arg, bool *error)
{
	unsigned long long	i;

	if (!arg)
		return (g_last_execution_code);
	i = 0;
	while (is_whitespace(arg[i]))
		i++;
	if (arg[i] == '\0')
		*error = true;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!ft_isdigit(arg[i]))
		*error = true;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && !is_whitespace(arg[i]))
			*error = true;
		i++;
	}
	i = ft_atoi_long(arg, error);
	return (i % 256);
}

/* Determine si un mode silencieux est active */
static bool	is_quiet_mode(t_data *data)
{
	t_command	*cmd;

	cmd = data->cmd;
	if (!cmd)
		return (false);
	if (cmd->next != NULL || cmd->prev != NULL)
		return (true);
	return (false);
}

/* Utilise pour quitter le shell */
int	exit_builtin(t_data *data, char **args)
{
	int		exit_code;
	bool	error;
	bool	quiet;

	quiet = is_quiet_mode(data);
	error = false;
	if (!quiet && data->interactive)
		ft_putendl_fd("exit", 2);
	if (!args || !args[1])
		exit_code = g_last_execution_code;
	else
	{
		exit_code = get_exit_code(args[1], &error);
		if (error)
			exit_code = error_command("exit", args[1],
					"numeric argument required", 2);
		else if (args[2])
			return (error_command("exit", NULL, "too many arguments", 1));
	}
	exit_shell(data, exit_code);
	return (2);
}

/*-- Traitement commande env --*/
/* Affiche les variables d'environnement definies */
int	env_builtin(t_data *data, char **args)
{
	int	i;

	if (args && args[1])
		return (error_command("env", NULL, "too many arguments", 2));
	i = 0;
	if (!data->env)
		return (EXIT_FAILURE);
	while (data->env[i])
		ft_putendl_fd(data->env[i++], STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

/*-- Traitement commande echo --*/
/* Verifie si une str contient le flag -n */
static bool	contains_n_flag(char *arg)
{
	int		i;
	bool	n_flag_found;

	n_flag_found = false;
	i = 0;
	if (arg[i] != '-')
		return (n_flag_found);
	i++;
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		n_flag_found = true;
	return (n_flag_found);
}

/* Gere l'affichage des arguments passes a la commande echo */
static void	display_echo_args(char **args, bool skip_newline, int i)
{
	if (!args[i])
	{
		if (!skip_newline)
			ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		else if (!args[i + 1] && !skip_newline)
			ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

/* Affiche des str sur la sortie standard avec ou sans modification */
int	echo_builtin(t_data *data, char **args)
{
	int		i;
	bool	newline_suppression;

	(void)data;
	newline_suppression = false;
	i = 1;
	while (args[i] && contains_n_flag(args[i]))
	{
		newline_suppression = true;
		i++;
	}
	display_echo_args(args, newline_suppression, i);
	return (EXIT_SUCCESS);
}

/*-- Traitement commande cd --*/
/* Sert a refleter les changements dans les repertoires */
static void	update_directory(t_data *data, char *directory_to_update)
{
	set_env_var(data, "OLDPWD", get_env_var_value(data->env, "PWD"));
	set_env_var(data, "PWD", directory_to_update);
	if (data->old_working_dir)
	{
		free_ptr(data->old_working_dir);
		data->old_working_dir = ft_strdup(data->working_directory);
	}
	if (data->working_directory)
	{
		free_ptr(data->working_directory);
		data->working_directory = ft_strdup(directory_to_update);
	}
	free_ptr(directory_to_update);
}

/* Gere les erreurs liees au changement de repertoire */
static	bool	change_directory_error(char *directory_path)
{
	if (errno == ESTALE)
		errno = ENOENT;
	error_command("cd", directory_path, strerror(errno), errno);
	return (false);
}

/* Sert a changer le repertoire de travail courant */
static bool	change_directory(t_data *data, char *path)
{
	char	*ret;
	char	*temp;
	char	cwd[PATH_MAX];

	ret = NULL;
	if (chdir(path) != 0)
		return (change_directory_error(path));
	ret = getcwd(cwd, PATH_MAX);
	if (!ret)
	{
		error_command("cd: error retrieving current directory",
			"getcwd: cannot access parent directories",
			strerror(errno), errno);
		ret = ft_strjoin(data->working_directory, "/");
		temp = ret;
		ret = ft_strjoin(temp, path);
		free_ptr(temp);
	}
	else
		ret = ft_strdup(cwd);
	update_directory(data, ret);
	return (true);
}

int	cd_builtin(t_data *data, char **args)
{
	char	*path;

	if (!args || !args[1] || is_whitespace(args[1][0])
		|| args[1][0] == '\0' || ft_strncmp(args[1], "--", 3) == 0)
	{
		path = get_env_var_value(data->env, "HOME");
		if (!path || *path == '\0' || is_whitespace(*path))
			return (error_command("cd", NULL, "HOME not set", EXIT_FAILURE));
		return (!change_dir(data, path));
	}
	if (args[2])
		return (error_command("cd", NULL, "too many arguments", EXIT_FAILURE));
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_var_value(data->env, "OLDPWD");
		if (!path)
			return (error_command("cd", NULL, "OLDPWD not set", EXIT_FAILURE));
		return (!change_dir(data, path));
	}
	return (!change_directory(data, args[1]));
}
