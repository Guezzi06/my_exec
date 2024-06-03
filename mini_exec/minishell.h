/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:22:35 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/03 20:31:15 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"
#include "libft/ft_printf.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

typedef struct s_token
{
    int             simple_q;
    int             double_q;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct s_begin
{
    t_token *first;
}   t_begin;

typedef struct s_pipes_part
{
    int                 index_pipe;
    char                **words;
    char                *infile;
    char                *outfile;
    int                 append;
    char                *cmd;
    char                **args;
    int                 nb_arg;
    int                 *heredocs_part; // je mets le paires de fd pour lire le fd de lecture et fermer au prealable le fd d ecriture
    int                 if_infile; // si == 1 : permet de savoir si je dois faire ma redirection dans un infile < ou pas
    int                 if_heredoc; // si == 1 : alors je vais chercher dans mon tableau de heredocs pour recuperer le fd correspondant et le mettre en tant que infile
    int                 fd[2]; // va contenir mon fd infile et mon fd outfile, init a -1 pour tester si il faut close le fd avant de le reouvrir avec un autre fichier
    
    // les fd ont la priorite sur les pipes en terme d'entree et de sortie, si infile_fd est != -1 alors ce sera notre entree, pareil pour outfile_fd
    struct s_pipes_part *next;
}   t_pipes_part;

typedef struct s_begin_pipes
{
    t_pipes_part    *first;
    int             nb_pipes;
    int             *p; // stocke les pipes par paire avec p[i*2] pour lecture et p[i*2 + 1] pour ecriture
    int             *heredocs;
    pid_t           *pids;
}   t_begin_pipes;

// creation de ma liste de base envoye par le lexer + creation de ma liste de tranches de pipe
void    affich_list(t_begin *begin_list);
void    create_tokens(t_begin *begin_list, char **argv);
void    create_argv(char **argv);
void    init_pipes_list(t_begin_pipes *pipes_list);
void    affich_pipes_list(t_begin_pipes *pipes_list);
void    create_pipes_list(t_begin *begin_list, t_begin_pipes *pipes_list);

// check error for pipes and redir
int     pipes_error(t_begin_pipes *pipes_list);
int     redir_error(t_begin_pipes *pipes_list);
void    ft_error(char *s);

// check tokens
void    check_infile_part(t_begin_pipes *pipes_list);
void    check_outfile_part(t_begin_pipes *pipes_list);
void    affich_infiles_outfiles(t_begin_pipes *pipes_list);
void    check_cmds_args(t_begin_pipes *pipes_list);
void    create_part_args(t_pipes_part *pipe_part);
void    affich_cmds_args(t_begin_pipes *pipes_list);

// handle heredocs, files and check errors
void    create_heredocs(t_begin_pipes *pipes_list);
void	heredoc(t_begin_pipes *pipes_list, char *end, int count, int i);
void    open_close_files(t_begin_pipes *pipes_list);
int     open_infile(t_pipes_part *pipe_part, char *infile);
int     open_outfile(t_pipes_part *pipe_part, char *outfile, char *redir);
void    define_heredocs_in_part(t_begin_pipes *pipes_list);
void    read_heredoc(t_begin_pipes *pipes_list);  //debuggage heredocs

#endif