/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmds_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:54:15 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/09 14:05:11 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	last_cmd_bonus(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	close(pipex->pipe_fd[0]);
	if(pipex->heredoc == 1)
		pipex->outfile_fd = open(pipex->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		pipex->outfile_fd = open(pipex->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (pipex->outfile_fd == -1)
	{
		send_error_msg("cannot open output file\n");
		free_tab(pipex->child_args);
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	dup2(pipex->outfile_fd, STDOUT_FILENO);
}

void	first_cmd_bonus(t_pipex *pipex)
{
	close(pipex->pipe_fd[0]);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	close(pipex->pipe_fd[1]);
	if(pipex->heredoc == 1)
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
}

void	middle_cmd_bonus(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	dup2(pipex->tmp_outfd, STDOUT_FILENO);
	close(pipex->tmp_outfd);
	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	close(pipex->pipe_fd[0]);

}

void	forkchild_bonus(t_pipex *pipex, int i)
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
		pipex->child_args = ft_split(pipex->cmds[pipex->nb_cmds + 1 - /*pipex->heredoc*/ i], ' ');
		if (!pipex->child_args)
			parse_fail(pipex);
		else if (!pipex->child_args[0])
			command_fail(pipex);
		if (i == 0)
			last_cmd_bonus(pipex);
		else if (i == pipex->cmds /* - 1*/ )
			first_cmd_bonus(pipex);
		else if (i > 0  && i < pipex->cmds /* - 1 */ )
			middle_cmd_bonus(pipex);
		ft_execve(pipex);
		command_fail(pipex);
	}
	if (pipex->pid != 0)
	{
		if(pipex->tmp_outfd != -1)
			close(pipex->tmp_outfd);
		pipex->tmp_outfd = pipex->pipe_fd[1];
	}
}

void	wait_parent(t_pipex *pipex)
{
	int i;

	i = 0;
	waitpid(pipex->pid_last, &pipex->exit_code, 0);
	if (pipex->exit_code >= 255)
		pipex->exit_code = 127;
	while (i < pipex->nb_cmds)
	{
		wait(NULL);
		i++;
	}
}
