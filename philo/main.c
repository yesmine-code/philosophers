#include "include/philosophers.h"

enum e_status
{
	eating = 1,
	sleeping = 2,
	thinking = 3,
	done	= 4
};

long long current_timestamp(long long start_time)
{
    struct	timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return (milliseconds - start_time);
}

void	philo_init(t_philosoher *philo, int i, pthread_mutex_t *fork_1, pthread_mutex_t *fork_2)
{
	philo->id = i;
	philo->status = thinking;
	philo->fork_1 = fork_1;
	philo->fork_2 = fork_2;
	philo->number_of_time_has_ate = 0;
	philo->time_last_eat = 0;
}

void	general_init(t_general *general, pthread_mutex_t *philo_died, pthread_mutex_t *print)
{
	pthread_mutex_init(philo_died, NULL);
	pthread_mutex_init(print, NULL);
	pthread_mutex_lock(philo_died);
	general->philo_died = philo_died;
	general->one_has_died = 0;
	general->start_time = current_timestamp(0);
	general->print = print;
}

void	philo_init2(t_philosoher *philo, t_arguments args, t_general *general)
{
	philo->args = args;
	philo->general = general;
}

void print(char *str, t_philosoher philo, int dead)
{
	pthread_mutex_lock(philo.general->print);
	if (philo.general->one_has_died == 0)
	{
		printf("%lld\t| %d\t| %s\n", current_timestamp(philo.general->start_time), philo.id + 1, str);
		philo.general->one_has_died = dead;
	}
	pthread_mutex_unlock(philo.general->print);
}

void *func2(void *input)
{
	t_philosoher *philo;

	philo = (t_philosoher*) input;
	while (1)
	{
		if (philo->general->one_has_died == 1 || philo->status == done)
			break;
		if (((current_timestamp(philo->general->start_time) - philo->time_last_eat) >= philo->args.time_to_die) &&
				philo->status != eating && philo->status != done)
		{
			print("died", *philo, 1);
			pthread_mutex_unlock(philo->fork_1);
		//	if(philo->fork_2 != NULL)
			pthread_mutex_unlock(philo->fork_2);
			break;
		}
	}
	return (NULL);
}

void *func(void *input)
{
	t_philosoher *philo;
	philo = (t_philosoher*) input;
	while (philo->number_of_time_has_ate < philo->args.number_of_time_each_philosopher_must_eat ||  philo->args.number_of_time_each_philosopher_must_eat == -1)
	{
		if (philo->general->one_has_died == 1)
			break;
		pthread_mutex_lock(philo->fork_1);
		print("has taken a fork", *philo, 0);
	//	if(philo->fork_2 != NULL)
		pthread_mutex_lock(philo->fork_2);
		print("has taken a fork", *philo, 0);
		philo->status = eating;
		philo->number_of_time_has_ate = philo->number_of_time_has_ate + 1;
		print("is eating", *philo, 0);
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

int main(int ac, char **av)
{
	t_general		general;
	t_arguments		arg;
	pthread_mutex_t	philo_died;
	pthread_mutex_t	print;
	int			r;
	int			i;
	int			ret;

	i = 0;
	if (ac < 5 || ac > 6)
	{
		printf("ERROR : wrong number of arguments");
		return (1);
	}
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
	t_list		*threads;
	t_list		*monitors;
	t_list		*philo;
	t_list		*forks;

	threads = NULL;
	monitors = NULL;
	philo = NULL;
	forks = NULL;

	general_init(&general, &philo_died, &print);
	create_forks_list(arg, &forks);
	create_philo_list(arg, &general, &philo, forks);
	create_threads_and_monitors_lists(arg, &threads, &monitors);
	i = 0;
	while (i < arg.number_of_philo)
	{
		ret = pthread_create(get_from_list_by_id(threads, i), NULL, &func, get_from_list_by_id(philo, i));
		r = pthread_create(get_from_list_by_id(monitors, i), NULL, &func2, get_from_list_by_id(philo, i));
		if (ret != 0 || r != 0)
		{
			printf("ERROR while creating threads");
			return (1);
		}
		i = i + 2;
	}
	usleep(1000);
	i = 1;
	while (i < arg.number_of_philo)
	{
		ret = pthread_create(get_from_list_by_id(threads, i), NULL, &func, get_from_list_by_id(philo, i));
		r = pthread_create(get_from_list_by_id(monitors, i), NULL, &func2, get_from_list_by_id(philo, i));
		if (ret != 0 || r != 0)
		{
			printf("ERROR while creating threads");
			return (1);
		}
		i = i + 2;
	}
	i = 0;
	while (i < arg.number_of_philo)
	{
		pthread_t *tmp;
		tmp = (pthread_t *)get_from_list_by_id(threads, i);
		ret = pthread_join(*tmp, NULL);
		tmp = (pthread_t *)get_from_list_by_id(monitors, i);
		r = pthread_join(*tmp, NULL);
		if (ret != 0 || r != 0)
		{
			printf("ERROR while threads join");
			return (1);
		}
		i++;
	}
	if(forks != NULL)
		ft_free(forks);
	if(monitors != NULL)
		ft_free(monitors);
	if(threads!= NULL)
		ft_free(threads);
	if(philo != NULL)
		ft_free(philo);
	return (0);
}
