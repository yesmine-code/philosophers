/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybesbes <ybesbes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 11:09:57 by ybesbes           #+#    #+#             */
/*   Updated: 2021/09/23 11:10:04 by ybesbes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philosophers.h"

void	ft_free(t_list *list)
{
	t_list	*tmp;
	t_list	*tmp2;

	tmp = list;
	while (tmp != NULL && tmp->content != NULL)
	{
		free(tmp->content);
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
}

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL || lst->next == NULL)
		return (lst);
	return (ft_lstlast(lst->next));
}

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	if (alst == NULL)
		return ;
	if (*alst == NULL)
		*alst = new;
	else
		ft_lstlast(*alst)->next = new;
}

void	*get_from_list_by_id(t_list *list, int id)
{
	t_list	*tmp;

	if (id < 0)
		return (NULL);
	tmp = list;
	while (tmp && id > 0)
	{
		tmp = tmp->next;
		id--;
	}
	if (id > 0)
		return (NULL);
	return (tmp->content);
}

void	create_philo_list(t_arguments arg, t_general *general,
	t_list **philo, t_list *forks)
{
	t_list			*new_philo;
	t_philosoher	*philosopher;
	int				i;

	i = 0;
	while (i < arg.number_of_philo)
	{
		new_philo = malloc(sizeof(t_list));
		philosopher = malloc(sizeof(t_philosoher));
		if (new_philo != NULL && philosopher != NULL)
		{
			new_philo->content = philosopher;
			new_philo->next = NULL;
		}
		philo_init2(new_philo->content, arg, general);
		philo_init(new_philo->content, i, get_from_list_by_id
			(forks, mini(i, (i + 1) % arg.number_of_philo)),
			get_from_list_by_id(forks, maxi(i, (i + 1) % arg.number_of_philo)));
		ft_lstadd_back(philo, new_philo);
		i++;
	}
}
