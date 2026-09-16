/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demain <demain@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 16:32:58 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/16 12:36:58 by demain           ###   #######belgium.be */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	pq_higher_priority(t_queue_node *a, t_queue_node *b, int scheduler)
{
	if (scheduler == 0)
		return (a->seq < b->seq);
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	return (a->seq < b->seq);
}

// permet d'ajouter un codeur dans la file d'attente d'un dongle
void	pq_push(t_queue *pq, t_queue_node node)
{
	if (pq->size == 0)
		pq->nodes[0] = node;
	else if (pq_higher_priority(&node, &pq->nodes[0], pq->scheduler))
	{
		pq->nodes[1] = pq->nodes[0];
		pq->nodes[0] = node;
	}
	else
		pq->nodes[1] = node;
	pq->size++;
}

// permet de pop le codeur qui prend le dongle 
// pour le retirer de la file d'attente
t_queue_node	pq_pop(t_queue *pq)
{
	t_queue_node	top;

	top = pq->nodes[0];
	pq->size--;
	if (pq->size > 0)
		pq->nodes[0] = pq->nodes[1];
	return (top);
}

// Permet de donner l'id du codeur en tête de la file d'attente 
// pour savoir si c'est son tour de prendre le dongle
int	pq_peek_id(t_queue *pq)
{
	if (pq->size == 0)
		return (-1);
	return (pq->nodes[0].id);
}