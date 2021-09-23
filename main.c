/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybesbes <ybesbes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 11:10:34 by ybesbes           #+#    #+#             */
/*   Updated: 2021/09/23 11:10:38 by ybesbes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philosophers.h"

int	thread_join(t_list **threads, t_list **monitors, int philo_num)
{
	int			i;
	int			ret;
	int			r;
	pthread_t	*tmp;

	i = 0;
	while (i < philo_num)
	{
		tmp = (pthread_t *)get_from_list_by_id(*threads, i);
		ret = pthread_join(*tmp, NULL);
		tmp = (pthread_t *)get_from_list_by_id(*monitors, i);
		r = pthread_join(*tmp, NULL);
		if (ret != 0 || r != 0)
		{
			printf("ERROR while threads join");
			return (1);
		}
		i++;
	}
	return (0);
}

int	full_impair_job_func(int philo_num, t_list **threads,
		t_list **monitors, t_list **philo)
{
	int	i;
	int	ret;
	int	r;

	i = 1;
	while (i < philo_num)
	{
		ret = pthread_create(get_from_list_by_id(*threads, i),
				NULL, &func, get_from_list_by_id(*philo, i));
		r = pthread_create(get_from_list_by_id(*monitors, i),
				NULL, &func2, get_from_list_by_id(*philo, i));
		if (ret != 0 || r != 0)
		{
			printf("ERROR while creating threads");
			return (1);
		}
		i = i + 2;
	}
	return (0);
}

int	full_pair_job(int philo_num, t_list **threads,
		t_list **monitors, t_list **philo)
{
	int	i;
	int	ret;
	int	r;

	i = 0;
	while (i < philo_num)
	{
		ret = pthread_create(get_from_list_by_id(*threads, i),
				NULL, &func, get_from_list_by_id(*philo, i));
		r = pthread_create(get_from_list_by_id(*monitors, i),
				NULL, &func2, get_from_list_by_id(*philo, i));
		if (ret != 0 || r != 0)
		{
			printf("ERROR while creating threads");
			return (1);
		}
		i = i + 2;
	}
	return (0);
}

int	mainly_work(t_arguments *arg, t_general *general)
{
	t_list			*threads;
	t_list			*monitors;
	t_list			*philo;
	t_list			*forks;

	threads = NULL;
	monitors = NULL;
	philo = NULL;
	forks = NULL;
	create_forks_list(*arg, &forks);
	create_philo_list(*arg, general, &philo, forks);
	create_threads_and_monitors_lists(*arg, &threads, &monitors);
	if (full_pair_job(arg->number_of_philo, &threads, &monitors, &philo) == 1)
		return (1);
	usleep(1000);
	if (full_impair_job_func(arg->number_of_philo,
			&threads, &monitors, &philo) == 1)
		return (1);
	if (thread_join(&threads, &monitors, arg->number_of_philo) == 1)
		return (1);
	free_all(&forks, &monitors, &threads, &philo);
	return (0);
}

int	main(int ac, char **av)
{
	t_general		general;
	t_arguments		arg;
	pthread_mutex_t	print;

	if (manage_error(ac) == 1)
		return (1);
	if (check_arg(av[1]) == 0 || check_arg(av[2]) == 0 || check_arg(av[3]) == 0
		|| check_arg(av[4]) == 0 || (ac == 6 && check_arg(av[5]) == 0))
	{
		printf("ERROR : wrong arguments");
		return (1);
	}
	arg.number_of_philo = ft_atoi(av[1]);
	arg.time_to_die = ft_atoi(av[2]);
	arg.time_to_eat = ft_atoi(av[3]);
	arg.time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		arg.number_of_time_each_philosopher_must_eat = ft_atoi(av[5]);
	else
		arg.number_of_time_each_philosopher_must_eat = -1;
	general_init(&general, &print);
	if (mainly_work(&arg, &general) == 1)
		return (1);
	return (0);
}
