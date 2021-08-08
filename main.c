#include "philosophers.h"

enum status
{
	eating = 1,
	sleeping = 2,
	thinking = 3,
	done	= 4
};

long long current_timestamp(long long start_time)
{
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
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

void	general_init(t_general *general, pthread_mutex_t *print)
{
	pthread_mutex_init(print, NULL);
	general->one_has_died = 0;
	general->start_time = current_timestamp(0);
	general->print = print;
}

void	philo_init2(t_philosoher *philo, t_arguments args, t_general *general, pthread_mutex_t *mutex_status, pthread_mutex_t *mutex_last_eat)
{
	philo->args = args;
	philo->general = general;
	philo->mutex_status = mutex_status;
	philo->mutex_last_eat = mutex_last_eat;
}

float time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

void print(char *str, t_philosoher *philo, int dead)
{
	pthread_mutex_lock(philo->general->print);
	if(philo->general->one_has_died == 0)
	{
		printf("%lld\t| %d\t| %s\n", current_timestamp(philo->general->start_time), philo->id + 1, str);
		philo->general->one_has_died = dead;
	}
	pthread_mutex_unlock(philo->general->print);
}

void *func2(void *input)
{
	t_philosoher *philo;

	philo = (t_philosoher*) input;
	while(1)
	{
		pthread_mutex_lock(philo->general->print);
		if (philo->general->one_has_died == 1) //|| philo->status == done)
		{
			pthread_mutex_unlock(philo->general->print);
			break;
		}
		pthread_mutex_unlock(philo->general->print);
		pthread_mutex_unlock(philo->mutex_status);
		pthread_mutex_unlock(philo->mutex_last_eat);
		if (((current_timestamp(philo->general->start_time) - philo->time_last_eat) >= philo->args.time_to_die) &&
				philo->status != eating && philo->status != done)
		{
			print("died", philo, 1);
			pthread_mutex_unlock(philo->mutex_last_eat);
			pthread_mutex_unlock(philo->mutex_status);
			pthread_mutex_unlock(philo->fork_1);
			pthread_mutex_unlock(philo->fork_2);
			break;
		}
	}

	return NULL;
}

void change_status(t_philosoher *philo, int status)
{
	pthread_mutex_lock(philo->mutex_status);
	philo->status = status;
	pthread_mutex_unlock(philo->mutex_status);
}

void *func(void *input)
{
	t_philosoher *philo;
	philo = (t_philosoher*) input;

	while (philo->number_of_time_has_ate < philo->args.number_of_time_each_philosopher_must_eat ||  philo->args.number_of_time_each_philosopher_must_eat == -1)
	{
		pthread_mutex_lock(philo->general->print);
		if (philo->general->one_has_died == 1)
		{
			pthread_mutex_unlock(philo->general->print);
			break;
		}
		pthread_mutex_unlock(philo->general->print);
		pthread_mutex_lock(philo->fork_1);
		print("has taken a fork", philo, 0);
		pthread_mutex_lock(philo->fork_2);
		print("has taken a fork", philo, 0);
		change_status(philo, eating);
		philo->number_of_time_has_ate = philo->number_of_time_has_ate + 1;
		//pthread_mutex_lock(philo->mutex_last_eat);
		philo->time_last_eat = current_timestamp(philo->general->start_time);
		//pthread_mutex_unlock(philo->mutex_last_eat);
		print("is eating", philo, 0);
		usleep(philo->args.time_to_eat * 1000);
		pthread_mutex_unlock(philo->fork_1);
		pthread_mutex_unlock(philo->fork_2);
		change_status(philo, sleeping);
		print("is sleeping", philo, 0);
		usleep(philo->args.time_to_sleep * 1000);
		change_status(philo, thinking);
		print("is thinking", philo, 0);
	}
	philo->status = done;
	return (NULL);
}

int main(int ac, char **av)
{
	t_general	general;
	t_arguments arg;
	int			r;
	int			i;
	int			ret;
	
	i = 0;
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
	pthread_t thread[arg.number_of_philo];
	pthread_t monitor[arg.number_of_philo];
	t_philosoher philo[arg.number_of_philo];
	pthread_mutex_t forks[arg.number_of_philo];
	pthread_mutex_t mutex_status[arg.number_of_philo];
	pthread_mutex_t mutex_last_eat[arg.number_of_philo];
	pthread_mutex_t print;
	general_init(&general, &print);
	while (i < arg.number_of_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&mutex_status[i], NULL);
		pthread_mutex_init(&mutex_last_eat[i], NULL);
		i++;
	}
	i = 0;
	while (i < arg.number_of_philo)
	{
		philo_init2(&philo[i], arg, &general, &mutex_status[i], &mutex_last_eat[i]);
		philo_init(&philo[i], i, &forks[(i + 1) % arg.number_of_philo], &forks[i]);
		i = i + 2;
	}

	i = 1;
	while (i < arg.number_of_philo)
	{
		philo_init2(&philo[i], arg, &general, &mutex_status[i], &mutex_last_eat[i]);
		philo_init(&philo[i], i, &forks[i], &forks[(i + 1) % arg.number_of_philo]);
		i = i + 2;
	}
	
	i = 0;
	while(i < arg.number_of_philo)
	{
		ret = pthread_create(&thread[i], NULL,&func, &philo[i]);
		r = pthread_create(&monitor[i], NULL,&func2, &philo[i]);
		if(ret != 0 || r != 0)
		{
			printf("ERROR while creating threads");
			return 1;
		}
		i += 2;
	}
	i = 1;
	while(i < arg.number_of_philo)
	{
		ret = pthread_create(&thread[i], NULL,&func, &philo[i]);
		r = pthread_create(&monitor[i], NULL,&func2, &philo[i]);
		if(ret != 0 || r != 0)
		{
			printf("ERROR while creating threads");
			return 1;
		}
		i += 2;
	}
	
	i = 0;
	while (i < arg.number_of_philo)
	{
		ret = pthread_join(thread[i], NULL);
		pthread_join(monitor[i], NULL);
		if (ret != 0)
		{
			printf("ERROR while threads join");
			return 1;
		}
		i++;
	}
	return 0;
}