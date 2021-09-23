/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phy2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybesbes <ybesbes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 12:07:20 by ybesbes           #+#    #+#             */
/*   Updated: 2021/09/23 12:07:23 by ybesbes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philosophers.h"

void	*func2(void *input)
{
	t_philosoher	*philo;

	philo = (t_philosoher *) input;
	while (1)
	{
		if (philo->general->one_has_died == 1 || philo->status == done)
			break ;
		if (((current_timestamp(philo->general->start_time)
					- philo->time_last_eat) >= philo->args.time_to_die)
			&& philo->status != eating && philo->status != done)
		{
			print("died", *philo, 1);
			pthread_mutex_unlock(philo->fork_1);
			pthread_mutex_unlock(philo->fork_2);
			break ;
		}
	}
	return (NULL);
}

void	philo_eat(t_philosoher **philo)
{
	pthread_mutex_lock((*philo)->fork_1);
	print("has taken a fork", **philo, 0);
	pthread_mutex_lock((*philo)->fork_2);
	print("has taken a fork", **philo, 0);
	(*philo)->status = eating;
	(*philo)->number_of_time_has_ate = (*philo)->number_of_time_has_ate + 1;
	print("is eating", **philo, 0);
}

void	*func(void *input)
{
	t_philosoher	*philo;

	philo = (t_philosoher *) input;
	while (philo->number_of_time_has_ate
		< philo->args.number_of_time_each_philosopher_must_eat
		|| philo->args.number_of_time_each_philosopher_must_eat == -1)
	{
		if (philo->general->one_has_died == 1)
			break ;
		philo_eat(&philo);
		philo->time_last_eat = current_timestamp(philo->general->start_time);
		usleep(philo->args.time_to_eat * 1000);
		pthread_mutex_unlock(philo->fork_1);
		pthread_mutex_unlock(philo->fork_2);
		philo->status = sleeping;
		print("is sleeping", *philo, 0);
		usleep(philo->args.time_to_sleep * 1000);
		philo->status = thinking;
		print("is thinking", *philo, 0);
		if (philo->args.number_of_philo % 2 != 0)
			usleep(5000);
	}
	philo->status = done;
	return (NULL);
}

void	free_all(t_list **forks, t_list **monitors,
		t_list **threads, t_list **philo)
{
	if (*forks != NULL)
		ft_free(*forks);
	if (*monitors != NULL)
		ft_free(*monitors);
	if (*threads != NULL)
		ft_free(*threads);
	if (*philo != NULL)
		ft_free(*philo);
}

int	manage_error(int ac)
{
	if (ac < 5 || ac > 6)
	{
		printf("ERROR : wrong number of arguments");
		return (1);
	}
	return (0);
}
