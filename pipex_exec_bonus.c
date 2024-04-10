/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:59:34 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/10 19:54:26 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_pipex_bonus(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->nb_cmds)
	{
		if(i < (pipex->nb_cmds - 1))
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

void	ft_heredoc(t_pipex *pipex)
{
	char *buf;

	buf = NULL;
	pipex->here_doc = 1;
	pipex->nb_cmds -= 2;
	pipex->infile_fd = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (pipex->infile_fd == -1)
		input_fail(pipex);
	while(1)
	{
		write(STDERR_FILENO, ">> ", 3);
		buf = get_next_line(STDIN_FILENO);
		if(!buf)
			heredoc_error(pipex);
		if ((ft_strncmp("", pipex->cmds[2], ft_strlen(pipex->cmds[2]))== 0)
			&& ((ft_strncmp("\n", buf, ft_strlen(buf))) == 0))
			break;
		if (ft_strncmp(buf, "\n", 1) != 0)
			if (ft_strncmp(buf, pipex->cmds[2], (ft_strlen(buf) - 1)) == 0)
				break;
		write(pipex->infile_fd, buf, ft_strlen(buf));
		free(buf);
	}
	free(buf);
	close(pipex->infile_fd);
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
	if (ac <= 5)
	{
		send_error_msg("pipex : input error -> ./bonus_pipex infile cmd1 ... cmdn outfile\n");
		send_error_msg("pipex : input error -> ./bonus_pipex here_doc LIMITER cmd1 ... cmdn outfile\n");
		free_tab(pipex.paths);
		exit(EXIT_FAILURE);
	}
	if (ac >= 5 && ft_strncmp(av[1], "here_doc", 8) == 0)
		ft_heredoc(&pipex);
	if (ac >= 5 || (ac >= 6 && pipex.here_doc == 1))
		ft_pipex_bonus(&pipex);
	if(pipex.here_doc == 1 && pipex.infile_fd != -1)
	{
		close(pipex.infile_fd);
		unlink("temp");
	}
	free_tab(pipex.paths);
	return (pipex.exit_code);
}

