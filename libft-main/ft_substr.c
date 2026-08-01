/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 13:05:22 by pahenriq          #+#    #+#             */
/*   Updated: 2026/06/06 14:02:15 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_malloc_empty(void)
{
	char	*empty;

	empty = (malloc(sizeof(char) * 1));
	if (!empty)
		return (NULL);
	empty[0] = '\0';
	return (empty);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_str;
	size_t	s_len;
	size_t	i;

	i = 0;
	if (!s)
		return (ft_malloc_empty());
	s_len = ft_strlen(s);
	if (s_len < start)
		return (ft_malloc_empty());
	if (len > s_len - start)
		len = s_len - start;
	sub_str = (char *)malloc((len + 1) * sizeof(char));
	if (!sub_str)
		return (ft_malloc_empty());
	while (i < len && s[start + i])
	{
		sub_str[i] = s[start + i];
		i++;
	}
	sub_str[i] = '\0';
	return (sub_str);
}
