/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabarnou <gabarnou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 18:09:47 by gabarnou          #+#    #+#             */
/*   Updated: 2024/04/15 22:33:03 by gabarnou         ###   ########.fr       */
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
# include <stdbool.h>
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
	int		exit_code;		// valeur retour general
	int		here_doc;		// flag de presence d'un heredoc
	int		tmp_outfd;		// stock fd[1] entre fork
}			t_pipex;

/* UTILS */
char	**path_extraction(char **envp);
char	*ft_strjoin_triple(char const *s1, char const *s2, char const *s3);
void	ft_execve(t_pipex *pipex);
void	send_error_msg(char *str);

/* ERRORS */
void	free_tab(char **tab);
void	command_fail(t_pipex *pipex);
void	parse_fail(t_pipex *pipex);
void	input_fail(t_pipex *pipex);
void	heredoc_error(t_pipex *pipex);

/* PIPEX_MAIN */
int		main(int ac, char **av, char **envp);

/* PIPEX */
void	ft_pipex(t_pipex *pipex);
void	forkchild(t_pipex *pipex, int i);
void	first_cmd(t_pipex *pipex);
void	last_cmd(t_pipex *pipex);

/* BONUS_CMDS*/
void	last_cmd_bonus(t_pipex *pipex);
void	first_cmd_bonus(t_pipex *pipex);
void	middle_cmd_bonus(t_pipex *pipex);
void	forkchild_bonus(t_pipex *pipex, int i);
void	wait_parent(t_pipex *pipex);

/* BONUS_EXEC */
void	ft_pipex_bonus(t_pipex *pipex);
void	pipex_init(int ac, char **av, char **envp, t_pipex *pipex);
int		main(int ac, char **av, char **envp);

/* PIPEX_HEREDOC */
//bool	is_limiter(const char *buf, const char *delimiter);
void	heredoc_init(t_pipex *pipex);
void	ft_heredoc(t_pipex *pipex);
void	heredoc_exec(t_pipex pipex, int i);

/* PIPEX_CMDS_HEREDOC*/
void	forkchild_two_commands_heredoc(t_pipex *pipex, int i);
void	first_two_cmd_heredoc(t_pipex *pipex);
void	last_two_cmd_heredoc(t_pipex *pipex);

#endif
