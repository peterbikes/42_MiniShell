/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 17:11:41 by heda-sil          #+#    #+#             */
/*   Updated: 2022/12/16 16:30:19 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft/libft.h"
# include <stdarg.h>
# include <unistd.h>

# define CONVERSION "cspdiuxX%"
# define FLAGS "-0.# +"
# define NFLAGS 6
# define DEC "0123456789"
# define HEX "0123456789abcdef"

typedef struct s_print
{
	va_list	args;
	int		flags[NFLAGS];
	int		precision;
	int		width;
	int		length;
	char	*base;
}	t_print;

enum	e_flags
{
	minus,
	dot,
	hash,
	plus,
	space,
	zero
};

//Main printf helper functions
int		ft_printf(const char *format, ...);
int		ft_get_format(t_print *print, const char *format, int index);
int		ft_get_digits(t_print *print, const char *format, int index);
void	ft_converter(t_print *print, const char *format, int index);
t_print	*ft_print_init(t_print *print);

//convert nbrs
void	ft_print_diu(t_print *print, int type);
void	ft_print_hex(t_print *print, int hex_type);

//printf nbr util functions
void	ft_align(t_print *print, int len);
void	ft_nbr_formatter(t_print *print, int len, unsigned int n, int type);
void	ft_hex_formatter(t_print *print, char *hex, int hex_type);
char	*ft_itoa_base(t_print *print, unsigned int n, int type);
int		ft_nbr_len(unsigned int n, int type, char *base);

//Convert chars
void	ft_print_char(t_print *print);
void	ft_print_str(t_print *print);

//Convert ptrs
void	ft_print_ptr(t_print *print);

#endif
