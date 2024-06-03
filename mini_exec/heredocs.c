/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 16:14:36 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/03 20:45:22 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    create_heredocs(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;
    int             count; // stocker le nombre de heredocs
    
    count = 0;
    pipe_part = pipes_list->first;
    while (pipe_part)
    {
        i = 0;
        while (pipe_part->words[i])
        {
            if (ft_strcmp(pipe_part->words[i], "<<") == 0)
                count++;
            i++;
        }
        pipe_part = pipe_part->next;
    }
    pipe_part = pipes_list->first;
    pipes_list->heredocs = malloc(sizeof(int) * (2 * count));
    if (!pipes_list->heredocs)
        exit(1);
    count = 0;
    while (pipe_part)
    {
        i = 0;
        while (pipe_part->words[i])
        {
            if (ft_strcmp(pipe_part->words[i], "<<") == 0)
            {
                printf("\ncount = %d\n", count);
                heredoc(pipes_list, pipe_part->words[i + 1], count, i);
                count++;
            }
            i++;
        }
        pipe_part = pipe_part->next;
    }
}

void	heredoc(t_begin_pipes *pipes_list, char *end, int count, int i)
{
	char	*line;

	if (pipe(pipes_list->heredocs + (count * 2)) == -1)
		exit (1);
	while (1)
	{
		ft_printf("> ");
		line = get_next_line(0);
		if ((ft_strncmp(line, end, ft_strlen(end)) == 0
				&& ft_strlen(line) == ft_strlen(end) + 1)
			&& line[0] != '\n')
			break ;
		else
			write(pipes_list->heredocs[count * 2 + 1], line, ft_strlen(line));
	}
	free(line);
}

void    define_heredocs_in_part(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i; // index qui va parcourir les words pour chaque pipe_part
    int             j; // index qui va compter le nombre de heredocs pour le pipe_part actuel
    int             k; // index qui va parcourir les heredocs dans le pipes_list
    
    pipe_part = pipes_list->first;
    k = 0;
    while (pipe_part)
    {
        i = 0;
        j = 0;
        while (pipe_part->words[i])
        {
            if (ft_strcmp(pipe_part->words[i], "<<") == 0)
                j++;
            i++;
        }
        pipe_part->heredocs_part = malloc(sizeof(int) * (j * 2 + 1));
        if (!pipe_part->heredocs_part)
            exit (1);
        i = 0;
        while (i < j * 2)
        {
            pipe_part->heredocs_part[i] = pipes_list->heredocs[k];
            k++;
            i++;
        }
        pipe_part->heredocs_part[i] = -2;
        pipe_part = pipe_part->next;
    }
}