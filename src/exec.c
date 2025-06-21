/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljudd <ljudd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:52:06 by ljudd             #+#    #+#             */
/*   Updated: 2025/06/21 16:05:05 by ljudd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* create_pipes :
	create pipes to link the different input / output of the commands
	first command takes input from the read file and last command output
	is the write file
	if error while trying to access the in or out files, leave the fd to -1
	error message is handled before execution has in shell
*/
void	create_pipes(char **argv, t_pipex *p)
{
	int	k;
	int	pipefd[2];

	k = -1;
	while (++k < p->n - 1)
	{
		if (pipe(pipefd) == -1)
			exit_pipex(p, "error while creating pipes", NULL, 1);
		p->cmds[k].out_fd = pipefd[WRITE_END];
		p->cmds[k + 1].in_fd = pipefd[READ_END];
	}
	p->cmds[0].in_fd = open(argv[1], O_RDONLY, 0644);
	p->cmds[p->n - 1].out_fd = open(argv[p->n + 2],
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
}

/* init_p :
	function to initialize the different arguments of p
*/
void	init_p(int argc, t_pipex *p, bool bonus)
{
	int	k;

	p->cmds = NULL;
	p->path = NULL;
	p->n = argc - 3;
	if (bonus && p->n < 2)
		exit_pipex(p, "invalid number of arguments\n", NULL, 1);
	else if (!bonus && p->n != 2)
		exit_pipex(p, "invalid number of arguments\n", NULL, 1);
	p->cmds = ft_calloc(p->n, sizeof(t_cmd));
	if (!p->cmds)
		exit_pipex(p, "error while malloc commands array\n", NULL, 1);
	k = -1;
	while (++k < p->n)
	{
		p->cmds[k].in_fd = -1;
		p->cmds[k].out_fd = -1;
	}
}

/* exec_child :
	make some checks then execute the child process
*/
void	exec_child(t_pipex *p, int k, char **argv, char **envp)
{
	if (p->cmds[k].in_fd == -1 && k == 0)
		exit_pipex(p, "no such file or directory: %s\n", argv[1], 1);
	if (p->cmds[k].out_fd == -1 && k == p->n - 1)
		exit_pipex(p, "no such file or directory: %s\n",
			argv[p->n + 2], 1);
	if (dup2(p->cmds[k].in_fd, STDIN_FILENO) == -1)
		exit_pipex(p, "error while duplicating fd\n", NULL, 1);
	if (dup2(p->cmds[k].out_fd, STDOUT_FILENO) == -1)
		exit_pipex(p, "error while duplicating fd\n", NULL, 1);
	close_fds(p);
	if (!p->cmds[k].cmd_path)
		exit_pipex(p, "command not found: %s\n",
			p->cmds[k].cmd_argv[0], 127);
	if (access(p->cmds[k].cmd_path, F_OK) != 0)
		exit_pipex(p, "no such file or directory: %s\n",
			p->cmds[k].cmd_argv[0], 127);
	execve(p->cmds[k].cmd_path, p->cmds[k].cmd_argv, envp);
	exit_pipex(p, "unexpected error: %s\n", p->cmds[k].cmd_argv[0], 1);
}
