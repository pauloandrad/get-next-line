/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:50:31 by pahenriq          #+#    #+#             */
/*   Updated: 2026/06/04 17:02:51 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	max_nmemb;
	void	*p;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	max_nmemb = INT_MAX / size;
	if (max_nmemb < nmemb)
		return (NULL);
	p = malloc(nmemb * size);
	if (p != NULL)
		ft_bzero(p, nmemb * size);
	return (p);
}
