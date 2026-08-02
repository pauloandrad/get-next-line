/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pahenriq <pahenriq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 17:40:27 by pahenriq          #+#    #+#             */
/*   Updated: 2026/06/13 12:57:28 by pahenriq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew_clear_aux(void *content, t_list **lst_aux,
		void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;

	new_lst = ft_lstnew(f(content));
	if (!new_lst || !new_lst->content)
	{
		ft_lstclear(&new_lst, del);
		ft_lstclear(lst_aux, del);
		return (NULL);
	}
	return (new_lst);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*first_lst;
	t_list	*new_lst;
	t_list	*curr_lst;

	if (!lst || !f || !del)
		return (NULL);
	first_lst = NULL;
	while (lst)
	{
		new_lst = ft_lstnew_clear_aux(&lst->content, &first_lst, f, del);
		if (!first_lst)
		{
			curr_lst = new_lst;
			first_lst = curr_lst;
		}
		else
		{
			ft_lstadd_back(&curr_lst, new_lst);
			curr_lst = curr_lst->next;
		}
		lst = lst->next;
	}
	return (first_lst);
}
