#include "philo.h"

int	alloc_threads(t_philo ***ph_table, pthread_t **th, t_params *p)
{
	*ph_table = malloc(sizeof(t_philo *) * p->nb_of_philo);
	if (!ph_table)
		return (0);
	*th = malloc(sizeof(pthread_t) * p->nb_of_philo);
	if (!th)
		return (0);
	return (1);
}

int	init_mutexes(t_params *p)
{
	int	i;

	i = 0;
	p->mtx_forks = malloc(sizeof(pthread_mutex_t) * p->nb_of_philo);
	if (!p->mtx_forks)
		return (0);
	while (i < p->nb_of_philo)
	{
		if (pthread_mutex_init(&p->mtx_forks[i], NULL))
			return (0);
		i++;
	}
	if (pthread_mutex_init(&p->print_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&p->death_mutex, NULL))
		return (0);
	return (1);
}

t_philo	*create_philosopher(t_params *p, pthread_t *th, int i)
{
	t_philo	*ph;

	ph = malloc(sizeof(t_philo));
	ph->philo = i + 1;
	if (i - 1 == -1)
		ph->fork1 = p->nb_of_philo - 1;
	else
		ph->fork1 = i - 1;
	ph->fork2 = i;
	ph->p = p;
	ph->time_to_die = p->time_to_die;
	ph->time_to_eat = p->time_to_eat;
	ph->time_to_sleep = p->time_to_sleep;
	ph->nb_of_eats = p->nb_of_eats;
	if (pthread_create(th + i, NULL, &simulation, (void *)ph))
		return (NULL);
	return (ph);
}

int	destroy_mutexes(t_params *p)
{
	int	i;
	int ret;

	i = 0;
	while (i < p->nb_of_philo)
	{
		if (pthread_mutex_destroy(&p->mtx_forks[i]))
		{
			printf("fork mutex\n");
			return (0);
		}
		i++;
	}
	if ((ret = pthread_mutex_destroy(&p->print_mutex)))
	{
		printf("print mutex\n");
		return (0);
	}
	if (pthread_mutex_destroy(&p->death_mutex))
	{
		printf("death mutex\n");
		return (0);
	}
	return (1);
}
