/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybesbes <ybesbes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 11:18:55 by ybesbes           #+#    #+#             */
/*   Updated: 2021/09/23 11:18:58 by ybesbes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philosophers.h"

void	create_forks_list(t_arguments arg, t_list **forks)
{
	t_list			*new_fork;
	pthread_mutex_t	*mutex_fork;
	int				i;

	i = 0;
	while (i < arg.number_of_philo)
	{
		mutex_fork = malloc(sizeof(pthread_mutex_t));
		new_fork = malloc(sizeof(t_list));
		if (new_fork != NULL)
		{
			new_fork->content = mutex_fork;
			new_fork->next = NULL;
		}
		pthread_mutex_init(new_fork->content, NULL);
		ft_lstadd_back(forks, new_fork);
		i++;
	}
}

void	init_malloc(t_list **new_monitor, pthread_t **monitor,
	t_list **new_thread, pthread_t **thread)
{
	*new_monitor = malloc(sizeof(t_list));
	*new_thread = malloc(sizeof(t_list));
	*thread = malloc(sizeof(pthread_t));
	*monitor = malloc(sizeof(pthread_t));
}

void	create_threads_and_monitors_lists(t_arguments arg,
	t_list **threads, t_list **monitors)
{
	t_list		*new_thread;
	pthread_t	*thread;
	t_list		*new_monitor;
	pthread_t	*monitor;
	int			i;

	i = 0;
	while (i < arg.number_of_philo)
	{
		init_malloc(&new_monitor, &monitor, &new_thread, &thread);
		if (new_thread != NULL)
		{
			new_thread->content = thread;
			new_thread->next = NULL;
		}
		if (new_monitor != NULL)
		{
			new_monitor->content = monitor;
			new_monitor->next = NULL;
		}
		ft_lstadd_back(threads, new_thread);
		ft_lstadd_back(monitors, new_monitor);
		i++;
	}
}

int	mini(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

int	maxi(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}
