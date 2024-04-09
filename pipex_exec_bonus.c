/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:59:34 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/09 16:33:57 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_pipex_bonus(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->nb_cmds)
	{
		if(i < (pipex->nb_cmds - 1)) // why - 1 ?
		{
			if (pipe(pipex->pipe_fd) == -1)
			{
				send_error_msg("pipe failed\n");
				free_tab(pipex->paths);
				exit(EXIT_FAILURE);
			}
		}
		forkchild_bonus(pipex, i);
		if (i == 0)
			pipex->pid_last = pipex->pid;
		if (i < (pipex->nb_cmds - 1))
			close(pipex->pipe_fd[0]);
		i++;
	}
	if (pipex->pid != 0)
		wait_parent(pipex);
		// close something maybe ??
}
void	ft_heredoc(t_pipex *pipex)
{
	
}

void pipex_init(int ac, char **av, char **envp, t_pipex *pipex)
{
	pipex->envp = envp;
	pipex->paths = path_extraction(envp);
	pipex->nb_cmds = ac - 3;
	pipex->cmds = av;
	pipex->infile = av[1];
	pipex->infile_fd = -1;
	pipex->outfile = av[ac -1];
	pipex->outfile_fd = -1;
	pipex->exit_code = 0;
	pipex->heredoc = 0;
	pipex->tmp_outfd = -1;
	pipex->child_args = NULL; 
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	pipex_init(&pipex, ac, av, envp);
	if (ac >= 5 && /* presence heredoc*/)
		ft_heredoc(&pipex); // gestion heredoc
	if (/* declanchement bonus */)
		ft_pipex_bonus(&pipex);
	if(pipex.heredoc == 0)
	{
		send_error_msg("pipex : input error -> ./bonus_pipex infile cmd1 ... cmdn outfile\n");
		send_error_msg("pipex : input error -> ./bonus_pipex here_doc LIMITER cmd1 ... cmdn outfile\n");
	}
	if (pipex.heredoc == 1)
		send_error_msg("pipex : input error -> ./bonus_pipex here_doc LIMITER cmd1 ... cmdn outfile\n");
	if(pipex.heredoc == 1 && pipex.infile_fd != -1)
	{
		close(pipex.infile_fd);
		unlink("temp");
	}
	free_tab(pipex.paths);
	return (pipex.exit_code);
}

