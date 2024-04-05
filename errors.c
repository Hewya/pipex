/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:54:22 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/05 15:47:45 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if(!tab)
		return;
	while(tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	command_fail(t_pipex *pipex)
{
	char *msg;

	msg = ft_strjoin_triple("pipex : command not found -> ", pipex->child_args[0], "\n");
	if(!msg)
		write(STDERR_FILENO, "pipex : command not found\n", 26);
	else
	{
		write(STDERR_FILENO, msg, ft_strlen(msg));
		free(msg);
	}
	free_tab(pipex->child_args);
	free_tab(pipex->paths);
	if (pipex->outfile_fd != -1)
		close(pipex->outfile_fd);
	if (pipex->infile_fd != -1)
		close(pipex->infile_fd);
	close(pipex->pipe_fd[0]);
	close(pipex->pipe_fd[1]);
	exit(EXIT_FAILURE);
}

void	parse_fail(t_pipex *pipex)
{
	write(STDERR_FILENO, "pipex : parsing failure\n", 24);
	free_tab(pipex->paths);
	exit(EXIT_FAILURE);
}

