/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:21:59 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/03 20:45:40 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main (int argc, char **argv, char **env)
{
    t_begin         *begin_list;
    t_begin_pipes   *pipes_list;

    begin_list = malloc(sizeof(*begin_list));
    pipes_list = malloc(sizeof(*pipes_list));
    if (!begin_list || !pipes_list)
        exit (1);
    begin_list->first = NULL;
    create_argv(argv); // --> je cree mes propres chaines
    create_tokens(begin_list, argv); // --> comme si je recupere le lexer
    init_pipes_list(pipes_list);
    printf("\nliste de mots : \n\n");
    affich_list(begin_list); // --> permet d'afficher tous les char* de mes tokens
    create_pipes_list(begin_list, pipes_list); // --> je cree une liste chainee dans laquelle chaque element correspondra a une tranche de pipes
    printf("\n ---\n\nliste de tranches de pipe : \n\n");
    affich_pipes_list(pipes_list);
    if (pipes_error(pipes_list))
        ft_error("syntax error near unexpected token \'|\'");  // message d'erreur : syntax error near unexpected token '|'
    if (redir_error(pipes_list))
        ft_error("syntax error near unexpected token \'<\' or \'>\'"); // message d'erreur : syntax error near unexpected token '< . >' (deuxieme des tokens a mettre ici avec au max 2 tokens d'affilee si identiques, ex: > ><><< , 2e token > uniquement)
    check_infile_part(pipes_list);
    check_outfile_part(pipes_list);
    printf("\n ---\n\naffichage des infiles et outfiles : \n\n");
    affich_infiles_outfiles(pipes_list);
    check_cmds_args(pipes_list);
    printf("\naffichage des cmds et args : \n\n");
    affich_cmds_args(pipes_list);
    printf("Reaffichage liste : \n\n");
    affich_pipes_list(pipes_list);
    create_heredocs(pipes_list);
    define_heredocs_in_part(pipes_list);
    read_heredoc(pipes_list);
    if (pipes_list->nb_pipes >= 1)
        begin_forks(pipes_list);
    //open_close_files(pipes_list);
    // je peux commencer la partie execution (est ce que la fonction precedente doit faire partie de l'execution ?)
    // pour chaque pipe_part je dois creer un process enfant avec fork() ; donc surement integrer la fonction open_close_files a cette partie
}

// a mettre dans la partie FORK
void    open_close_files(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;

    pipe_part = pipes_list->first;
    while (pipe_part)
    {
        i = 0;
        while (pipe_part->words[i])
        {
            if (ft_strcmp(pipe_part->words[i], "<") == 0)
            {
                if (!open_infile(pipe_part, pipe_part->words[i + 1]))
                    break;
            }
            else if (ft_strcmp(pipe_part->words[i], ">") == 0
                || ft_strcmp(pipe_part->words[i], ">>") == 0)
            {
                if (!open_outfile(pipe_part, pipe_part->words[i + 1], pipe_part->words[i]))
                    break;
            }
            i++;
        }
        pipe_part = pipe_part->next;
    }
}

int open_outfile(t_pipes_part *pipe_part, char *outfile, char *redir)
{
    if (pipe_part->fd[1])
        close(pipe_part->fd[1]);
    if (ft_strcmp(redir, ">") == 0)
        pipe_part->fd[1] = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else
        pipe_part->fd[1] = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (pipe_part->fd[1] < 0)
	{
		if (errno == ENOENT)
			ft_printf("%s: No such file or directory\n", outfile);
		else if (errno == EACCES)
			ft_printf("%s: Permission denied\n", outfile);
		else
			perror("Erreur lors de l'ouverture du fichier\n");
		return (0);
	}
    return (1);
}

int open_infile(t_pipes_part *pipe_part, char *infile)
{
    if (pipe_part->fd[0])
        close(pipe_part->fd[0]);
    pipe_part->fd[0] = open(infile, O_RDONLY, 0644);
	if (pipe_part->fd[0] < 0)
	{
		if (errno == ENOENT)
			ft_printf("%s: No such file or directory\n", infile);
		else if (errno == EACCES)
			ft_printf("%s: Permission denied\n", infile);
		else
			perror("Erreur lors de l'ouverture du fichier \n");
        return (0);
	}
    return (1);
}
