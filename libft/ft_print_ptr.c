/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 19:53:05 by heda-sil          #+#    #+#             */
/*   Updated: 2022/12/16 16:59:14 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*ft_itoall(t_print *print, unsigned long long n)
{
	int					size;
	unsigned long long	temp;
	char				*arr;

	temp = n;
	size = 0;
	if (!n)
		return ("(nil)");
	while (temp > 0)
	{
		size++;
		temp /= 16;
	}
	arr = ft_calloc((size +1), sizeof(*arr));
	if (!arr)
		return (NULL);
	while (--size >= 0)
	{
		arr[size] = print->base[n % 16];
		n /= 16;
		print->length++;
	}
	return (arr);
}

void	ft_print_ptr(t_print *print)
{
	unsigned long long	n;
	int					len;
	char				*print_ptr;

	n = va_arg(print->args, unsigned long long);
	print->base = HEX;
	print->flags[hash] = 1;
	print_ptr = ft_itoall(print, n);
	if (n)
		len = ft_strlen(print_ptr) + 2;
	else
	{
		len = ft_strlen(print_ptr);
		print->length += len;
	}
	if (print->width && !print->flags[minus] && !print->flags[zero])
		ft_align(print, len);
	if (n)
		ft_hex_formatter(print, print_ptr, 0);
	ft_putstr_fd(print_ptr, 1);
	if (n)
		free(print_ptr);
	if (print->flags[minus] && !print->flags[zero])
		ft_align(print, len);
}
