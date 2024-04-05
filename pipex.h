/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:09:47 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/05 14:48:20 by gabarnou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_pipex
{
	int		infile_fd;		// identifiant du infile
	int		outfile_fd;		// identifiant du outfile
	char	*infile;		// av[1]
	char	*outfile;		// av[n]
	int		nb_cmds;		// nombre de commande
	char	**cmds;			// av[2] a av[n - 1]
	char	**child_args;	// toutes les parties d'une commande
	char	**paths;		// extrait de envp
	char	**envp;			// envp input
	int		pipe_fd[2];		// pipes fd : [0] = read, [1] = write
	int		pid;			// id du fork actuel
	int		pid_last;		// id de la 1er commande a retourner
	int		exit_code;		// 


}			t_pipex;

/* UTILS */
char	**path_extraction(char **envp);
char	*ft_strjoin_triple(char const *s1, char const *s2, char const *s3);
void	ft_execve(t_pipex *pipex);
void	close_all(t_pipex *pipex);

/* ERRORS */
void	free_tab(char **tab);
void	command_fail(t_pipex *pipex);
void	parse_fail(t_pipex *pipex);

/* PIPEX */
int		main(int ac, char **av, char **envp);
void	ft_pipex(t_pipex *pipex);
void	forkchild(t_pipex *pipex, int i);
void	first_cmd(t_pipex *pipex);
void	last_cmd(t_pipex *pipex);

#endif