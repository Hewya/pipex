/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:09:52 by gabarnou          #+#    #+#             */
/*   Updated: 2024/03/28 01:07:26 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_pipex(t_pipex *pipex)
{
	
}

int	main(int ac, char **av, char **envp)
{
	t_pipex pipex;

	pipex.envp = envp;
	pipex.paths = path_extraction(envp);
	pipex.nb_cmds = ac - 3;
	pipex.cmds = av;
	pipex.infile = av[1];
	pipex.infile_fd = -1; //DOCU LE -1
	pipex.outfile = av[ac -1];
	pipex.outfile_fd = -1;
	if(ac == 5)
		ft_pipex(&pipex);
	else
		ft_printf("pipex : input error -> ./pipex infile cmd 1 cmd 2 outfile\n"); //AJOUTER FT_PRINFT A LA LIBFT
	free_tab(pipex.paths);
	/*
	GERER LES EXIT_ERROR 
	*/
	return(0); //A CHANGER
}