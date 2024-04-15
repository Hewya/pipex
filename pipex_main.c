/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 21:48:43 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/15 22:33:04 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **envp)
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
}
