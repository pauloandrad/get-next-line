/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 18:51:33 by pahenriq          #+#    #+#             */
/*   Updated: 2026/06/04 20:19:47 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	character;

	i = 0;
	character = (unsigned char)c;
	while (s[i])
	{
		if (s[i] == character)
			return ((char *)&s[i]);
		i++;
	}
	if (character == '\0')
		return ((char *)&s[i]);
	return (NULL);
}
