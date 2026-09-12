/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:54:11 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/12 18:04:49 by acoromin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	has_priority(t_coder *a, t_coder *b)
{
	if (a->data->scheduler == SCHEDULER_FIFO)
		return (a->request.order < b->request.order);
	if (a->request.deadline != b->request.deadline)
		return (a->request.deadline < b->request.deadline);
	return (a->id < b->id);
}

int	heap_push(t_heap *heap, t_coder *coder)
{
	int		i;
	int		parent;
	t_coder	*tmp;

	if (heap->size >= heap->capacity)
		return (0);
	i = heap->size;
	heap->items[i] = coder;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!has_priority(heap->items[i], heap->items[parent]))
			break ;
		tmp = heap->items[i];
		heap->items[i] = heap->items[parent];
		heap->items[parent] = tmp;
		i = parent;
	}
	return (1);
}

t_coder	*heap_pop(t_heap *heap)
{
	t_coder	*result;
	int		i;
	int		left;
	int		right;
	int		best;
	t_coder	*tmp;

	if (heap->size == 0)
		return (NULL);
	result = heap->items[0];
	heap->size--;
	if (heap->size == 0)
		return (result);
	heap->items[0] = heap->items[heap->size];
	i = 0;
	while (1)
	{
		left = i * 2 + 1;
		right = i * 2 + 2;
		best = i;
		if (left < heap->size
			&& has_priority(heap->items[left], heap->items[best]))
			best = left;
		if (right < heap->size
			&& has_priority(heap->items[right], heap->items[best]))
			best = right;
		if (best == i)
			break ;
		tmp = heap->items[i];
		heap->items[i] = heap->items[best];
		heap->items[best] = tmp;
		i = best;
	}
	return (result);
}
