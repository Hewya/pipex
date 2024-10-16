/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:59:34 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/16 17:07:38 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	command_fail_bonus(t_pipex *pipex)
{
	send_error_msg("pipex : command not found\n");
	free_tab(pipex->child_args);
	free_tab(pipex->paths);
	if (pipex->outfile_fd != -1)
		close(pipex->outfile_fd);
	if (pipex->infile_fd != -1)
		close(pipex->infile_fd);
	if (pipex->pipe_fd[0] != -1)
	{
		close(pipex->pipe_fd[0]);
		close(pipex->pipe_fd[1]);
	}
	if (pipex->tmp_outfd != -1)
		close(pipex->tmp_outfd);
	exit(EXIT_FAILURE);
}

void	ft_pipex_bonus(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->nb_cmds)
	{
		if (i < (pipex->nb_cmds - 1))
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
}

void	pipex_init(int ac, char **av, char **envp, t_pipex *pipex)
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
	pipex->here_doc = 0;
	pipex->tmp_outfd = -1;
	pipex->child_args = NULL;
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	pipex_init(ac, av, envp, &pipex);
	if (ac > 5 || (ac > 6 && (ft_strncmp(av[1], "here_doc", 8) == 0)))
	{
		if (ft_strncmp(av[1], "here_doc", 8) == 0)
			heredoc_exec(pipex, ac);
		else
			ft_pipex_bonus(&pipex);
	}
	else
	{
		ft_printf("input error: ./bonus_pipex infile cmd1 ... cmdn outfile\n");
		ft_printf("input error :");
		ft_printf("./bonus_pipex here_doc LIMITER cmd1 ... cmdn outfile\n");
	}
	if (pipex.here_doc == 1 && pipex.infile_fd != -1)
	{
		close(pipex.infile_fd);
		unlink("temp");
	}
	free_tab(pipex.paths);
	return (pipex.exit_code);
}
