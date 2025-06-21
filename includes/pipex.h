/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljudd <ljudd@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 09:24:35 by ljudd             #+#    #+#             */
/*   Updated: 2025/06/21 17:23:59 by ljudd            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H

# define PIPEX_H

# include "libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h> 

# define READ_END 0
# define WRITE_END 1

/* t_cmd : 
	contains the different infos to succesfully launch each command
	in_fd : file descriptor of the input
	out_fd : file descriptor of the output
	cmd_path : path of the command to execute
	argv : arguments of the command to execute
	pid : pid associated to the child process of the command while forking
*/
typedef struct s_cmd
{
	int		in_fd;
	int		out_fd;
	char	*cmd_path;
	char	**cmd_argv;
	pid_t	pid;
}			t_cmd;

/* t_pipex : 
	main type containing the different data, used to assure continuity 
	while development in term of arguments passed to functions
	path : path of the command for the environment
	cmds : commands to do and their infos, see belox
	n : number of commands
	status : get the status 
*/
typedef struct s_pipex
{
	char	**path;
	t_cmd	*cmds;
	int		n;
	int		status;
}			t_pipex;

/*		utils.c	*/

void	get_path(t_pipex *p, char **envp);
void	find_cmd(t_pipex *p, int i);
void	get_cmds(char **argv, t_pipex *p);

/*		exit.c	*/

void	clean_path(t_pipex *p);
void	clean_cmds(t_pipex *p);
void	close_fds(t_pipex *p);
void	close_fd(t_pipex *p, int k);

/*		exec.c	*/

void	create_pipes(char **argv, t_pipex *p);
void	init_p(int argc, t_pipex *p, bool bonus);
void	exec_child(t_pipex *p, int k, char **argv, char **envp);
void	exit_pipex(t_pipex *p, char *s1, char *s2, int returncode);

#endif