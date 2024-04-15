/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:15:47 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/15 22:15:21 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// bool	is_limiter(const char *buf, const char *delimiter)
// {
// 	return (ft_strcmp(buf, delimiter) == 0);
// }

void	heredoc_init(t_pipex *pipex)
{
	pipex->here_doc = 1;
	pipex->nb_cmds -= 1;
	pipex->infile_fd = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (pipex->infile_fd == -1)
		input_fail(pipex);
}

void	ft_heredoc(t_pipex *pipex)
{
	char	*buf;
	size_t	len;

	buf = NULL;
	heredoc_init(pipex);
	while (1)
	{
		write(STDERR_FILENO, ">> ", 3);
		buf = get_next_line(STDIN_FILENO);
		if (!buf)
			heredoc_error(pipex);
		len = ft_strlen(buf);
		if (len > 0 && buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (ft_strcmp(buf, pipex->cmds[2]) == 0)
		{
			free(buf);
			break ;
		}
		buf[len - 1] = '\n';
		write(pipex->infile_fd, buf, ft_strlen(buf));
		free(buf);
	}
	close(pipex->infile_fd);
}

void	last_cmd_heredoc(t_pipex *pipex)
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

void	first_cmd_heredoc(t_pipex *pipex)
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
void	heredoc_exec(t_pipex pipex, int i)
{
	ft_heredoc(&pipex);
	pipex.nb_cmds = i - 4;
	if (pipex.here_doc == 1 && i == 6)
		ft_pipex(&pipex);
	else
		ft_pipex_bonus(&pipex);	
}
