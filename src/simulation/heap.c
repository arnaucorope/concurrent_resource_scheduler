/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoromin@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:54:11 by acoromin          #+#    #+#             */
/*   Updated: 2026/09/14 11:04:40 by acoromin         ###   ########.fr       */
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
	t_coder	*tmp;

	if (heap->size >= heap->capacity)
		return (0);
	heap->items[heap->size] = coder;
	heap->size++;
	if (heap->size == 2
		&& has_priority(heap->items[1], heap->items[0]))
	{
		tmp = heap->items[0];
		heap->items[0] = heap->items[1];
		heap->items[1] = tmp;
	}
	return (1);
}

void	heap_pop(t_heap *heap)
{
	if (heap->size == 0)
		return ;
	heap->size--;
	if (heap->size == 1)
		heap->items[0] = heap->items[1];
}

t_coder	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (heap->items[0]);
}

void	heap_remove(t_heap *heap, t_coder *coder)
{
	if (heap->size == 0)
		return ;
	if (heap->items[0] == coder)
	{
		heap->size--;
		if (heap->size == 1)
			heap->items[0] = heap->items[1];
		return ;
	}
	if (heap->size == 2 && heap->items[1] == coder)
		heap->size--;
}
