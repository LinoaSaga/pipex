/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljudd <ljudd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:49:17 by ljudd             #+#    #+#             */
/*   Updated: 2025/06/21 15:53:26 by ljudd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* clean_path :
	clean the path if not NULL
*/
void	clean_path(t_pipex *p)
{
	int	k;

	if (p->path)
	{
		k = -1;
		while (p->path[++k])
			free(p->path[k]);
		free(p->path);
	}	
}

/* clean_cmds :
	clean the cmds if not NULL
*/
void	clean_cmds(t_pipex *p)
{
	int	k;
	int	j;

	if (p->cmds)
	{
		k = -1;
		while (++k < p->n)
		{
			if (p->cmds[k].cmd_path)
				free(p->cmds[k].cmd_path);
			if (p->cmds[k].cmd_argv)
			{
				j = -1;
				while (p->cmds[k].cmd_argv[++j])
					free(p->cmds[k].cmd_argv[j]);
				free(p->cmds[k].cmd_argv);
			}
		}
		free(p->cmds);
	}
}

/* close_fds :
	close all the eventuals fd that may still be left opened
*/
void	close_fds(t_pipex *p)
{
	int	k;

	if (p->cmds)
	{
		k = -1;
		while (++k < p->n)
		{
			if (p->cmds[k].in_fd > -1)
				close(p->cmds[k].in_fd);
			if (p->cmds[k].out_fd > -1)
				close(p->cmds[k].out_fd);
		}
	}
}

/* close_fd :
	close a single fd in and out
*/
void	close_fd(t_pipex *p, int k)
{
	if (p->cmds[k].in_fd > -1)
	{
		close(p->cmds[k].in_fd);
		p->cmds[k].in_fd = -1;
	}
	if (p->cmds[k].out_fd > -1)
	{
		close(p->cmds[k].out_fd);
		p->cmds[k].out_fd = -1;
	}
}

/* exit_pipex : 
	leave the program, cleaning all the memory use, closing the fds
	and displaying eventually an error message
*/
void	exit_pipex(t_pipex *p, char *s1, char *s2, int returncode)
{
	if (s1)
	{
		ft_putstr_fd("pipex: ", 2);
		if (s2)
			ft_dprintf(2, s1, s2);
		else
			ft_putstr_fd(s1, 2);
	}
	clean_path(p);
	close_fds(p);
	clean_cmds(p);
	exit(returncode);
}
