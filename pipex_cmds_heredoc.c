/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmds_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 22:17:58 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/16 12:42:10 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	last_two_cmds_heredoc(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	close(pipex->pipe_fd[0]);
	if (pipex->here_doc == 1)
		pipex->outfile_fd = open(pipex->outfile,
				O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
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

void	first_two_cmds_heredoc(t_pipex *pipex)
{
	close(pipex->pipe_fd[0]);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	close(pipex->pipe_fd[1]);
	if (pipex->here_doc == 1)
		pipex->infile_fd = open("/tmp/temp", O_RDONLY);
	else
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

void	forkchild_two_cmds_heredoc(t_pipex *pipex, int i)
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
		pipex->child_args = ft_split
			(pipex->cmds[pipex->nb_cmds + 1 + pipex->here_doc - i], ' ');
		if (!pipex->child_args)
			parse_fail(pipex);
		else if (!pipex->child_args[0])
			command_fail(pipex);
		if (i == 0)
			last_two_cmds_heredoc(pipex);
		else if (i == 1)
			first_two_cmds_heredoc(pipex);
		ft_execve(pipex);
		command_fail(pipex);
	}
}

void	ft_pipex_two_cmds_heredoc(t_pipex *pipex)
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
		forkchild_two_cmds_heredoc(pipex, i);
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
