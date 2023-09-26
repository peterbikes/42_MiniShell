/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 16:16:05 by heda-sil          #+#    #+#             */
/*   Updated: 2022/12/30 14:57:46 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	ft_size(long int n)
{
	int	size;

	size = 0;
	if (n == 0)
		size = 1;
	while (n > 0)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static long int	ft_abs(int n)
{
	long int	nb;

	nb = n;
	if (n < 0)
		return (-nb);
	return (nb);
}

char	*ft_itoa(int n)
{
	int		size;
	char	*arr;
	int		sign;

	size = ft_size(ft_abs(n));
	if (n < 0)
		arr = (char *)malloc((size + 1 + 1) * sizeof (*arr));
	else
		arr = (char *)malloc((size + 1) * sizeof (*arr));
	if (!arr)
		return (NULL);
	sign = 1;
	if (n < 0)
	{
		arr[0] = '-';
		sign = 0;
	}
	arr[size - sign + 1] = 0;
	while (size > 0)
	{
		arr[size - sign] = (ft_abs(n) % 10) + '0';
		n = n / 10;
		size--;
	}
	return (arr);
}
