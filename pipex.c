/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:09:52 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/15 22:33:01 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	last_cmd(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	close(pipex->pipe_fd[0]);
	pipex->outfile_fd = open(pipex->outfile,
			O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (pipex->outfile_fd == -1)
	{
		send_error_msg("cannot open output file\n");
		free_tab(pipex->child_args);
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	dup2(pipex->outfile_fd, STDOUT_FILENO);
	close(pipex->outfile_fd);
}

void	first_cmd(t_pipex *pipex)
{
	close(pipex->pipe_fd[0]);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	close(pipex->pipe_fd[1]);
	pipex->infile_fd = open(pipex->infile, O_RDONLY);
	if (pipex->infile_fd == -1)
	{
		send_error_msg("cannot open input file\n");
		free_tab(pipex->child_args);
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	dup2(pipex->infile_fd, STDIN_FILENO);
	close(pipex->infile_fd);
}

void	forkchild(t_pipex *pipex, int i)
{
	pipex->pid = fork();
	if (pipex->pid == -1)
	{
		send_error_msg("fork failed\n");
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	if (pipex->pid == 0)
	{
		pipex->child_args = ft_split(pipex->cmds[pipex->nb_cmds + 1 - i], ' ');
		if (!pipex->child_args)
			parse_fail(pipex);
		else if (!pipex->child_args[0])
			command_fail(pipex);
		if (i == 0)
			last_cmd(pipex);
		else if (i == 1)
			first_cmd(pipex);
		ft_execve(pipex);
		command_fail(pipex);
	}
}

void	ft_pipex(t_pipex *pipex)
{
	int	i;

	i = 0;
	if (pipe(pipex->pipe_fd) == -1)
	{
		send_error_msg("pipe failed\n");
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	while (i < pipex->nb_cmds)
	{
		forkchild(pipex, i);
		if (i == 0)
			pipex->pid_last = pipex->pid;
		if (pipex->pid != 0)
			close(pipex->pipe_fd[0]);
		i++;
	}
	if (pipex->pid != 0)
	{
		close(pipex->pipe_fd[1]);
		waitpid(pipex->pid, NULL, 0);
		waitpid(pipex->pid_last, &pipex->exit_code, 0);
	}
}

/*int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	pipex.envp = envp;
	pipex.paths = path_extraction(envp);
	pipex.nb_cmds = ac - 3;
	pipex.cmds = av;
	pipex.infile = av[1];
	pipex.infile_fd = -1;
	pipex.outfile = av[ac -1];
	pipex.outfile_fd = -1;
	pipex.exit_code = 0;
	if (ac == 5)
		ft_pipex(&pipex);
	else
		ft_printf("pipex : input error -> ./pipex infile cmd1 cmd2 outfile\n");
	free_tab(pipex.paths);
	if (pipex.exit_code >= 256)
		pipex.exit_code = 127;
	return (pipex.exit_code);
}*/
