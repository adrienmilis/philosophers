#include "philo.h"

/* pour checker si un philo va die : lancer un thread dans lequel on time et on check la variable de quand le philo devrait die, qui 
	est actualisee chaque fois qu'il mange */

/* pour checker si un philo est dead : checker avec un usleep dans un autre thread (detached) si la variable a change : si oui, exit le thread 
	ce qui devrait (?) terminerl e programme */

void	*eat_timer(void *philo)
{
	int	elapsed;
	t_philo	*ph;

	ph = (t_philo *)philo;
	while (1)
	{
		// if elapsed >= time_of_death, set philo_is_dead
		elapsed = get_elapsed(ph);
		if (elapsed >= ph->time_of_death)
		{
			// + mutex
			pthread_mutex_lock(&ph->p->death_mutex);
			print_actions(ph, "died");
			ph->p->philo_is_dead = 1;
			pthread_mutex_unlock(&ph->p->death_mutex);
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
		print_actions(ph, "has taken a fork");
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork2]));
		print_actions(ph, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork2]));
		print_actions(ph, "has taken a fork");
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork1]));
		print_actions(ph, "has taken a fork");
	}
	pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
	pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
	print_actions(ph, "is eating");
	my_usleep(ph->p->time_to_eat);
	// philo has finished eating - start a thread
	ph->time_of_death = get_elapsed(ph) + ph->p->time_to_die;
	if (i == 0)
	{
		if (pthread_create(&hunger_th, NULL, &eat_timer, (void *)ph))
			return (0);
		pthread_detach(hunger_th);
		i = 1;
	}
	return (1);
}

void	sleeping_thinking(t_philo *ph)
{
	if (ph->philo == 1)
	{
		print_actions(ph, "is sleeping");
		my_usleep(ph->p->time_to_sleep);
		print_actions(ph, "is thinking");
	}
}

void	*simulation(void *philo)
{
	t_philo			*ph;

	ph = (t_philo*)philo;
	gettimeofday(&ph->begin, NULL);
	while (1)
	{
		eating(ph);
		sleeping_thinking(ph);
	}
	return (NULL);
}

void	*monitor_death(void	*parameters)
{
	t_params	*p;

	p = (t_params *)parameters;
	while (1)
	{
		if (p->philo_is_dead)
			return (NULL);
		usleep(50);
	}
}

int	start_simulation(t_params *p)
{
	t_philo		**ph_table;
	t_philo		*ph;
	pthread_t	*th;
	pthread_t	monitor;
	int			i;

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

	// thread monitoring the death of a philosopher
	if (pthread_create(&monitor, NULL, &monitor_death, (void *)p))
		return (error_free("Error while creating thread", th, p, ph_table));
	if (pthread_join(monitor, NULL))
		return (error_free("Error while joining threads", th, p, ph_table));

	if (!destroy_mutexes(p))
		return (error_free("Error while destroying mutexes", th, p, ph_table));
	free_all(p, th, ph_table);
	return (1);
}

int	main(int argc, char **argv)
{
	t_params		p;
	
	p.philo_is_dead = 0;
	p.mtx_forks = NULL;
	if (argc < 5 || argc > 6)
	{
		printf("Unvalid number of arguments\n");
		return (1);
	}
	if (!parse_parameters(argv, &p, argc - 1))
	{
		printf("Argument is not an int\n");	// erreur de malloc aussi possible
		return (1);
	}
	p.forks = malloc(sizeof(int) * p.nb_of_philo);
	if (!p.forks)
		return (0);
	memset(p.forks, 0, sizeof(int) * p.nb_of_philo);
	start_simulation(&p);
	return (0);
}
