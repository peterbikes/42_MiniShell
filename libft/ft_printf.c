/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:15:27 by heda-sil          #+#    #+#             */
/*   Updated: 2022/12/16 16:25:03 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

//check page 327 from c-lang pdf
int	ft_get_format(t_print *print, const char *format, int index)
{
	while (ft_strchr(FLAGS, format[++index]) || ft_isdigit(format[index]))
	{
		if (format[index] == '-')
		{
			print->flags[minus] = 1;
			print->flags[zero] = 0;
		}
		else if (format[index] == '0' && !print->flags[dot])
			print->flags[zero] = 1;
		else if (format[index] == '.')
		{
			print->flags[dot] = 1;
			print->flags[zero] = 0;
		}
		else if (format[index] == '#')
			print->flags[hash] = 1;
		else if (format[index] == ' ')
			print->flags[space] = 1;
		else if (format[index] == '+')
			print->flags[plus] = 1;
		else if (ft_isdigit(format[index]))
			index = ft_get_digits(print, format, index);
	}
	return (index);
}

int	ft_get_digits(t_print *print, const char *format, int index)
{
	if (format[index - 1] == '.')
		print->precision = ft_atoi(&format[index]);
	else
		print->width = ft_atoi(&format[index]);
	while (ft_isdigit(format[index]))
		index++;
	return (index - 1);
}

void	ft_converter(t_print *print, const char *format, int index)
{
	if (format[index] == 'c')
		ft_print_char(print);
	if (format[index] == 's')
		ft_print_str(print);
	if (format[index] == 'p')
		ft_print_ptr(print);
	if (format[index] == 'd' || format[index] == 'i')
		ft_print_diu(print, 0);
	if (format[index] == 'u')
		ft_print_diu(print, 1);
	if (format[index] == 'x')
		ft_print_hex(print, 0);
	if (format[index] == 'X')
		ft_print_hex(print, 1);
	if (format[index] == '%')
		print->length += write(1, "%", 1);
}

t_print	*ft_print_init(t_print *print)
{
	int	i;

	i = NFLAGS - 1;
	while (i >= 0)
	{
		print->flags[i] = 0;
		i--;
	}
	print->base = DEC;
	print->width = 0;
	print->length = 0;
	print->precision = 0;
	return (print);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	int		length;
	t_print	print;

	ft_print_init(&print);
	va_start(print.args, format);
	length = 0;
	i = -1;
	while (format[++i])
	{
		if (format[i] == '%')
		{
			ft_print_init(&print);
			i = ft_get_format(&print, format, i);
			ft_converter(&print, format, i);
			length += print.length;
		}
		else
			length += write(1, &format[i], 1);
	}
	va_end(print.args);
	return (length);
}
