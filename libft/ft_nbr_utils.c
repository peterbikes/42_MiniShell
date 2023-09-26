/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nbr_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 17:57:45 by heda-sil          #+#    #+#             */
/*   Updated: 2022/12/16 15:36:23 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_align(t_print *print, int len)
{
	int	i;

	i = print->width - len;
	while (--i >= 0)
		print->length += write(1, " ", 1);
}

void	ft_nbr_formatter(t_print *print, int len, unsigned int n, int type)
{
	int	i;
	int	j;

	if (!type && (int)n < 0)
		print->length += write(1, "-", 1);
	else if (print->flags[plus] && !type)
		print->length += write(1, "+", 1);
	else if (print->flags[space] && !type)
		print->length += write(1, " ", 1);
	if ((print->flags[zero] && !print->flags[minus]) || print->flags[dot])
	{
		if (!type && (int)n < 0 && print->flags[dot])
			len--;
		i = print->width - len;
		if (!print->flags[dot])
			while (--i >= 0)
				print->length += write(1, "0", 1);
		else if (print->flags[dot])
		{
			j = print->precision - ft_nbr_len(n, type, print->base);
			while (--j >= 0)
				print->length += write(1, "0", 1);
		}
	}
}

void	ft_hex_formatter(t_print *print, char *hex, int hex_type)
{
	int	i;

	i = -1;
	if (print->flags[hash])
	{
		print->length += write(1, "0", 1);
		if (!hex_type)
			print->length += write(1, "x", 1);
	}
	if (hex_type)
	{
		if (print->flags[hash])
			print->length += write(1, "X", 1);
		while (hex[++i])
			hex[i] = ft_toupper(hex[i]);
	}
}

char	*ft_itoa_base(t_print *print, unsigned int n, int type)
{
	int		size;
	int		base_size;
	char	*arr;

	if (!type && (int)n < 0)
		n = -n;
	size = ft_nbr_len(n, type, print->base);
	base_size = ft_strlen(print->base);
	arr = ft_calloc((size + 1), sizeof(*arr));
	if (!arr)
		return (NULL);
	while (--size >= 0)
	{
		arr[size] = print->base[n % base_size];
		n /= base_size;
		print->length++;
	}
	return (arr);
}

int	ft_nbr_len(unsigned int n, int type, char *base)
{
	int	len;
	int	base_size;

	len = 0;
	base_size = ft_strlen(base);
	if (!n)
		len++;
	if (!type && (int)n < 0)
		n = -n;
	while (n > 0)
	{
		len++;
		n /= base_size;
	}
	return (len);
}
