/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 18:51:33 by pahenriq          #+#    #+#             */
/*   Updated: 2026/06/13 12:32:57 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				i;
	char			*p;
	unsigned char	character;

	i = 0;
	p = NULL;
	character = (unsigned char)c;
	while (s[i])
	{
		if (s[i] == character)
			p = ((char *)&s[i]);
		i++;
	}
	if (character == '\0')
		return ((char *)&s[i]);
	return (p);
}
