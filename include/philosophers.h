/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybesbes <ybesbes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 11:11:11 by ybesbes           #+#    #+#             */
/*   Updated: 2021/09/23 11:11:13 by ybesbes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

enum e_status
{
	eating = 1,
	sleeping = 2,
	thinking = 3,
	done	= 4
};

typedef struct s_arguments
{
	int	number_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
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
	int					number_of_time_has_ate;
	long long			time_last_eat;
	t_general			*general;

}		t_philosoher;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

long long	current_timestamp(long long start_time);
void		print(char *str, t_philosoher philo, int dead);
int			mini(int a, int b);
int			maxi(int a, int b);
void		ft_free(t_list *list);
int			check_arg(char *arg);
int			ft_is_digit(char c);
int			ft_strlen(char *str);
int			ft_atoi(const char *nptr);
void		ft_lstadd_back(t_list **alst, t_list *new);
void		create_forks_list(t_arguments arg, t_list **forks);
void		create_philo_list(t_arguments arg, t_general *general,
				t_list **philo, t_list *forks);
void		*get_from_list_by_id(t_list *list, int id);
void		create_threads_and_monitors_lists(t_arguments arg,
				t_list **threads, t_list **monitors);
void		philo_init(t_philosoher *philo, int i, pthread_mutex_t *fork_1,
				pthread_mutex_t *fork_2);
long long	current_timestamp(long long start_time);
void		general_init(t_general *general, pthread_mutex_t *print);
void		philo_init2(t_philosoher *philo, t_arguments args,
				t_general *general);
float		time_diff(struct timeval *start, struct timeval *end);
void		print(char *str, t_philosoher philo, int dead);
void		*func2(void *input);
void		*func(void *input);
void		free_all(t_list **forks, t_list **monitors,
				t_list **threads, t_list **philo);
int			manage_error(int ac);

#endif
