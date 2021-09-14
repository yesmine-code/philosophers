#include "include/philosophers.h"

void ft_free(t_list *list)
{
	t_list *tmp;
	
	tmp = list;
	while(tmp != NULL && tmp->content != NULL)
	{
		free(tmp->content);
		tmp = tmp->next;
	}
	if (list != NULL)
		free(list);
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
	t_list *tmp;

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

void create_philo_list(t_arguments arg, t_general *general, t_list **philo, t_list *forks)
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
		philo_init(new_philo->content, i,get_from_list_by_id(forks, min(i, (i + 1) % arg.number_of_philo)), get_from_list_by_id(forks, max(i, (i + 1) % arg.number_of_philo)));
		ft_lstadd_back(philo, new_philo);;
		i++;
	}
}
void	create_forks_list(t_arguments arg, t_list **forks)
{
	t_list			*new_fork;
	pthread_mutex_t	*mutex_fork;
	int 			i;

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

void	create_threads_and_monitors_lists(t_arguments arg, t_list **threads, t_list **monitors)
{
	t_list			*new_thread;
	pthread_t		*thread;
	t_list			*new_monitor;
	pthread_t		*monitor;
	int 			i;

	i = 0;
	while (i < arg.number_of_philo)
	{
		new_monitor = malloc(sizeof(t_list));
		new_thread = malloc(sizeof(t_list));
		thread = malloc(sizeof(pthread_t));
		monitor = malloc(sizeof(pthread_t));
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

