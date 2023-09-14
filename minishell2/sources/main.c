/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 13:27:11 by vfuster-          #+#    #+#             */
/*   Updated: 2023/09/14 07:36:21 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_wds(t_data *data, char *wd)
{
	set_env_var(data, "OLDPWD", get_env_var_value(data->env, "PWD"));
	set_env_var(data, "PWD", wd);
	if (data->old_working_directory)
	{
		free_ptr(data->old_working_directory);
		data->old_working_directory = ft_strdup(data->working_directory);
	}
	if (data->working_directory)
	{
		free_ptr(data->working_directory);
		data->working_directory = ft_strdup(wd);
	}
	free_ptr(wd);
}

static	bool	chdir_errno_mod(char *path)
{
	if (errno == ESTALE)
		errno = ENOENT;
	error_command("cd", path, strerror(errno), errno);
	return (false);
}

static bool	change_dir(t_data *data, char *path)
{
	char	*ret;
	char	*tmp;
	char	cwd[PATH_MAX];

	ret = NULL;
	if (chdir(path) != 0)
		return (chdir_errno_mod(path));
	ret = getcwd(cwd, PATH_MAX);
	if (!ret)
	{
		error_command("cd: error retrieving current directory",
			"getcwd: cannot access parent directories",
			strerror(errno), errno);
		ret = ft_strjoin(data->working_directory, "/");
		tmp = ret;
		ret = ft_strjoin(tmp, path);
		free_ptr(tmp);
	}
	else
		ret = ft_strdup(cwd);
	update_wds(data, ret);
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
	return (!change_dir(data, args[1]));
}

static void	print_cmd_args(t_command *cmd)
{
	int	i;

	if (!cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		printf("\tArgs[%d] = %s\n", i, cmd->args[i]);
		i++;
	}
}

static void	print_cmd_io(t_command *cmd)
{
	if (!cmd->io_fds)
		return ;
	if (cmd->io_fds->infile)
	{
		printf("\tInfile: %s\n", cmd->io_fds->infile);
		printf("\t\tfd_in: %d\n", cmd->io_fds->fd_in);
	}
	if (cmd->io_fds->heredoc_delimiter)
		printf("\tHeredoc delimiter: %s\n", cmd->io_fds->heredoc_delimiter);
	if (cmd->io_fds->outfile)
	{
		printf("\tOutfile: %s\n", cmd->io_fds->outfile);
		printf("\t\tfd_in: %d\n", cmd->io_fds->fd_out);
	}
}

void	print_cmd_list(t_data *data)
{
	t_command	*cmd;

	cmd = data->cmd;
	printf("\n---- COMMAND LIST\n");
	while (cmd)
	{
		printf("--- Command = %s\n", cmd->command);
		print_cmd_args(cmd);
		printf("\tPipe_output = %d\n", cmd->pipe_output);
		print_cmd_io(cmd);
		if (cmd->prev == NULL)
			printf("\tprev = NULL\n");
		else
			printf("\tprev = %s\n", cmd->prev->command);
		if (cmd->next == NULL)
			printf("\tnext = NULL\n");
		else
			printf("\tnext = %s\n", cmd->next->command);
		printf("\n");
		cmd = cmd->next;
	}
	printf("\n");
}

void	print_token_type(t_token *token, char *prefix)
{
	printf("%s", prefix);
	if (token->type == SPACES)
		printf("SPACES\n");
	else if (token->type == WORD)
		printf("WORD\n");
	else if (token->type == VAR)
		printf("VAR\n");
	else if (token->type == PIPE)
		printf("PIPE\n");
	else if (token->type == INPUT)
		printf("INPUT\n");
	else if (token->type == TRUNC)
		printf("TRUNC\n");
	else if (token->type == HEREDOC)
		printf("HEREDOC\n");
	else if (token->type == APPEND)
		printf("APPEND\n");
	else if (token->type == END)
		printf("END\n");
}

void	print_token_list(t_token **tokens)
{
	t_token	*lst;
	int		i;

	lst = *tokens;
	printf("\n---- TOKEN LIST\n");
	i = 0;
	while (lst)
	{
		printf("--- Token [%d] [%p]\n", i, lst);
		printf("\tString = [%s]\n", lst->str);
		printf("\tStr backup = [%s]\n", lst->str_backup);
		print_token_type(lst, "\tType = ");
		printf("\tStatus = %d\n", lst->status);
		if (lst->prev)
			printf("\tPrev = [%p]\n", lst->prev);
		else
			printf("\tPrev = NULL\n");
		if (lst->next)
			printf("\tNext = [%p]\n", lst->next);
		else
			printf("\tNext = NULL\n");
		i++;
		lst = lst->next;
	}
}

static char	**realloc_env_vars(t_data *data, int size)
{
	char	**new_env;
	int		i;

	new_env = ft_calloc(size + 1, sizeof * new_env);
	if (!new_env)
		return (NULL);
	i = 0;
	while (data->env[i] && i < size)
	{
		new_env[i] = ft_strdup(data->env[i]);
		free_ptr(data->env[i]);
		i++;
	}
	free(data->env);
	return (new_env);
}

bool	set_env_var(t_data *data, char *key, char *value)
{
	int		idx;
	char	*tmp;

	idx = get_env_var_index(data->env, key);
	if (value == NULL)
		value = "";
	tmp = ft_strjoin("=", value);
	if (!tmp)
		return (false);
	if (idx != -1 && data->env[idx])
	{
		free_ptr(data->env[idx]);
		data->env[idx] = ft_strjoin(key, tmp);
	}
	else
	{
		idx = env_var_count(data->env);
		data->env = realloc_env_vars(data, idx + 1);
		if (!data->env)
			return (false);
		data->env[idx] = ft_strjoin(key, tmp);
	}
	free_ptr(tmp);
	return (true);
}

bool	remove_env_var(t_data *data, int idx)
{
	int	i;
	int	count;

	if (idx > env_var_count(data->env))
		return (false);
	free_ptr(data->env[idx]);
	i = idx;
	count = idx;
	while (data->env[i + 1])
	{
		data->env[i] = ft_strdup(data->env[i + 1]);
		free_ptr(data->env[i + 1]);
		count++;
		i++;
	}
	data->env = realloc_env_vars(data, count);
	if (!data->env)
		return (false);
	return (true);
}

int	env_var_count(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

int	get_env_var_index(char **env, char *var)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free_ptr(tmp);
			return (i);
		}
		i++;
	}
	free_ptr(tmp);
	return (-1);
}

char	*get_env_var_value(char **env, char *var)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(var, "=");
	if (!tmp)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(tmp, env[i], ft_strlen(tmp)) == 0)
		{
			free_ptr(tmp);
			return (ft_strchr(env[i], '=') + 1);
		}
		i++;
	}
	free_ptr(tmp);
	return (NULL);
}

bool	is_valid_env_var_key(char *var)
{
	int	i;

	i = 0;
	if (ft_isalpha(var[i]) == 0 && var[i] != '_')
		return (false);
	i++;
	while (var[i] && var[i] != '=')
	{
		if (ft_isalnum(var[i]) == 0 && var[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static char	*find_valid_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;

	cmd_path = NULL;
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (!cmd_path)
		{
			error_command("malloc", NULL,
				"an unexpected error occured", EXIT_FAILURE);
			return (NULL);
		}
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free_ptr(cmd_path);
		i++;
	}
	return (NULL);
}

static char	**get_paths_from_env(t_data *data)
{
	char	**env_paths;

	if (get_env_var_index(data->env, "PATH") == -1)
		return (NULL);
	env_paths = ft_split(get_env_var_value(data->env, "PATH"), ':');
	if (!env_paths)
		return (NULL);
	return (env_paths);
}

char	*get_cmd_path(t_data *data, char *name)
{
	char	**env_paths;
	char	*cmd;
	char	*cmd_path;

	if (!name)
		return (NULL);
	env_paths = get_paths_from_env(data);
	if (!env_paths)
		return (NULL);
	cmd = ft_strjoin("/", name);
	if (!cmd)
	{
		free_array(env_paths);
		return (NULL);
	}
	cmd_path = find_valid_cmd_path(cmd, env_paths);
	if (!cmd_path)
	{
		free_ptr(cmd);
		free_array(env_paths);
		return (NULL);
	}
	return (cmd_path);
}

bool	cmd_is_dir(char *cmd)
{
	struct stat	cmd_stat;

	ft_memset(&cmd_stat, 0, sizeof(cmd_stat));
	stat(cmd, &cmd_stat);
	return (S_ISDIR(cmd_stat.st_mode));
}

int	check_command_not_found(t_data *data, t_command *cmd)
{
	if (ft_strchr(cmd->command, '/') == NULL
		&& get_env_var_index(data->env, "PATH") != -1)
		return (error_command(cmd->command, NULL, "command not found",
				CMD_NOT_FOUND));
	if (access(cmd->command, F_OK) != 0)
		return (error_command(cmd->command, NULL, strerror(errno), CMD_NOT_FOUND));
	else if (cmd_is_dir(cmd->command))
		return (error_command(cmd->command, NULL, "Is a directory",
				CMD_NOT_EXECUTABLE));
	else if (access(cmd->command, F_OK | X_OK) != 0)
		return (error_command(cmd->command, NULL, strerror(errno),
				CMD_NOT_EXECUTABLE));
	return (EXIT_SUCCESS);
}

int	execute_builtin(t_data *data, t_command *cmd)
{
	int	ret;

	ret = CMD_NOT_FOUND;
	if (ft_strncmp(cmd->command, "cd", 3) == 0)
		ret = cd_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "echo", 5) == 0)
		ret = echo_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "env", 4) == 0)
		ret = env_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "export", 7) == 0)
		ret = export_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "pwd", 4) == 0)
		ret = pwd_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "unset", 6) == 0)
		ret = unset_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "exit", 5) == 0)
		ret = exit_builtin(data, cmd->args);
	return (ret);
}

static int	execute_sys_bin(t_data *data, t_command *cmd)
{
	if (!cmd->command || cmd->command[0] == '\0')
		return (CMD_NOT_FOUND);
	if (cmd_is_dir(cmd->command))
		return (CMD_NOT_FOUND);
	cmd->path = get_cmd_path(data, cmd->command);
	if (!cmd->path)
		return (CMD_NOT_FOUND);
	if (execve(cmd->path, cmd->args, data->env) == -1)
		error_command("execve", NULL, strerror(errno), errno);
	return (EXIT_FAILURE);
}

/* execute_local_bin:
*	Attempts to execute the given command as is, in case
*	it is a local directory file or already contains the
*	path to bin.
*	Returns CMD_NOT_FOUND if the command is not an existing executable
*	file. Returns 1 in case of failure to launch executable.
*/
static int	execute_local_bin(t_data *data, t_command *cmd)
{
	int	ret;

	ret = check_command_not_found(data, cmd);
	if (ret != 0)
		return (ret);
	if (execve(cmd->command, cmd->args, data->env) == -1)
		return (error_command("execve", NULL, strerror(errno), errno));
	return (EXIT_FAILURE);
}

int	execute_command(t_data *data, t_command *cmd)
{
	int	ret;

	if (!cmd || !cmd->command)
		exit_shell(data, error_command("child", NULL,
				"parsing error: no command to execute!", EXIT_FAILURE));
	if (!check_infile_outfile(cmd->io_fds))
		exit_shell(data, EXIT_FAILURE);
	set_pipe_fds(data->cmd, cmd);
	redirect_io(cmd->io_fds);
	close_fds(data->cmd, false);
	if (ft_strchr(cmd->command, '/') == NULL)
	{
		ret = execute_builtin(data, cmd);
		if (ret != CMD_NOT_FOUND)
			exit_shell(data, ret);
		ret = execute_sys_bin(data, cmd);
		if (ret != CMD_NOT_FOUND)
			exit_shell(data, ret);
	}
	ret = execute_local_bin(data, cmd);
	exit_shell(data, ret);
	return (ret);
}

int	g_last_exit_code;

static int	get_children(t_data *data)
{
	pid_t	wpid;
	int		status;
	int		save_status;

	close_fds(data->cmd, false);
	save_status = 0;
	wpid = 0;
	while (wpid != -1 || errno != ECHILD)
	{
		wpid = waitpid(-1, &status, 0);
		if (wpid == data->pid)
			save_status = status;
		continue ;
	}
	if (WIFSIGNALED(save_status))
		status = 128 + WTERMSIG(save_status);
	else if (WIFEXITED(save_status))
		status = WEXITSTATUS(save_status);
	else
		status = save_status;
	return (status);
}

static int	create_children(t_data *data)
{
	t_command	*cmd;

	cmd = data->cmd;
	while (data->pid != 0 && cmd)
	{
		data->pid = fork();
		if (data->pid == -1)
			return (error_command("fork", NULL, strerror(errno), EXIT_FAILURE));
		else if (data->pid == 0)
			execute_command(data, cmd);
		cmd = cmd->next;
	}
	return (get_children(data));
}

static int	prep_for_exec(t_data *data)
{
	if (!data || !data->cmd)
		return (EXIT_SUCCESS);
	if (!data->cmd->command)
	{
		if (data->cmd->io_fds
			&& !check_infile_outfile(data->cmd->io_fds))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	if (!create_pipes(data))
		return (EXIT_FAILURE);
	return (CMD_NOT_FOUND);
}

void	copy_var_value(char *new_str, char *var_value, int *j)
{
	int	k;

	k = 0;
	while (var_value[k])
	{
		new_str[*j] = var_value[k];
		k++;
		(*j)++;
	}
}

int	execute(t_data *data)
{
	int	ret;

	ret = prep_for_exec(data);
	if (ret != CMD_NOT_FOUND)
		return (ret);
	if (!data->cmd->pipe_output && !data->cmd->prev
		&& check_infile_outfile(data->cmd->io_fds))
	{
		redirect_io(data->cmd->io_fds);
		ret = execute_builtin(data, data->cmd);
		restore_io(data->cmd->io_fds);
	}
	if (ret != CMD_NOT_FOUND)
		return (ret);
	return (create_children(data));
}

char	*get_new_token_string(char *oldstr, char *var_value,
		int newstr_size, int index)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(sizeof(char) * newstr_size);
	if (!new_str)
		return (NULL);
	while (oldstr[i])
	{
		if (oldstr[i] == '$' && i == index)
		{
			copy_var_value(new_str, var_value, &j);
			i = i + var_length(oldstr + index) + 1;
			if (oldstr[i] == '\0')
				break ;
		}
		new_str[j++] = oldstr[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}

static void	update_status(t_token **token_node, char c)
{
	if (c == '\'' && (*token_node)->status == DEFAULT)
		(*token_node)->status = SQUOTE;
	else if (c == '\"' && (*token_node)->status == DEFAULT)
		(*token_node)->status = DQUOTE;
	else if (c == '\'' && (*token_node)->status == SQUOTE)
		(*token_node)->status = DEFAULT;
	else if (c == '\"' && (*token_node)->status == DQUOTE)
		(*token_node)->status = DEFAULT;
}

static bool	is_next_char_a_sep(char c)
{
	if (c == '$' || c == ' ' || c == '=' || c == '\0')
		return (true);
	else
		return (false);
}

static bool	var_between_quotes(char *str, int i)
{
	if (i > 0)
	{
		if (str[i - 1] == '\"' && str[i + 1] == '\"')
			return (true);
		else
			return (false);
	}
	return (false);
}

int	var_expander(t_data *data, t_token **token_lst)
{
	t_token	*temp;
	int		i;

	temp = *token_lst;
	while (temp)
	{
		if (temp->type == VAR)
		{
			i = 0;
			while (temp->str[i])
			{
				update_status(&temp, temp->str[i]);
				if (temp->str[i] == '$'
					&& is_next_char_a_sep(temp->str[i + 1]) == false
					&& var_between_quotes(temp->str, i) == false
					&& (temp->status == DEFAULT || temp->status == DQUOTE))
					replace_var(&temp,
						recover_val(temp, temp->str + i, data), i);
				else
					i++;
			}
		}
		temp = temp->next;
	}
	return (0);
}

char	*var_expander_heredoc(t_data *data, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$'
			&& is_next_char_a_sep(str[i + 1]) == false
			&& var_between_quotes(str, i) == false)
			str = replace_str_heredoc(str, recover_val(NULL, str + i, data), i);
		else
			i++;
	}
	return (str);
}

static int	erase_var(t_token **token_node, char *str, int index)
{
	int		i;
	int		j;
	int		len;
	char	*new_str;

	i = 0;
	j = 0;
	len = ft_strlen(str) - var_length(str + index);
	new_str = (char *)malloc(sizeof(char) * len + 1);
	if (!new_str)
		return (1);
	while (str[i])
	{
		if (str[i] == '$' && i == index)
		{
			i = i + var_length(str + index) + 1;
			if (str[i] == '\0')
				break ;
		}
		new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	free_ptr((*token_node)->str);
	(*token_node)->str = new_str;
	return (0);
}

static char	*erase_and_replace(t_token **token_node, char *str,
			char *var_value, int index)
{
	char	*newstr;
	int		newstr_size;

	newstr_size = (ft_strlen(str) - var_length(str + index)
			+ ft_strlen(var_value));
	newstr = get_new_token_string(str, var_value, newstr_size, index);
	if (token_node && *token_node)
	{
		free_ptr((*token_node)->str);
		(*token_node)->str = newstr;
	}
	return (newstr);
}

int	replace_var(t_token **token_node, char *var_value, int index)
{
	if (var_value == NULL)
	{
		if (erase_var(token_node, (*token_node)->str, index) == 1)
		{
			free_ptr(var_value);
			return (1);
		}
	}
	else
	{
		if (erase_and_replace(token_node, (*token_node)->str, \
		var_value, index) == NULL)
		{
			free_ptr(var_value);
			return (1);
		}
	}
	free_ptr(var_value);
	return (0);
}

char	*replace_str_heredoc(char *str, char *var_value, int index)
{
	char	*tmp;

	tmp = NULL;
	if (var_value == NULL)
		*str = '\0';
	else
	{
		tmp = str;
		str = erase_and_replace(NULL, str, var_value, index);
		free_ptr(tmp);
	}
	free_ptr(var_value);
	return (str);
}

static int	var_exists(t_data *data, char *var)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var);
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, len) == 0)
			return (0);
		i++;
	}
	return (1);
}

static char	*search_env_var(t_data *data, char *var)
{
	char	*str;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var);
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, len) == 0)
			break ;
		i++;
	}
	str = ft_strdup(data->env[i] + len);
	return (str);
}

char	*recover_val(t_token *token, char *str, t_data *data)
{
	char	*value;
	char	*var;

	var = identify_var(str);
	if (var && var_exists(data, var) == 0)
	{
		if (token != NULL)
			token->var_exists = true;
		value = search_env_var(data, var);
	}
	else if (var && var[0] == '?' && var[1] == '=')
		value = ft_itoa(g_last_exit_code);
	else
		value = NULL;
	free_ptr(var);
	return (value);
}

static void	change_status_to_quote(t_token **token_node, int *i)
{
	if ((*token_node)->str[*i] == '\'')
		(*token_node)->status = SQUOTE;
	if ((*token_node)->str[*i] == '\"')
		(*token_node)->status = DQUOTE;
	(*i)++;
}

static bool	if_quotes_and_default(t_token **token_node, int i)
{
	if (((*token_node)->str[i] == '\'' || (*token_node)->str[i] == '\"')
		&& (*token_node)->status == DEFAULT)
		return (true);
	else
		return (false);
}

static bool	change_back_to_default(t_token **token_node, int *i)
{
	if (((*token_node)->str[*i] == '\'' && (*token_node)->status == SQUOTE)
		|| ((*token_node)->str[*i] == '\"' && (*token_node)->status == DQUOTE))
	{
		(*token_node)->status = DEFAULT;
		(*i)++;
		return (true);
	}
	else
		return (false);
}

int	remove_quotes(t_token **token_node)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = malloc(sizeof(char) * count_len((*token_node)->str, i, i));
	if (!new_line)
		return (1);
	while ((*token_node)->str[i])
	{
		if (if_quotes_and_default(token_node, i) == true)
		{
			change_status_to_quote(token_node, &i);
			continue ;
		}
		else if (change_back_to_default(token_node, &i) == true)
			continue ;
		new_line[j++] = (*token_node)->str[i++];
	}
	new_line[j] = '\0';
	free_ptr((*token_node)->str);
	(*token_node)->str = new_line;
	(*token_node)->join = true;
	return (0);
}

int	count_len(char *str, int count, int i)
{
	int	status;

	status = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && status == DEFAULT)
		{
			if (str[i] == '\'')
				status = SQUOTE;
			if (str[i] == '\"')
				status = DQUOTE;
			i++;
			continue ;
		}
		else if ((str[i] == '\'' && status == SQUOTE)
			|| (str[i] == '\"' && status == DQUOTE))
		{
			status = DEFAULT;
			i++;
			continue ;
		}
		count++;
		i++;
	}
	return (count + 1);
}

bool	quotes_in_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}

int	handle_quotes(t_data *data)
{
	t_token	*temp;

	temp = data->token;
	while (temp)
	{
		if (quotes_in_string(temp->str) == true
			&& (!temp->prev || (temp->prev && temp->prev->type != HEREDOC)))
			remove_quotes(&temp);
		temp = temp->next;
	}
	return (0);
}

bool	is_var_compliant(char c)
{
	if (ft_isalnum(c) == 0 && c != '_')
		return (false);
	else
		return (true);
}

int	var_length(char *str)
{
	int		i;
	int		count;

	count = 0;
	i = 0;
	while (str[i] != '$')
		i++;
	i++;
	if ((str[i] >= '0' && str[i] <= '9') || str[i] == '?')
		return (count + 1);
	while (str[i])
	{
		if (is_var_compliant(str[i]) == false)
			break ;
		count++;
		i++;
	}
	return (count);
}

char	*identify_var(char *str)
{
	char	*var;
	char	*tmp;
	int		start;
	int		len;
	int		i;

	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			start = i + 1;
			break ;
		}
		i++;
	}
	len = var_length(str);
	var = ft_substr(str, start, len);
	if (!var)
		return (NULL);
	tmp = ft_strjoin(var, "=");
	free_ptr(var);
	var = tmp;
	return (var);
}

int	save_separator(t_token **token_lst, char *str, int index, int type)
{
	int		i;
	char	*sep;

	i = 0;
	if (type == APPEND || type == HEREDOC)
	{
		sep = malloc(sizeof(char) * 3);
		if (!sep)
			return (1);
		while (i < 2)
			sep[i++] = str[index++];
		sep[i] = '\0';
		lst_add_back_token(token_lst, lst_new_token(sep, NULL, type, DEFAULT));
	}
	else
	{
		sep = malloc(sizeof(char) * 2);
		if (!sep)
			return (1);
		while (i < 1)
			sep[i++] = str[index++];
		sep[i] = '\0';
		lst_add_back_token(token_lst, lst_new_token(sep, NULL, type, DEFAULT));
	}
	return (0);
}

int	save_word(t_token **token_lst, char *str, int index, int start)
{
	int		i;
	char	*word;

	i = 0;
	word = malloc(sizeof(char) * (index - start + 1));
	if (!word)
		return (1);
	while (start < index)
	{
		word[i] = str[start];
		start++;
		i++;
	}
	word[i] = '\0';
	lst_add_back_token(token_lst, \
			lst_new_token(word, ft_strdup(word), WORD, DEFAULT));
	return (0);
}

int	is_separator(char *str, int i)
{
	if (((str[i] > 8 && str[i] < 14) || str[i] == 32))
		return (SPACES);
	else if (str[i] == '|')
		return (PIPE);
	else if (str[i] == '<' && str[i + 1] == '<')
		return (HEREDOC);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (APPEND);
	else if (str[i] == '<')
		return (INPUT);
	else if (str[i] == '>')
		return (TRUNC);
	else if (str[i] == '\0')
		return (END);
	else
		return (0);
}

int	set_status(int status, char *str, int i)
{
	if (str[i] == '\'' && status == DEFAULT)
		status = SQUOTE;
	else if (str[i] == '\"' && status == DEFAULT)
		status = DQUOTE;
	else if (str[i] == '\'' && status == SQUOTE)
		status = DEFAULT;
	else if (str[i] == '\"' && status == DQUOTE)
		status = DEFAULT;
	return (status);
}

int	save_word_or_sep(int *i, char *str, int start, t_data *data)
{
	int	type;

	type = is_separator(str, (*i));
	if (type)
	{
		if ((*i) != 0 && is_separator(str, (*i) - 1) == 0)
			save_word(&data->token, str, (*i), start);
		if (type == APPEND || type == HEREDOC || type == PIPE
			|| type == INPUT || type == TRUNC || type == END)
		{
			save_separator(&data->token, str, (*i), type);
			if (type == APPEND || type == HEREDOC)
				(*i)++;
		}
		start = (*i) + 1;
	}
	return (start);
}

int	tokenization(t_data *data, char *str)
{
	int	i;
	int	end;
	int	start;
	int	status;

	i = -1;
	start = 0;
	end = ft_strlen(str);
	status = DEFAULT;
	while (++i <= end)
	{
		status = set_status(status, str, i);
		if (status == DEFAULT)
			start = save_word_or_sep(&i, str, start, data);
	}
	if (status != DEFAULT)
	{
		if (status == DQUOTE)
			error_message("unexpected EOF while looking for matching", "\"", true);
		else if (status == SQUOTE)
			error_message("unexpected EOF while looking for matching", "\'", true);
		error_message("syntax error", "unexpected end of file", false);
		return (1);
	}
	return (0);
}

static void	link_extremities(t_token *to_del, t_token *temp, t_token *insert)
{
	while (temp != to_del)
		temp = temp->next;
	insert->prev = temp->prev;
	temp->prev->next = insert;
	while (insert->next)
		insert = insert->next;
	temp->next->prev = insert;
	insert->next = temp->next;
}

t_token	*insert_lst_between(t_token **head, t_token *to_del, t_token *insert)
{
	t_token	*temp;

	temp = *head;
	if (temp == NULL)
		*head = insert;
	else if (temp == to_del)
	{
		*head = insert;
		insert->next = temp->next;
		if (temp->next != NULL)
			temp->next->prev = insert;
	}
	else
		link_extremities(to_del, temp, insert);
	free_ptr(to_del->str);
	free_ptr(to_del);
	return (insert);
}

t_token	*lst_new_token(char *str, char *str_backup, int type, int status)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token) * 1);
	if (!(new_node))
		return (NULL);
	new_node->str = str;
	new_node->str_backup = str_backup;
	new_node->var_exists = false;
	new_node->type = type;
	new_node->status = status;
	new_node->join = false;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	lst_add_back_token(t_token **alst, t_token *new_node)
{
	t_token	*start;

	start = *alst;
	if (start == NULL)
	{
		*alst = new_node;
		return ;
	}
	if (alst && *alst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
}

void	lstdelone_token(t_token *lst, void (*del)(void *))
{
	if (del && lst && lst->str)
	{
		(*del)(lst->str);
		lst->str = NULL;
	}
	if (del && lst && lst->str_backup)
	{
		(*del)(lst->str_backup);
		lst->str_backup = NULL;
	}
	if (lst->prev)
		lst->prev->next = lst->next;
	if (lst->next)
		lst->next->prev = lst->prev;
	free_ptr(lst);
}

void	lstclear_token(t_token **lst, void (*del)(void *))
{
	t_token	*tmp;

	tmp = NULL;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		lstdelone_token(*lst, del);
		*lst = tmp;
	}
}

static bool	input_is_space(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!is_whitespace(input[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	parse_user_input(t_data *data)
{
	if (data->user_input == NULL)
		exit_builtin(data, NULL);
	else if (ft_strcmp(data->user_input, "\0") == 0)
		return (false);
	else if (input_is_space(data->user_input))
		return (true);
	add_history(data->user_input);
	if (tokenization(data, data->user_input) == FAILURE)
		return (false);
	if (data->token->type == END)
		return (false);
	if (check_if_var(&data->token) == FAILURE)
		return (false);
	var_expander(data, &data->token);
	handle_quotes(data);
	create_commands(data, data->token);
	return (true);
}

static bool	consecutive_ops(t_token *token_node)
{
	if (token_node->prev)
	{
		if (token_node->type == PIPE && token_node->prev->type == PIPE)
			return (true);
		if (token_node->type > PIPE && token_node->prev->type > PIPE)
			return (true);
		if (token_node->type == END && token_node->prev->type >= PIPE)
			return (true);
	}
	return (false);
}

int	check_consecutives(t_token **token_lst)
{
	t_token	*temp;

	temp = *token_lst;
	while (temp)
	{
		if (consecutive_ops(temp) == true)
		{
			if (temp->type == END && temp->prev && temp->prev->type > PIPE)
				error_message("syntax error near unexpected token", "newline", true);
			else if (temp->type == END && temp->prev)
				error_message("syntax error near unexpected token",
					temp->prev->str, true);
			else
				error_message("syntax error near unexpected token", temp->str, true);
			return (FAILURE);
		}
		temp = temp->next;
	}
	return (SUCCESS);
}

static void	variable_check(t_token **token_node)
{
	int	i;

	i = 0;
	while ((*token_node)->str[i])
	{
		if ((*token_node)->str[i] == '$')
		{
			if ((*token_node)->prev && (*token_node)->prev->type == HEREDOC)
				break ;
			(*token_node)->type = VAR;
			return ;
		}
		i++;
	}
}

int	check_if_var(t_token **token_lst)
{
	t_token	*temp;

	temp = *token_lst;
	if (temp->type == PIPE)
	{
		error_message("syntax error near unexpected token", temp->str, true);
		return (FAILURE);
	}
	while (temp)
	{
		variable_check(&temp);
		if (check_consecutives(&temp) == FAILURE)
			return (FAILURE);
		temp = temp->next;
	}
	return (SUCCESS);
}

static bool	contains_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			return (true);
		i++;
	}
	return (false);
}

static void	split_var_cmd_token(t_command *last_cmd, char *cmd_str)
{
	t_token		*new_tokens;
	t_token		*tmp;
	char		**strs;
	int			i;

	new_tokens = NULL;
	strs = ft_split(cmd_str, ' ');
	if (!strs)
		return ;
	last_cmd->command = ft_strdup(strs[0]);
	if (strs[1])
		new_tokens = lst_new_token(ft_strdup(strs[1]), NULL, WORD, DEFAULT);
	tmp = new_tokens;
	i = 1;
	while (strs[++i])
		lst_add_back_token(&new_tokens,
			lst_new_token(ft_strdup(strs[i]), NULL, WORD, DEFAULT));
	lst_add_back_token(&new_tokens,
		lst_new_token(NULL, NULL, END, DEFAULT));
	fill_args(&new_tokens, last_cmd);
	lstclear_token(&tmp, &free_ptr);
	free_array(strs);
}

void	parse_word(t_command **cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*last_cmd;

	temp = *token_lst;
	while (temp->type == WORD || temp->type == VAR)
	{
		last_cmd = lst_last_cmd(*cmd);
		if (temp->prev == NULL || (temp->prev && temp->prev->type == PIPE)
			|| last_cmd->command == NULL)
		{
			if (temp->type == VAR && contains_space(temp->str))
				split_var_cmd_token(last_cmd, temp->str);
			else
				last_cmd->command = ft_strdup(temp->str);
			temp = temp->next;
		}
		else
			fill_args(&temp, last_cmd);
	}
	*token_lst = temp;
}

char	*get_relative_path(char *file_to_open)
{
	char	*path;
	char	*ret;

	if (file_to_open[0] == '/')
		return (ft_strdup(file_to_open));
	path = ft_strdup("./");
	ret = ft_strjoin(path, file_to_open);
	printf("PARSING - Get_rel_path function return : %s\n", ret);
	return (ret);
}

static void	open_outfile_trunc(t_redirection *io, char *file, char *var_filename)
{
	if (!remove_old_file_ref(io, false))
		return ;
	io->outfile = ft_strdup(file);
	if (io->outfile && io->outfile[0] == '\0')
	{
		error_command(var_filename, NULL, "ambiguous redirect", false);
		return ;
	}
	io->fd_out = open(io->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (io->fd_out == -1)
		error_command(io->outfile, NULL, strerror(errno), false);
}

void	parse_trunc(t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	initialize_redirection(cmd);
	open_outfile_trunc(cmd->io_fds, temp->next->str, temp->next->str_backup);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}

void	parse_pipe(t_command **cmd, t_token **token_lst)
{
	t_command	*last_cmd;

	last_cmd = lst_last_cmd(*cmd);
	last_cmd->pipe_output = true;
	lst_add_back_cmd(&last_cmd, lst_new_cmd(false));
	*token_lst = (*token_lst)->next;
}

bool	remove_old_file_ref(t_redirection *io, bool infile)
{
	if (infile == true && io->infile)
	{
		if (io->fd_in == -1 || (io->outfile && io->fd_out == -1))
			return (false);
		if (io->heredoc_delimiter != NULL)
		{
			free_ptr(io->heredoc_delimiter);
			io->heredoc_delimiter = NULL;
			unlink(io->infile);
		}
		free_ptr(io->infile);
		close(io->fd_in);
	}
	else if (infile == false && io->outfile)
	{
		if (io->fd_out == -1 || (io->infile && io->fd_in == -1))
			return (false);
		free_ptr(io->outfile);
		close(io->fd_out);
	}
	return (true);
}

static void	open_infile(t_redirection *io, char *file, char *original_filename)
{
	if (!remove_old_file_ref(io, true))
		return ;
	io->infile = ft_strdup(file);
	if (io->infile && io->infile[0] == '\0')
	{
		error_command(original_filename, NULL, "ambiguous redirect", false);
		return ;
	}
	io->fd_in = open(io->infile, O_RDONLY);
	if (io->fd_in == -1)
		error_command(io->infile, NULL, strerror(errno), false);
}

void	parse_input(t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	initialize_redirection(cmd);
	open_infile(cmd->io_fds, temp->next->str, temp->next->str_backup);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}

static char	*make_str_from_tab(char **tab)
{
	char	*str;
	char	*tmp;
	int		i;

	i = -1;
	while (tab[++i])
	{
		tmp = str;
		if (i == 0)
			str = ft_strdup(tab[0]);
		else
		{
			str = ft_strjoin(tmp, tab[i]);
			free_ptr(tmp);
		}
		if (tab[i + 1])
		{
			tmp = str;
			str = ft_strjoin(tmp, " ");
			free_ptr(tmp);
		}
	}
	free_array(tab);
	return (str);
}

static char	*get_expanded_var_line(t_data *data, char *line)
{
	char	**words;
	int		i;

	words = ft_split(line, ' ');
	if (!words)
		return (NULL);
	i = 0;
	while (words[i])
	{
		if (ft_strchr(words[i], '$'))
		{
			words[i] = var_expander_heredoc(data, words[i]);
			if (!words[i])
				return (NULL);
		}
		i++;
	}
	return (make_str_from_tab(words));
}

static bool	evaluate_heredoc_line(t_data *data, char **line,
									t_redirection *io, bool *ret)
{
	if (*line == NULL)
	{
		error_command("warning", "here-document delimited by end-of-file: wanted",
			io->heredoc_delimiter, true);
		*ret = true;
		return (false);
	}
	if (ft_strcmp(*line, io->heredoc_delimiter) == 0)
	{
		*ret = true;
		return (false);
	}
	if (io->heredoc_quotes == false && ft_strchr(*line, '$'))
	{
		*line = get_expanded_var_line(data, *line);
		if (!(*line))
		{
			free_ptr(*line);
			*ret = false;
			return (false);
		}
	}
	return (true);
}

bool	fill_heredoc(t_data *data, t_redirection *io, int fd)
{
	char	*line;
	bool	ret;

	ret = false;
	line = NULL;
	while (1)
	{
		set_signals_interactive();
		line = readline(">");
		set_signals_noninteractive();
		if (!evaluate_heredoc_line(data, &line, io, &ret))
			break ;
		ft_putendl_fd(line, fd);
		free_ptr(line);
	}
	free_ptr(line);
	return (ret);
}

bool	get_heredoc(t_data *data, t_redirection *io)
{
	int		tmp_fd;
	bool	ret;

	ret = true;
	tmp_fd = open(io->infile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	ret = fill_heredoc(data, io, tmp_fd);
	close(tmp_fd);
	return (ret);
}

static char	*get_heredoc_name(void)
{
	static int	i;
	char		*name;
	char		*number;

	number = ft_itoa(i);
	if (!number)
		return (NULL);
	name = ft_strjoin(HEREDOC_NAME, number);
	free(number);
	i++;
	return (name);
}

static char	*get_delim(char *delim, bool *quotes)
{
	int	len;

	len = ft_strlen(delim) - 1;
	if ((delim[0] == '\"' && delim[len] == '\"')
		|| (delim[0] == '\'' && delim[len] == '\''))
	{
		*quotes = true;
		return (ft_strtrim(delim, "\'\""));
	}
	return (ft_strdup(delim));
}

void	parse_heredoc(t_data *data, t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;
	t_redirection	*io;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	initialize_redirection(cmd);
	io = cmd->io_fds;
	if (!remove_old_file_ref(io, true))
		return ;
	io->infile = get_heredoc_name();
	io->heredoc_delimiter = get_delim(temp->next->str, &(io->heredoc_quotes));
	if (get_heredoc(data, io))
		io->fd_in = open(io->infile, O_RDONLY);
	else
		io->fd_in = -1;
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}

static void	open_outfile_append(t_redirection *io, char *file, char *var_filename)
{
	if (!remove_old_file_ref(io, false))
		return ;
	io->outfile = ft_strdup(file);
	if (io->outfile && io->outfile[0] == '\0' && var_filename)
	{
		error_command(var_filename, NULL, "ambiguous redirect", false);
		return ;
	}
	io->fd_out = open(io->outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (io->fd_out == -1)
		error_command(io->outfile, NULL, strerror(errno), false);
}

void	parse_append(t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	initialize_redirection(cmd);
	open_outfile_append(cmd->io_fds, temp->next->str, temp->next->str_backup);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}

char	*join_vars(t_token **token_node)
{
	t_token	*temp;
	char	*str;

	temp = *token_node;
	str = ft_strdup(temp->str);
	while (temp->type == VAR && temp->next->type == VAR
		&& temp->next->join == true)
	{
		str = ft_strjoin(str, temp->next->str);
		temp = temp->next;
	}
	*token_node = temp;
	return (str);
}

int	count_args(t_token *temp)
{
	int	i;

	i = 0;
	while (temp && (temp->type == WORD || temp->type == VAR))
	{
		if (temp->type == VAR && temp->join == true)
		{
			while (temp->type == VAR && temp->join == true)
				temp = temp->next;
			i++;
		}
		else
		{
			i++;
			temp = temp->next;
		}
	}
	return (i);
}

char	**copy_in_new_tab(int len, char **new_tab,
			t_command *last_cmd, t_token *tmp)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new_tab[i] = last_cmd->args[i];
		i++;
	}
	while (tmp->type == WORD || tmp->type == VAR)
	{
		if (tmp->join == true)
			new_tab[i] = join_vars(&tmp);
		else
			new_tab[i] = ft_strdup(tmp->str);
		i++;
		tmp = tmp->next;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

void	remove_empty_var_args(t_token **tokens)
{
	t_token	*temp;

	temp = *tokens;
	while (temp->type == WORD || temp->type == VAR)
	{
		if (temp->type == VAR && temp->str[0] == '\0'
			&& temp->var_exists == false)
		{
			temp = temp->next;
			if (temp == (*tokens)->next)
				(*tokens) = (*tokens)->next;
			lstdelone_token(temp->prev, free_ptr);
		}
		else
			temp = temp->next;
	}
}

int	create_args_echo_mode(t_token **token_node, t_command *last_cmd)
{
	int		nb_args;
	t_token	*temp;
	int		i;

	remove_empty_var_args(token_node);
	temp = *token_node;
	nb_args = count_args(temp);
	last_cmd->args = malloc(sizeof(char *) * (nb_args + 2));
	if (!last_cmd->args)
		return (FAILURE);
	i = 0;
	last_cmd->args[i] = ft_strdup(last_cmd->command);
	i++;
	while (temp->type == WORD || temp->type == VAR)
	{
		if (temp->join == true)
			last_cmd->args[i] = join_vars(&temp);
		else
			last_cmd->args[i] = ft_strdup(temp->str);
		i++;
		temp = temp->next;
	}
	last_cmd->args[i] = NULL;
	*token_node = temp;
	return (SUCCESS);
}

int	add_args_echo_mode(t_token **token_node, t_command *last_cmd)
{
	int		len;
	int		nb_args;
	char	**new_tab;
	t_token	*temp;

	remove_empty_var_args(token_node);
	temp = *token_node;
	nb_args = count_args(temp);
	len = 0;
	while (last_cmd->args[len])
		len++;
	new_tab = malloc(sizeof(char *) * (nb_args + len + 1));
	if (!new_tab)
		return (FAILURE);
	new_tab = copy_in_new_tab(len, new_tab, last_cmd, temp);
	free(last_cmd->args);
	last_cmd->args = new_tab;
	while (temp->type == WORD || temp->type == VAR)
		temp = temp->next;
	*token_node = temp;
	return (SUCCESS);
}

int	count_arguments(t_token *temp)
{
	int	i;

	i = 0;
	while (temp && (temp->type == WORD || temp->type == VAR))
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

int	create_args_default_mode(t_token **token_node, t_command *last_cmd)
{
	int		i;
	int		nb_args;
	t_token	*temp;

	i = 0;
	temp = *token_node;
	nb_args = count_arguments(temp);
	last_cmd->args = malloc(sizeof(char *) * (nb_args + 2));
	if (!last_cmd->args)
		return (FAILURE);
	temp = *token_node;
	i = 0;
	last_cmd->args[i] = ft_strdup(last_cmd->command);
	i++;
	while (temp->type == WORD || temp->type == VAR)
	{
		last_cmd->args[i] = ft_strdup(temp->str);
		i++;
		temp = temp->next;
	}
	last_cmd->args[i] = NULL;
	*token_node = temp;
	return (SUCCESS);
}

static char	**copy_default_in_new_tab(
	int len, char **new_tab, t_command *last_cmd, t_token **tk_node)
{
	int		i;
	t_token	*temp;

	i = 0;
	temp = *tk_node;
	while (i < len)
	{
		new_tab[i] = last_cmd->args[i];
		i++;
	}
	while (temp->type == WORD || temp->type == VAR)
	{
		new_tab[i] = ft_strdup(temp->str);
		i++;
		temp = temp->next;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

int	add_args_default_mode(t_token **token_node, t_command *last_cmd)
{
	int		i;
	int		len;
	char	**new_tab;
	t_token	*temp;

	i = 0;
	temp = *token_node;
	while (temp->type == WORD || temp->type == VAR)
	{
		i++;
		temp = temp->next;
	}
	len = 0;
	while (last_cmd->args[len])
		len++;
	new_tab = malloc(sizeof(char *) * (i + len + 1));
	if (!new_tab)
		return (FAILURE);
	new_tab = copy_default_in_new_tab(len, new_tab, last_cmd, token_node);
	free(last_cmd->args);
	last_cmd->args = new_tab;
	*token_node = temp;
	return (SUCCESS);
}

int	fill_args(t_token **token_node, t_command *last_cmd)
{
	if (!ft_strcmp(last_cmd->command, "echo"))
	{
		if (!(last_cmd->args))
			return (create_args_echo_mode(token_node, last_cmd));
		else
			return (add_args_echo_mode(token_node, last_cmd));
	}
	else
	{
		if (last_cmd && !(last_cmd->args))
			return (create_args_default_mode(token_node, last_cmd));
		else
			return (add_args_default_mode(token_node, last_cmd));
	}
	return (SUCCESS);
}

static void	prep_no_arg_commands(t_data *data)
{
	t_command	*cmd;

	if (!data || !data->cmd)
		return ;
	cmd = data->cmd;
	while (cmd && cmd->command)
	{
		if (!cmd->args)
		{
			cmd->args = malloc(sizeof * cmd->args * 2);
			cmd->args[0] = ft_strdup(cmd->command);
			cmd->args[1] = NULL;
		}
		cmd = cmd->next;
	}
	cmd = lst_last_cmd(data->cmd);
}

void	create_commands(t_data *data, t_token *token)
{
	t_token	*temp;

	temp = token;
	if (temp->type == END)
		return ;
	while (temp->next != NULL)
	{
		if (temp == token)
			lst_add_back_cmd(&data->cmd, lst_new_cmd(false));
		if (temp->type == WORD || temp->type == VAR)
			parse_word(&data->cmd, &temp);
		else if (temp->type == INPUT)
			parse_input(&data->cmd, &temp);
		else if (temp->type == TRUNC)
			parse_trunc(&data->cmd, &temp);
		else if (temp->type == HEREDOC)
			parse_heredoc(data, &data->cmd, &temp);
		else if (temp->type == APPEND)
			parse_append(&data->cmd, &temp);
		else if (temp->type == PIPE)
			parse_pipe(&data->cmd, &temp);
		else if (temp->type == END)
			break ;
	}
	prep_no_arg_commands(data);
}

void	lst_delone_cmd(t_command *lst, void (*del)(void *))
{
	if (lst->command)
		(*del)(lst->command);
	if (lst->args)
		free_array(lst->args);
	if (lst->pipe_fd)
		(*del)(lst->pipe_fd);
	if (lst->io_fds)
		free_redirection(lst->io_fds);
	(*del)(lst);
}

void	lst_clear_cmd(t_command **lst, void (*del)(void *))
{
	t_command	*temp;

	temp = NULL;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		lst_delone_cmd(*lst, del);
		*lst = temp;
	}
}

static void	initialize_cmd(t_command **cmd)
{
	(*cmd)->command = NULL;
	(*cmd)->path = NULL;
	(*cmd)->args = NULL;
	(*cmd)->pipe_output = false;
	(*cmd)->pipe_fd = 0;
	(*cmd)->prev = NULL;
	(*cmd)->next = NULL;
}

t_command	*lst_new_cmd(bool value)
{
	t_command	*new_node;

	new_node = (t_command *)malloc(sizeof(t_command));
	if (!(new_node))
		return (NULL);
	ft_memset(new_node, 0, sizeof(t_command));
	new_node->pipe_output = value;
	initialize_cmd(&new_node);
	return (new_node);
}

void	lst_add_back_cmd(t_command **alst, t_command *new_node)
{
	t_command	*start;

	start = *alst;
	if (start == NULL)
	{
		*alst = new_node;
		return ;
	}
	if (alst && *alst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
}

t_command	*lst_last_cmd(t_command *cmd)
{
	while (cmd->next != NULL)
		cmd = cmd->next;
	return (cmd);
}

void	close_pipe_fds(t_command *cmds, t_command *skip_cmd)
{
	while (cmds)
	{
		if (cmds != skip_cmd && cmds->pipe_fd)
		{
			close(cmds->pipe_fd[0]);
			close(cmds->pipe_fd[1]);
		}
		cmds = cmds->next;
	}
}

bool	create_pipes(t_data *data)
{
	int			*fd;
	t_command	*tmp;

	tmp = data->cmd;
	while (tmp)
	{
		if (tmp->pipe_output || (tmp->prev && tmp->prev->pipe_output))
		{
			fd = malloc(sizeof * fd * 2);
			if (!fd || pipe(fd) != 0)
			{
				free_data(data, false);
				return (false);
			}
			tmp->pipe_fd = fd;
		}
		tmp = tmp->next;
	}
	return (true);
}

bool	set_pipe_fds(t_command *cmds, t_command *c)
{
	if (!c)
		return (false);
	if (c->prev && c->prev->pipe_output)
		dup2(c->prev->pipe_fd[0], STDIN_FILENO);
	if (c->pipe_output)
		dup2(c->pipe_fd[1], STDOUT_FILENO);
	close_pipe_fds(cmds, c);
	return (true);
}

bool	restore_io(t_redirection *io)
{
	int	ret;

	ret = true;
	if (!io)
		return (ret);
	if (io->stdin_backup != -1)
	{
		if (dup2(io->stdin_backup, STDIN_FILENO) == -1)
			ret = false;
		close(io->stdin_backup);
		io->stdin_backup = -1;
	}
	if (io->stdout_backup != -1)
	{
		if (dup2(io->stdout_backup, STDOUT_FILENO) == -1)
			ret = false;
		close(io->stdout_backup);
		io->stdout_backup = -1;
	}
	return (ret);
}

bool	redirect_io(t_redirection *io)
{
	int	ret;

	ret = true;
	if (!io)
		return (ret);
	io->stdin_backup = dup(STDIN_FILENO);
	if (io->stdin_backup == -1)
		ret = error_command("dup", "stdin backup", strerror(errno), false);
	io->stdout_backup = dup(STDOUT_FILENO);
	if (io->stdout_backup == -1)
		ret = error_command("dup", "stdout backup", strerror(errno), false);
	if (io->fd_in != -1)
		if (dup2(io->fd_in, STDIN_FILENO) == -1)
			ret = error_command("dup2", io->infile, strerror(errno), false);
	if (io->fd_out != -1)
		if (dup2(io->fd_out, STDOUT_FILENO) == -1)
			ret = error_command("dup2", io->outfile, strerror(errno), false);
	return (ret);
}

bool	check_infile_outfile(t_redirection *io)
{
	if (!io || (!io->infile && !io->outfile))
		return (true);
	if ((io->infile && io->fd_in == -1)
		|| (io->outfile && io->fd_out == -1))
		return (false);
	return (true);
}

void	signal_reset_prompt(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_signals_interactive(void)
{
	struct sigaction	act;

	ignore_sigquit();
	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_reset_prompt;
	sigaction(SIGINT, &act, NULL);
}

void	signal_print_newline(int signal)
{
	(void)signal;
	rl_on_new_line();
}

void	set_signals_noninteractive(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_print_newline;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}

void	ignore_sigquit(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}

static bool	init_env(t_data *data, char **env)
{
	int		i;

	data->env = ft_calloc(env_var_count(env) + 1, sizeof * data->env);
	if (!data->env)
		return (false);
	i = 0;
	while (env[i])
	{
		data->env[i] = ft_strdup(env[i]);
		if (!data->env[i])
			return (false);
		i++;
	}
	return (true);
}

static bool	init_wds(t_data *data)
{
	char	buff[PATH_MAX];
	char	*wd;

	wd = getcwd(buff, PATH_MAX);
	data->working_directory = ft_strdup(wd);
	if (!data->working_directory)
		return (false);
	if (get_env_var_index(data->env, "OLDPWD") != -1)
	{
		data->old_working_directory = ft_strdup(get_env_var_value(data->env,
					"OLDPWD"));
		if (!data->old_working_directory)
			return (false);
	}
	else
	{
		data->old_working_directory = ft_strdup(wd);
		if (!data->old_working_directory)
			return (false);
	}
	return (true);
}

bool	initialize_data(t_data *data, char **env)
{
	if (!init_env(data, env))
	{
		error_command("Fatal", NULL, "Could not initialize environment", 1);
		return (false);
	}
	if (!init_wds(data))
	{
		error_command("Fatal", NULL, "Could not initialize working directories",
			1);
		return (false);
	}
	data->token = NULL;
	data->user_input = NULL;
	data->cmd = NULL;
	data->pid = -1;
	g_last_exit_code = 0;
	return (true);
}

void	initialize_redirection(t_command *cmd)
{
	if (!cmd->io_fds)
	{
		cmd->io_fds = malloc(sizeof * cmd->io_fds);
		if (!cmd->io_fds)
			return ;
		cmd->io_fds->infile = NULL;
		cmd->io_fds->outfile = NULL;
		cmd->io_fds->heredoc_delimiter = NULL;
		cmd->io_fds->heredoc_quotes = false;
		cmd->io_fds->fd_in = -1;
		cmd->io_fds->fd_out = -1;
		cmd->io_fds->stdin_backup = -1;
		cmd->io_fds->stdout_backup = -1;
	}
}

void	exit_shell(t_data *data, int exno)
{
	if (data)
	{
		if (data->cmd && data->cmd->io_fds)
			close_fds(data->cmd, true);
		free_data(data, true);
	}
	exit(exno);
}

char	*join_strs(char *str, char *add)
{
	char	*tmp;

	if (!add)
		return (str);
	if (!str)
		return (ft_strdup(add));
	tmp = str;
	str = ft_strjoin(tmp, add);
	free_ptr(tmp);
	return (str);
}

static bool	add_detail_quotes(char *command)
{
	if (ft_strncmp(command, "export", 7) == 0
		|| ft_strncmp(command, "unset", 6) == 0)
		return (true);
	return (false);
}

int	error_command(char *command, char *detail, char *error_message, int error_nb)
{
	char	*msg;
	bool	detail_quotes;

	detail_quotes = add_detail_quotes(command);
	msg = ft_strdup("minishell: ");
	if (command != NULL)
	{
		msg = join_strs(msg, command);
		msg = join_strs(msg, ": ");
	}
	if (detail != NULL)
	{
		if (detail_quotes)
			msg = join_strs(msg, "`");
		msg = join_strs(msg, detail);
		if (detail_quotes)
			msg = join_strs(msg, "'");
		msg = join_strs(msg, ": ");
	}
	msg = join_strs(msg, error_message);
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
	return (error_nb);
}

void	error_message(char *error_message, char *detail, int quotes)
{
	char	*msg;

	msg = ft_strdup("minishell: ");
	msg = join_strs(msg, error_message);
	if (quotes)
		msg = join_strs(msg, " `");
	else
		msg = join_strs(msg, ": ");
	msg = join_strs(msg, detail);
	if (quotes)
		msg = join_strs(msg, "'");
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
}

bool	usage_message(bool return_val)
{
	ft_putendl_fd("Usage: ./minishell", 2);
	ft_putendl_fd("Usage: ./minishell -c \"input line\"", 2);
	return (return_val);
}

void	free_data(t_data *data, bool clear_history)
{
	if (data && data->user_input)
	{
		free_ptr(data->user_input);
		data->user_input = NULL;
	}
	if (data && data->token)
		lstclear_token(&data->token, &free_ptr);
	if (data && data->cmd)
		lst_clear_cmd(&data->cmd, &free_ptr);
	if (clear_history == true)
	{
		if (data && data->working_directory)
			free_ptr(data->working_directory);
		if (data && data->old_working_directory)
			free_ptr(data->old_working_directory);
		if (data && data->env)
			free_array(data->env);
		rl_clear_history();
	}
}

void	close_fds(t_command *cmds, bool close_backups)
{
	if (cmds->io_fds)
	{
		if (cmds->io_fds->fd_in != -1)
			close(cmds->io_fds->fd_in);
		if (cmds->io_fds->fd_out != -1)
			close(cmds->io_fds->fd_out);
		if (close_backups)
			restore_io(cmds->io_fds);
	}
	close_pipe_fds(cmds, NULL);
}

void	free_redirection(t_redirection *io)
{
	if (!io)
		return ;
	restore_io(io);
	if (io->heredoc_delimiter)
	{
		unlink(io->infile);
		free_ptr(io->heredoc_delimiter);
	}
	if (io->infile)
		free_ptr(io->infile);
	if (io->outfile)
		free_ptr(io->outfile);
	if (io)
		free_ptr(io);
}

void	free_array(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			if (tab[i])
			{
				free_ptr(tab[i]);
				tab[i] = NULL;
			}
			i++;
		}
		free(tab);
		tab = NULL;
	}
}

void	free_ptr(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

static bool	start_check(t_data *data, int ac, char **av)
{
	if (ac != 1 && ac != 3)
		return (usage_message(false));
	if (ac == 3)
	{
		data->interactive = false;
		if (!av[1] || (av[1] && ft_strcmp(av[1], "-c") != 0))
			return (usage_message(false));
		else if (!av[2] || (av[2] && av[2][0] == '\0'))
			return (usage_message(false));
	}
	else
		data->interactive = true;
	return (true);
}

void	minishell_interactive(t_data *data)
{
	while (1)
	{
		set_signals_interactive();
		data->user_input = readline(PROMPT);
		set_signals_noninteractive();
		if (parse_user_input(data) == true)
			g_last_exit_code = execute(data);
		else
			g_last_exit_code = 1;
		free_data(data, false);
	}
}

void	minishell_noninteractive(t_data *data, char *arg)
{
	char	**user_inputs;
	int		i;

	user_inputs = ft_split(arg, ';');
	if (!user_inputs)
		exit_shell(data, EXIT_FAILURE);
	i = 0;
	while (user_inputs[i])
	{
		data->user_input = ft_strdup(user_inputs[i]);
		if (parse_user_input(data) == true)
			g_last_exit_code = execute(data);
		else
			g_last_exit_code = 1;
		i++;
		free_data(data, false);
	}
	free_array(user_inputs);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	ft_memset(&data, 0, sizeof(t_data));
	if (!start_check(&data, ac, av) || !initialize_data(&data, env))
		exit_shell(NULL, EXIT_FAILURE);
	if (data.interactive)
		minishell_interactive(&data);
	else
		minishell_noninteractive(&data, av[2]);
	exit_shell(&data, g_last_exit_code);
	return (0);
}
