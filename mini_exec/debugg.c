#include "minishell.h"

void    read_heredoc(t_begin_pipes *pipes_list)
{
    int     i;
    char    buffer[1024];
    ssize_t bytes_read;

    /*i = 0;
    printf("Affichage des heredocs a partir du pipes_list :\n\n");
    while (i < 2)
    {
        close(pipes_list->heredocs[i * 2 + 1]);
        while ((bytes_read = read(pipes_list->heredocs[i * 2], buffer, 1023)) > 0)
        {
            printf("car_lus : %ld\n", bytes_read);
            buffer[bytes_read] = '\0';
            printf("\nheredoc %d : \n%s\n---\n", i+1, buffer);
        }
        close(pipes_list->heredocs[i * 2]);
        if (bytes_read == -1)
        {
            printf("error read fonction\n");
            exit (1);
        }
        i++;
    }*/
    printf("Valeur fd de chaque heredoc de pipes_list : \n");
    for (int z = 0; z < 4; z++)
    {
        printf("%d\n", pipes_list->heredocs[z]);
    }
    printf("\nAffichage des heredocs a partir de chaque pipe_part :\n");
    t_pipes_part *pipe_part = pipes_list->first;

    while (pipe_part)
    {
        i = 0;
        printf("\nfd_heredoc = %d\n", pipe_part->heredocs_part[i * 2]);
        while (pipe_part->heredocs_part[i * 2] != -2)
        {
            close(pipe_part->heredocs_part[i * 2 + 1]);
            while ((bytes_read = read(pipe_part->heredocs_part[i * 2], buffer, 1023)) > 0)
            {
                printf("car_lus : %ld\n", bytes_read);
                buffer[bytes_read] = '\0';
                printf("\nheredoc : \n%s\n---\n", buffer);
            }
            close(pipe_part->heredocs_part[i * 2]);
            if (bytes_read == -1)
            {
                printf("error read fonction\n");
                exit (1);
            }
            i++;
        }
        pipe_part = pipe_part->next;
    } //***
}

// la partie *** permet de debugger pour voir si les heredocs sur chaque pipe_part sont bien assignes, et ca fonctionne !!