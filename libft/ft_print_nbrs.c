/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nbrs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 17:57:40 by heda-sil          #+#    #+#             */
/*   Updated: 2022/12/16 15:38:53 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_printer(t_print *print, unsigned int n, char *arr)
{
	int	flag;

	flag = 1;
	if (!print->precision && !n && print->flags[dot])
	{
		flag = 0;
		print->length--;
	}
	if (flag)
		ft_putstr_fd(arr, 1);
	free(arr);
}

static int	ft_set_len(t_print *print, unsigned int n, int type)
{
	int	len;

	len = ft_nbr_len(n, type, print->base);
	if (print->precision > len)
		len = print->precision;
	if ((!type && (int)n < 0) || print->flags[plus] || print->flags[space])
		len++;
	if (!print->precision && !n && print->flags[dot])
		len = 0;
	return (len);
}

void	ft_print_diu(t_print *print, int type)
{
	unsigned int	n;
	int				len;
	char			*print_nbr;

	n = va_arg(print->args, unsigned int);
	print_nbr = ft_itoa_base(print, n, type);
	len = ft_set_len(print, n, type);
	if (print->width && !print->flags[minus] && !print->flags[zero])
		ft_align(print, len);
	ft_nbr_formatter(print, len, n, type);
	ft_printer(print, n, print_nbr);
	if (print->flags[minus] && !print->flags[zero])
		ft_align(print, len);
}

void	ft_print_hex(t_print *print, int hex_type)
{
	unsigned int	n;
	int				len;
	char			*print_nbr;

	n = va_arg(print->args, unsigned int);
	print->base = HEX;
	print_nbr = ft_itoa_base(print, n, 1);
	len = ft_set_len(print, n, 1);
	if (print->width && !print->flags[minus] && !print->flags[zero])
		ft_align(print, len);
	ft_nbr_formatter(print, len, n, 1);
	if (n)
		ft_hex_formatter(print, print_nbr, hex_type);
	ft_printer(print, n, print_nbr);
	if (print->flags[minus] && !print->flags[zero])
		ft_align(print, len);
}
