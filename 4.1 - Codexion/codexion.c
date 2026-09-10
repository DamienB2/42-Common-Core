/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 15:07:59 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/10 17:50:32 by dbaise           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int simulation(t_params params)
{
    t_coder     *coders;
    t_shared    shared;
    int         i;

    coders = malloc(sizeof(t_coder) * params.nb_coders);
    if (!coders)
        return (0);
    if (pthread_mutex_init(&shared.print_lock, NULL) != 0)
    {
        free(coders);
        return (0);
    }
    shared.nb_say = 0;
    i = 0;
    while (i < params.nb_coders)
    {
        coders[i].id = i + 1;
        coders[i].nb_hello = params.nb_compiles;
        coders[i].shared = &shared;
        if (pthread_create(&coders[i].thread, NULL, say, &coders[i]) != 0)
        {
            pthread_mutex_destroy(&shared.print_lock);
            free(coders);
            return (0);
        }
        i++;
    }
    i = 0;
    while (i < params.nb_coders)
    {
        pthread_join(coders[i].thread, NULL);
        i++;
    }
    pthread_mutex_destroy(&shared.print_lock);
    free(coders);
    return (1);
}

void *say(void *arg)
{
    t_coder *coder;
    int     i;

    coder = (t_coder *)arg;
    i = 0;
    while (i < coder->nb_hello)
    {
        pthread_mutex_lock(&coder->shared->print_lock);
        printf("Hello from coder %d\n", coder->id);
        printf("print number %i\n", coder->shared->nb_say);
        coder->shared->nb_say += 1;
        pthread_mutex_unlock(&coder->shared->print_lock);
        i++;
    }
    return (NULL);
}


int	main(int argc, char *argv[])
{
	t_params	params;

	if (!parser(argc, argv, &params))
		return (0);
	if (!simulation(params))
        return (1);
	return (0);
}