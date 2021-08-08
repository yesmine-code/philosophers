#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct s_arguments
{
	int	number_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int time_to_sleep;
	int	number_of_time_each_philosopher_must_eat;
}		t_arguments;

typedef struct s_general
{
	pthread_mutex_t		*print;
	long long			start_time;
	int					one_has_died;
}		t_general;

typedef struct s_philosopher
{
	t_arguments			args;
	int					id;
	int					status;
	pthread_mutex_t		*fork_1;
	pthread_mutex_t		*fork_2;
	pthread_mutex_t		*mutex_status;
	pthread_mutex_t		*mutex_last_eat;
	int					number_of_time_has_ate;
	long long			time_last_eat;
	t_general			*general;

}		t_philosoher;




int check_arg(char *arg);
int ft_is_digit(char c);
int ft_strlen(char *str);
int	ft_atoi(const char *nptr);






#endif
