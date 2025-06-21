/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljudd <ljudd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 18:00:38 by ljudd             #+#    #+#             */
/*   Updated: 2025/06/21 16:06:38 by ljudd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	p;
	int		k;

	init_p(argc, &p, true);
	get_path(&p, envp);
	get_cmds(argv, &p);
	create_pipes(argv, &p);
	k = -1;
	while (++k < p.n)
	{
		p.cmds[k].pid = fork();
		if (p.cmds[k].pid == -1)
			exit_pipex(&p, "error while forking\n", NULL, 1);
		if (p.cmds[k].pid == 0)
			exec_child(&p, k, argv, envp);
		else
			close_fd(&p, k);
	}
	k = -1;
	while (++k < p.n)
		waitpid(p.cmds[k].pid, &p.status, 0);
	if (WIFEXITED(p.status))
		exit_pipex(&p, NULL, NULL, WEXITSTATUS(p.status));
	exit_pipex(&p, "unexpected error", NULL, 0);
}
