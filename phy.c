/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phy.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybesbes <ybesbes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 11:49:46 by ybesbes           #+#    #+#             */
/*   Updated: 2021/09/23 11:49:48 by ybesbes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philosophers.h"

long long	current_timestamp(long long start_time)
{
	struct timeval	te;
	long long		milliseconds;

	gettimeofday(&te, NULL);
	milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	return (milliseconds - start_time);
}

void	philo_init(t_philosoher *philo, int i,
			pthread_mutex_t *fork_1, pthread_mutex_t *fork_2)
{
	philo->id = i;
	philo->status = thinking;
	philo->fork_1 = fork_1;
	philo->fork_2 = fork_2;
	philo->number_of_time_has_ate = 0;
	philo->time_last_eat = 0;
}

void	general_init(t_general *general, pthread_mutex_t *print)
{
	pthread_mutex_init(print, NULL);
	general->one_has_died = 0;
	general->start_time = current_timestamp(0);
	general->print = print;
}

void	philo_init2(t_philosoher *philo, t_arguments args,
			t_general *general)
{
	philo->args = args;
	philo->general = general;
}

void	print(char *str, t_philosoher philo, int dead)
{
	pthread_mutex_lock(philo.general->print);
	if (philo.general->one_has_died == 0)
	{
		printf("%lld\t| %d\t| %s\n",
			current_timestamp(philo.general->start_time),
			philo.id + 1, str);
		philo.general->one_has_died = dead;
	}
	pthread_mutex_unlock(philo.general->print);
}
