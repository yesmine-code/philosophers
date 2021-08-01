#include "philosophers.h"

enum status
{
	eating = 1,
	sleeping = 2,
	thinking = 3
};

long long current_timestamp(long long start_time)
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return (milliseconds - start_time);
}

void	philo_init(t_philosoher *philo, int i, pthread_mutex_t *fork_1, pthread_mutex_t *fork_2, t_arguments args, long long start_time, int *one_has_died)
{
	philo->args = args;
	philo->id = i;
	philo->status = thinking;
	philo->fork_1 = fork_1;
	philo->fork_2 = fork_2;
	philo->number_of_time_has_ate = 0;
	philo->start_time = start_time;
	philo->one_has_died = one_has_died;
}
float time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

int	check_death(long long time_last_eat, t_philosoher *philo)
{
	if (((current_timestamp(philo->start_time) - time_last_eat) >= philo->args.time_to_die) || *(philo->one_has_died) == 1)
	{
		if (*(philo->one_has_died) == 0)
			printf("%lld\t| %d\t| died\n", current_timestamp(philo->start_time), philo->id);
		*(philo->one_has_died) = 1;
		pthread_mutex_unlock(philo->fork_1);
		pthread_mutex_unlock(philo->fork_2);
		return 1;
	}
	return 0;
}

void *func(void *input)
{
	t_philosoher *philo;
	long long	time_last_eat;
	struct timeval current_time;
	philo = (t_philosoher*) input;
	time_last_eat = 0;


	//printf("hello i am the %d philosopher and my status is %d\n", philo->id, philo->status);
	while (philo->number_of_time_has_ate < philo->args.number_of_time_each_philosopher_must_eat ||  philo->args.number_of_time_each_philosopher_must_eat == -1)
	{
		pthread_mutex_lock(philo->fork_1);
		if (check_death(time_last_eat, philo) == 1)
			break;
		printf("%lld\t| %d\t| has taken a fork\n", current_timestamp(philo->start_time), philo->id);
		if (philo->fork_2 == NULL)
		{
			printf("%d\t| %d\t| died\n", philo->args.time_to_die, philo->id);
			break;
		}
		else
			pthread_mutex_lock(philo->fork_2);
		if (check_death(time_last_eat, philo) == 1)
			break;
		printf("%lld\t| %d\t| has taken a fork\n", current_timestamp(philo->start_time), philo->id);
		philo->status = eating;
		philo->number_of_time_has_ate = philo->number_of_time_has_ate + 1;
		time_last_eat = current_timestamp(philo->start_time);
		if (check_death(time_last_eat, philo) == 1)
			break;
		printf("%lld\t| %d\t| is eating\n", current_timestamp(philo->start_time), philo->id);
		usleep(philo->args.time_to_eat);
		pthread_mutex_unlock(philo->fork_1);
		pthread_mutex_unlock(philo->fork_2);
		if (check_death(time_last_eat, philo) == 1)
			break;
		philo->status = sleeping;
		printf("%lld\t| %d\t| is sleeping\n", current_timestamp(philo->start_time), philo->id);
		usleep(philo->args.time_to_sleep);
		if (check_death(time_last_eat, philo) == 1)
			break;
		philo->status = thinking;
		printf("%lld\t| %d\t| is thinking\n", current_timestamp(philo->start_time), philo->id);
	}
	return (NULL);
}

int main(int ac, char **av)
{
	t_arguments arg;
	int			n;
	int			i;
	int			ret;
	long long	start_time;
	int			one_has_died;
	
	i = 0;
	one_has_died = 0;
	if (ac < 5 || ac > 6)
	{
		printf("ERROR : wrong number of arguments");
		return 1;
	}
	if(check_arg(av[1]) == 0 || check_arg(av[2]) == 0 || check_arg(av[3]) == 0
		|| check_arg(av[4]) == 0 || (ac == 6 && check_arg(av[5]) == 0))
	{
		printf("ERROR : wrong arguments");
		return 1;
	}
	arg.number_of_philo = ft_atoi(av[1]);
	arg.time_to_die = ft_atoi(av[2]);
	arg.time_to_eat = ft_atoi(av[3]);
	arg.time_to_sleep = ft_atoi(av[4]);
	if(ac == 6)
		arg.number_of_time_each_philosopher_must_eat = ft_atoi(av[5]);
	else
		arg.number_of_time_each_philosopher_must_eat = -1;
	n = arg.number_of_philo;
	pthread_t thread[n];
	t_philosoher philo[n];
	pthread_mutex_t forks[n];

	while (i < n)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	start_time = current_timestamp(0);
	if (n == 1)
		philo_init(&philo[i], i, &forks[min(i, (i + 1) % n)], NULL, arg, start_time, &one_has_died);
	else
	{
		while (i < n)
		{
			philo_init(&philo[i], i, &forks[min(i, (i + 1) % n)], &forks[max(i, (i + 1) % n)], arg, start_time, &one_has_died);
			i++;
		}
	}
	i = 0;
	while(i < n)
	{
		ret = pthread_create(&thread[i], NULL,&func, &philo[i]);
		if(ret != 0)
		{
			printf("ERROR while creating threads");
			return 1;
		}
		i++;
	}
	i = 0;
	while (i < n)
	{
		ret = pthread_join(thread[i], NULL);
		if (ret != 0)
		{
			printf("ERROR while threads join");
			return 1;
		}
		i++;
	}
	return 0;
}