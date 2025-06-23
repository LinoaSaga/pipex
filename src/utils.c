/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljudd <ljudd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:51:20 by ljudd             #+#    #+#             */
/*   Updated: 2025/06/23 10:24:03 by ljudd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* get_path :
	Function to create the array of the different paths of the commands
	located in the environment
*/
void	get_path(t_pipex *p, char **envp)
{
	int	k;

	if (!envp)
		p->path = ft_split(" ", ' ');
	else
	{
		k = -1;
		while (envp[++k])
		{
			if (ft_strncmp(envp[k], "PATH=", 5) == 0)
				p->path = ft_split((envp[k] + 5), ':');
		}
		if (!p->path)
			p->path = ft_split(" ", ' ');
	}
	if (!p->path)
		exit_pipex(p, "error while searching for global variable PATH\n",
			NULL, 1);
	return ;
}

/* find_cmd :
	for the command i, search the command path relative to that command
	If the command is not found, simply leave if as NULL
	In case of malloc error, stop the program
*/
void	find_cmd(t_pipex *p, int i)
{
	char	*path;
	char	*full_path;
	int		k;

	k = -1;
	path = NULL;
	while (p->path[++k])
	{
		path = ft_strjoin(p->path[k], "/");
		if (!path)
			exit_pipex(p, "error while parsing command arguments\n", NULL, 1);
		full_path = ft_strjoin(path, p->cmds[i].cmd_argv[0]);
		free(path);
		if (!full_path)
			exit_pipex(p, "error while parsing command arguments\n", NULL, 1);
		if (access(full_path, F_OK) == 0)
		{
			p->cmds[i].cmd_path = full_path;
			return ;
		}
		free(full_path);
	}
}

/* get_cmds :
	stock the different commands arguments and also get each command path
*/
void	get_cmds(char **argv, t_pipex *p)
{
	int	i;

	i = -1;
	while (++i < p->n)
	{
		p->cmds[i].cmd_argv = ft_split(argv[i + 2], ' ');
		if (!p->cmds[i].cmd_argv || !p->cmds[i].cmd_argv[0])
		{
			if (p->cmds[i].cmd_argv)
				free(p->cmds[i].cmd_argv);
			p->cmds[i].cmd_argv = ft_split(argv[i + 2], 0);
		}
		if (!p->cmds[i].cmd_argv || !p->cmds[i].cmd_argv[0])
			exit_pipex(p, "error while parsing command arguments\n", NULL, 1);
		if (ft_strncmp(p->cmds[i].cmd_argv[0], "/", 1) == 0)
			p->cmds[i].cmd_path = ft_strdup(p->cmds[i].cmd_argv[0]);
		else if (ft_strstr(p->cmds[i].cmd_argv[0], "./")
			|| ft_strstr(p->cmds[i].cmd_argv[0], "../"))
			p->cmds[i].cmd_path = ft_strdup(p->cmds[i].cmd_argv[0]);
		else
			find_cmd(p, i);
	}
}
