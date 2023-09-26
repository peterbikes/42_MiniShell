/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_db_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:14:42 by heda-sil          #+#    #+#             */
/*   Updated: 2023/05/12 17:00:42 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_db_free(void *ptr)
{
	int		i;
	char	**mat;

	mat = (char **)ptr;
	if (mat)
	{
		i = -1;
		while (mat[++i])
		{
			if (mat[i])
				mat[i] = ft_free(mat[i]);
		}
		mat = ft_free(mat);
	}
	return (mat);
}
