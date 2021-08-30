#include "philo.h"

void	*eat_timer(void *params)
{
	int			elapsed;
	t_params	*p;
	int			i;

	p = (t_params *)params;
	while (1)
	{
		elapsed = get_elapsed(p);
		i = 0;
		while (i < p->nb_of_philo)
		{
			if (elapsed >= p->times_of_death[i])
			{
				pthread_mutex_lock(&p->death_mutex);
				print_actions(i + 1, "died", p);
				p->philo_is_dead = 1;
				pthread_mutex_unlock(&p->death_mutex);
				return (NULL);
			}
			i++;
		}
		usleep(100);
	}
}

int	eating(t_philo *ph)
{
	pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork1]));
	if (!print_actions(ph->philo, "has taken a fork", ph->p))
	{
		pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
		return (0);
	}
	pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork2]));
	if (!print_actions(ph->philo, "has taken a fork", ph->p))
	{
		pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
		pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
		return (0);
	}
	if (!print_actions(ph->philo, "is eating", ph->p))
		return (0);
	my_usleep(ph->time_to_eat);
	if (ph->p->nb_of_eats > -1)
		check_meals(ph);
	ph->p->times_of_death[ph->philo - 1] = get_elapsed(ph->p) + ph->time_to_die;
	pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
	pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
	return (1);
}

int	sleeping_thinking(t_philo *ph)
{
	if (!print_actions(ph->philo, "is sleeping", ph->p))
		return (0);
	my_usleep(ph->time_to_sleep);
	if (!print_actions(ph->philo, "is thinking", ph->p))
		return (0);
	return (1);
}

void	*simulation(void *philo)
{
	t_philo		*ph;
	pthread_t	hunger_th;

	ph = (t_philo *)philo;
	if (ph->philo % 2 == 0)
		my_usleep(ph->time_to_sleep / 2);
	if (ph->philo == 1)
	{
		gettimeofday(&ph->p->begin, NULL);
		if (pthread_create(&hunger_th, NULL, &eat_timer, (void *)ph->p))
			return (0);
		pthread_detach(hunger_th);
	}
	while (1)
	{
		if (ph->p->philo_is_dead)
			return (NULL);
		if (!eating(ph))
			return (NULL);
		if (ph->p->philo_is_dead)
			return (NULL);
		if (!sleeping_thinking(ph))
			return (NULL);
	}
	return (NULL);
}

int	start_simulation(t_params *p)
{
	t_philo		**ph_table;
	pthread_t	*th;
	int			i;

	ph_table = NULL;
	th = NULL;
	if (!alloc_threads(&ph_table, &th, p))
		return (error_free("Could not allocate threads", th, p, ph_table));
	if (!init_mutexes(p))
		return (error_free("Error while init mutexes", th, p, ph_table));
	if (!create_all_philos(p, ph_table, th))
		return (0);
	i = 0;
	while (i < p->nb_of_philo)
	{
		if (pthread_join(th[i++], NULL))
			return (error_free("Error while joining threads", th, p, ph_table));
	}
	usleep(100000);
	if (!destroy_mutexes(p, ph_table))
		return (error_free("Error while destroying mutexes", th, p, ph_table));
	free_all(p, th, ph_table);
	return (1);
}
