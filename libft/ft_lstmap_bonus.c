/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 17:16:25 by heda-sil          #+#    #+#             */
/*   Updated: 2022/11/18 17:35:48 by heda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*node;

	new_lst = NULL;
	if (lst && f && del)
	{
		while (lst)
		{
			node = ft_lstnew(f(lst->content));
			if (!node)
				ft_lstclear(&node, del);
			ft_lstadd_back(&new_lst, node);
			lst = lst->next;
		}
	}
	return (new_lst);
}
