/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 19:30:08 by pahenriq          #+#    #+#             */
/*   Updated: 2026/05/31 20:47:52 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_cnt_num_char(long n)
{
	if (n <= 9)
		return (1);
	return (1 + ft_cnt_num_char(n / 10));
}

static void	ft_alloc_num(char *str, long n, int *i)
{
	if (n <= 9)
	{
		str[*i] = n + '0';
		return ;
	}
	ft_alloc_num(str, n / 10, i);
	(*i)++;
	str[*i] = '0' + n % 10;
}

char	*ft_itoa(int n)
{
	int		sign;
	char	*str;
	long	num;
	int		i;

	num = (long)n;
	sign = 0;
	if (num < 0)
	{
		sign = 1;
		num *= -1;
	}
	str = malloc(ft_cnt_num_char(num) + sign + 1);
	if (!str)
		return (NULL);
	i = 0;
	if (sign)
	{
		str[i] = '-';
		i++;
	}
	ft_alloc_num(str, num, &i);
	str[i + 1] = '\0';
	return (str);
}
