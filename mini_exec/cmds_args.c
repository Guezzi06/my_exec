/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:20:17 by aguezzi           #+#    #+#             */
/*   Updated: 2024/05/31 18:14:32 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    affich_cmds_args(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;
    int             j;

    pipe_part = pipes_list->first;
    i = 1;
    while (pipe_part)
    {
        if (!pipe_part->cmd)
            printf("tranche %d : cmd = NULL ; ", i);
        else
            printf("tranche %d : cmd = %s ; ", i, pipe_part->cmd);
        if (!pipe_part->args[0])
            printf("args = NULL\n");
        else
        {
            j = 0;
            printf("args = ");
            while (pipe_part->args[j])
            {
                printf("%s - ", pipe_part->args[j]);
                j++;
            }
        }
        printf("\n");
        pipe_part = pipe_part->next;
        i++;
    }
    printf("\n");
}

void    check_cmds_args(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;
    
    pipe_part = pipes_list->first;
    pipe_part->nb_arg = 0;
    while (pipe_part)
    {
        i = 0;
        while (pipe_part->words[i])
        {
            if (ft_strcmp(pipe_part->words[i], ">") != 0
                && ft_strcmp(pipe_part->words[i], ">>") != 0
                && ft_strcmp(pipe_part->words[i], "<") != 0
                && ft_strcmp(pipe_part->words[i], "<<") != 0)
            {
                if (!pipe_part->cmd)
                    pipe_part->cmd = pipe_part->words[i];
                else
                    pipe_part->nb_arg++;
            }
            else
                i++;
            i++;
        }
        create_part_args(pipe_part);
        pipe_part = pipe_part->next;
    }
}

void    create_part_args(t_pipes_part *pipe_part)
{
    int i;
    int j;

    i = 0;
    j = 0;
    pipe_part->args = malloc(sizeof(char *) * (pipe_part->nb_arg + 2));
    printf("\nnb_args = %d\n", pipe_part->nb_arg);
    if (!pipe_part->args)
        exit (1);
    while (pipe_part->words[i])
    {
        if (ft_strcmp(pipe_part->words[i], ">") != 0
            && ft_strcmp(pipe_part->words[i], ">>") != 0
            && ft_strcmp(pipe_part->words[i], "<") != 0
            && ft_strcmp(pipe_part->words[i], "<<") != 0)
        {
            pipe_part->args[j] = pipe_part->words[i];
            j++;
        }
        else
            i++;
        i++;
    }
    pipe_part->args[j] = NULL;
}