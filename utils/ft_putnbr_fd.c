/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 15:30:53 by pahenriq          #+#    #+#             */
/*   Updated: 2026/06/04 15:41:54 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr_recursive(long nb, int fd)
{
	char	mol;

	mol = nb % 10;
	nb /= 10;
	if (nb > 0)
		ft_putnbr_recursive(nb, fd);
	mol += 48;
	write(fd, &mol, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	nb;

	if (fd < 0)
		return ;
	nb = (long)n;
	if (nb < 0)
	{
		nb *= -1;
		write(fd, "-", 1);
	}
	ft_putnbr_recursive(nb, fd);
}
