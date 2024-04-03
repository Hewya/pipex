/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:09:52 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/03 21:31:00 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	forkchild(t_pipex pipex, int i)
{
	
}

void	ft_pipex(t_pipex *pipex)
{
	int	i;

	i = 0;
	if(pipe(pipex->pipe_fd) == -1)
	{
		perror("pipe failed");
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	while (i < pipex->nb_cmds)
	{
		forkchild(pipex, i);
		if (i == 0)
			pipex->pid_last = pipex->pid;
		if (pipex->pid != 0)
			close(pipex->pipe_fd[0]); //fermer lecture
		i++;
	}
	if(pipex->pid != 0) // dans processus parent
	{
		close(pipex->pipe_fd[1]); // fermer ecriture parent
		waitpid(pipex->pid, NULL, 0); // parent attent fin exec prossecus fils
		waitpid(pipex->pid_last, &pipex->exit_code, 0); // attente du dernier child (bien attendre everyone)
	}
}

/*void	ft_fork()
{
	pid_t pid = fork();
	if (pid == -1)
	{
		// Il y a une erreur
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		// On est dans le fils
		printf("Mon PID est %i et celui de mon père est %i\n", getpid(),	getppid());
	}
	else
	{
		// On est dans le père
		printf("Mon PID est %i et celui de mon fils est %i\n", getpid(), pid);
	}
	return (EXIT_SUCCESS);
}*/


int	main(int ac, char **av, char **envp)
{
	t_pipex pipex;

	pipex.envp = envp;
	pipex.paths = path_extraction(envp);
	pipex.nb_cmds = ac - 3;
	pipex.cmds = av;
	pipex.infile = av[1];
	pipex.infile_fd = -1;
	pipex.outfile = av[ac -1];
	pipex.outfile_fd = -1;
	pipex.exit_code = 0;
	if(ac == 5)
		ft_pipex(&pipex);
	else
		ft_printf("pipex : input error -> ./pipex infile cmd 1 cmd 2 outfile\n"); //AJOUTER FT_PRINFT A LA LIBFT
	free_tab(pipex.paths);
	/*
	GERER LES EXIT_ERROR 
	*/
	if(/*quelque chose*/)
		pipex.exit_code = 127; //command not found
	return(pipex.exit_code);
}