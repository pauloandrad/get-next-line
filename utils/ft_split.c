/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:56:05 by pahenriq          #+#    #+#             */
/*   Updated: 2026/05/31 18:47:37 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_cntsubs(char const *s, char c)
{
	size_t	subs_len;
	size_t	i;

	i = 0;
	subs_len = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
		{
			subs_len++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (subs_len);
}

static char	**ft_free_matrix(void **matrix, size_t size)
{
	size_t	i;

	if (!matrix)
		return (NULL);
	i = 0;
	while (i < size && matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
	return (NULL);
}

static char	**ft_malloc_split(char const *s, char c)
{
	char	**s_list;

	if (!s)
		return (NULL);
	s_list = malloc((ft_cntsubs(s, c) + 1) * sizeof(char *));
	if (!s_list)
		return (NULL);
	return (s_list);
}

static void	ft_skip_spliter(size_t *i, char const *s, char c)
{
	while (s[*i] && s[*i] == c)
		(*i)++;
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	split_i;
	char	**s_list;

	s_list = ft_malloc_split(s, c);
	if (!s_list)
		return (NULL);
	i = 0;
	split_i = 0;
	while (s[i])
	{
		ft_skip_spliter(&i, s, c);
		if (!s[i])
			break ;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		s_list[split_i] = ft_substr(s, start, i - start);
		if (!s_list[split_i])
			return (ft_free_matrix((void **)s_list, split_i));
		split_i++;
	}
	s_list[split_i] = NULL;
	return (s_list);
}
