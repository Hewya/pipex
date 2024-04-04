/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:09:52 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/04 15:49:51 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	last_cmd(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	dup2(pipex->pipe_fd[0], STDIN_FILENO);
	close(pipex->pipe_fd[0]);
	pipex->outfile_fd = open(pipex->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(pipex->outfile_fd == -1)
	{
		perror("cannot oen output file");
		free_tab(pipex->child_args);
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	dup2(pipex->outfile_fd, STDOUT_FILENO);
}
void	first_cmd(t_pipex *pipex)
{
	close(pipex->pipe_fd[0]);
	dup2(pipex->pipe_fd[1], STDOUT_FILENO);
	close(pipex->pipe_fd[1]);
	pipex->infile_fd = open(pipex->infile, O_RDONLY);
	if(pipex->infile_fd == -1)
	{
		perror("cannot open input file");
		free_tab(pipex->child_args);
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	dup2(pipex->infile_fd, STDIN_FILENO);
}

void	forkchild(t_pipex *pipex, int i)
{
	pipex->pid = fork();
	if(pipex->pid == -1)
	{
		perror("fork failed");
		free_tab(pipex->paths);
		exit(EXIT_FAILURE);
	}
	if(pipex->pid == 0) // processus fils
	{
		pipex->child_args = ft_split(pipex->cmds[pipex->nb_cmds /* + 1 -i*/], ' '); // HERE
		if(!pipex->child_args)
		{
			write(STDERR_FILENO, "pipex : cmd parsing failed", 26);
			free_tab(pipex->paths);
			exit(EXIT_FAILURE);
		}
		else if (!pipex->child_args[0])
			command_fail(pipex);
		if (i == 0)
			last_cmd(pipex);
		else if (i == 1)
			first_cmd;
		ft_execve(pipex);
	}
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
		if (pipex->pid != 0) // si enfant
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
	if(pipex.exit_code >= 256)
		pipex.exit_code = 127; //command not found
	return(pipex.exit_code);
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