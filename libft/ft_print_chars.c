/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_chars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 13:42:51 by heda-sil          #+#    #+#             */
/*   Updated: 2022/12/16 15:17:30 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_char(t_print *print)
{
	unsigned char	c;

	c = va_arg(print->args, int);
	if (print->width && !print->flags[minus])
		ft_align(print, 1);
	print->length += write(1, &c, 1);
	if (print->flags[minus])
		ft_align(print, 1);
}

static int	ft_set_str_len(t_print *print, char *str, int flag)
{
	int	len;

	len = ft_strlen(str);
	if (print->precision < len && print->flags[dot])
	{
		if (flag)
			len = 0;
		else
			len = print->precision;
	}
	return (len);
}

void	ft_print_str(t_print *print)
{
	char	*str;
	int		i;
	int		flag;
	int		len;

	flag = 0;
	str = va_arg(print->args, char *);
	if (!str)
	{
		str = "(null)";
		flag = 1;
	}
	len = ft_set_str_len(print, str, flag);
	if (!print->flags[minus])
		ft_align(print, len);
	i = 0;
	while (str[i] && i < len)
	{
		print->length += write(1, &str[i], 1);
		i++;
	}
	if (print->flags[minus])
		ft_align(print, len);
}
