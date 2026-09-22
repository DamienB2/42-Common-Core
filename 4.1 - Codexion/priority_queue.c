/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 16:32:58 by dbaise            #+#    #+#             */
/*   Updated: 2026/09/22 16:10:32 by dbaise           ###   #######belgium.be */
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

t_queue_node	pq_pop(t_queue *pq)
{
	t_queue_node	top;

	top = pq->nodes[0];
	pq->size--;
	if (pq->size > 0)
		pq->nodes[0] = pq->nodes[1];
	return (top);
}

int	pq_peek_id(t_queue *pq)
{
	if (pq->size == 0)
		return (-1);
	return (pq->nodes[0].id);
}

void	pq_remove_id(t_queue *pq, int id)
{
	if (pq->size == 0)
		return ;
	if (pq->nodes[0].id == id)
	{
		pq->size--;
		if (pq->size == 1)
			pq->nodes[0] = pq->nodes[1];
	}
	else if (pq->size == 2 && pq->nodes[1].id == id)
		pq->size--;
}