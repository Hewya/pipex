/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:15:47 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/16 12:44:06 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	heredoc_exec(t_pipex pipex, int i)
{
	ft_heredoc(&pipex);
	pipex.nb_cmds = i - 4;
	if (pipex.here_doc == 1 && i == 6)
		ft_pipex_two_cmds_heredoc(&pipex);
	else
		ft_pipex_bonus(&pipex);
}
