#include "philo.h"

void	*eat_timer(void *philo)
{
	int	elapsed;
	t_philo	*ph;

	ph = (t_philo *)philo;
	while (1)
	{
		elapsed = get_elapsed(ph);
		if (elapsed >= ph->time_of_death)
		{
			pthread_mutex_lock(&ph->p->death_mutex);
			print_actions(ph, "died", 0);
			ph->p->philo_is_dead = 1;
			// usleep(1000000);
			pthread_mutex_unlock(&ph->p->death_mutex);
			pthread_mutex_unlock(&ph->p->print_mutex);
			return (NULL);
		}
		usleep(50);
	}
}

int	eating(t_philo *ph)
{
	pthread_t	hunger_th;
	static int	i;

	if (ph->philo % 2 == 0)
	{
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork1]));
		if (!print_actions(ph, "has taken a fork", 1))
		{
			pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
			return (0);
		}
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork2]));
		if (!print_actions(ph, "has taken a fork", 1))
		{
			pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
			pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
			return (0);
		}
	}
	else
	{
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork2]));
		if (!print_actions(ph, "has taken a fork", 1))
		{
			pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
			return (0);
		}
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork1]));
		if (!print_actions(ph, "has taken a fork", 1))
		{
			pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
			pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
			return (0);
		}
	}
	if (!print_actions(ph, "is eating", 1))
		return (0);
	my_usleep(ph->time_to_eat);
	pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
	pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
	// philo has finished eating - start a thread
	ph->time_of_death = get_elapsed(ph) + ph->time_to_die;
	if (i == 0)
	{
		if (pthread_create(&hunger_th, NULL, &eat_timer, (void *)ph))
			return (0);
		pthread_detach(hunger_th);
		i = 1;
	}
	return (1);
}

int	sleeping_thinking(t_philo *ph)
{
	if (ph->philo == 1)
	{
		if (!print_actions(ph, "is sleeping", 1))
			return (0);
		my_usleep(ph->time_to_sleep);
		if (!print_actions(ph, "is thinking", 1))
			return (0);
	}
	return (1);
}

void	*simulation(void *philo)
{
	t_philo	*ph;

	ph = (t_philo*)philo;
	gettimeofday(&ph->begin, NULL);
	while (1)
	{
		if (ph->p->philo_is_dead)
		{
			usleep(10000);
			return (NULL);
		}
		if (!eating(ph))
		{
			usleep(10000);
			return (NULL);
		}
		if (ph->p->philo_is_dead)
		{
			usleep(10000);
			return (NULL);
		}
		if (!sleeping_thinking(ph))
		{
			usleep(10000);
			return (NULL);
		}
	}
	return (NULL);
}

int	start_simulation(t_params *p)
{
	t_philo		**ph_table;
	pthread_t	*th;
	int			i;
	t_philo		*ph;

	ph_table = NULL;
	th = NULL;
	i = 0;
	if (!alloc_threads(&ph_table, &th, p))
		return (error_free("Could not allocate threads", th, p, ph_table));
	if (!init_mutexes(p))
		return (error_free("Error while init mutexes", th, p, ph_table));
	while (i < p->nb_of_philo)
	{
		ph = create_philosopher(p, th, i);
		if (!ph)
			return (error_free("Error while creating threads", th, p, ph_table));
		ph_table[i++] = ph;
	}
	i = 0;
	while (i < p->nb_of_philo)
	{
		if (pthread_join(th[i], NULL))
			return (error_free("Error while joining threads", th, p, ph_table));
		i++;
	}
	if (!destroy_mutexes(p))
		return (error_free("Error while destroying mutexes", th, p, ph_table));
	free_all(p, th, ph_table);
	return (1);
}

int main(int argc, char **argv)
{
	t_params	p;

	p.philo_is_dead = 0;
	p.mtx_forks = NULL;
	if (argc < 5 || argc > 6)
	{
		printf("Unvalid number of arguments\n");
		return (1);
	}
	if (!parse_parameters(argv, &p, argc - 1))
	{
		printf("Argument is not an int\n");
		return (1);
	}
	start_simulation(&p);
	return (0);
}