#include "philo.h"

/* 	This function checks if philosophers eat in time. If not, they die.
	It gets the elapsed time since beginning and checks if a philo should be dead by this time
	by comparing this time with a table containing the times of death of each philo */	

void	*eat_timer(void *params)
{
	int		elapsed;
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

	// t_philo	*ph;
	// ph = (t_philo *)philo;
	// while (1)
	// {
	// 	elapsed = get_elapsed(ph);
	// 	pthread_mutex_lock(&ph->tod_mutex);
	// 	if (elapsed >= ph->time_of_death)
	// 	{
	// 		pthread_mutex_lock(&ph->p->death_mutex);
	// 		print_actions(ph, "died", 0);
	// 		ph->p->philo_is_dead = 1;
	// 		// usleep(1000000);
	// 		pthread_mutex_unlock(&ph->p->death_mutex);
	// 		pthread_mutex_unlock(&ph->p->print_mutex);
	// 		pthread_mutex_unlock(&ph->tod_mutex);
	// 		return (NULL);
	// 	}
	// 	pthread_mutex_unlock(&ph->tod_mutex);
	// 	usleep(50);
	// }
}

int	tab_is_full(int *tab, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (tab[i] == 0)
			return (0);
		i++;
	}
	return (1);
}

int	eating(t_philo *ph)
{
	// pthread_t	hunger_th;

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
	// else
	// {
	// 	pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork2]));
	// 	if (!print_actions(ph->philo, "has taken a fork", ph->p))
	// 	{
	// 		pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
	// 		return (0);
	// 	}
	// 	pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork1]));
	// 	if (!print_actions(ph->philo, "has taken a fork", ph->p))
	// 	{
	// 		pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
	// 		pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);
	// 		return (0);
	// 	}
	// }
	if (!print_actions(ph->philo, "is eating", ph->p))
		return (0);
	my_usleep(ph->time_to_eat);
	if (ph->p->nb_of_eats > -1 && ph->p->philos_full[ph->philo - 1] == 0)
	{
		ph->nb_of_meals++;
		if (ph->nb_of_meals >= ph->p->nb_of_eats)
		{
			ph->p->philos_full[ph->philo - 1] = 1;
			if (tab_is_full(ph->p->philos_full, ph->p->nb_of_philo))
			{
				pthread_mutex_lock(&ph->p->death_mutex);
				ph->p->philo_is_dead = 1;
				pthread_mutex_unlock(&ph->p->death_mutex);
			}
		}
	}
	ph->p->times_of_death[ph->philo - 1] = get_elapsed(ph->p) + ph->time_to_die;
	pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork1]);
	pthread_mutex_unlock(&ph->p->mtx_forks[ph->fork2]);

	// pthread_mutex_lock(&ph->tod_mutex);
	// ph->time_of_death = get_elapsed(ph->p) + ph->time_to_die;

	// pthread_mutex_unlock(&ph->tod_mutex);
	// if (i == 0)
	// {
	// 	if (pthread_create(&hunger_th, NULL, &eat_timer, (void *)ph))
	// 		return (0);
	// 	pthread_detach(hunger_th);
	// 	i = 1;
	// }
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
	// checker avant 
}

void	*simulation(void *philo)
{
	t_philo		*ph;
	pthread_t	hunger_th;

	ph = (t_philo*)philo;
	if (ph->philo % 2 == 0)
		my_usleep(ph->time_to_sleep / 2);
	if (ph->philo == 1)
	{
		gettimeofday(&ph->p->begin, NULL);
		if (pthread_create(&hunger_th, NULL, &eat_timer, (void *)ph->p))
			return (0);
		pthread_detach(hunger_th);
	}
	// if (pthread_create(&hunger_th, NULL, &eat_timer, (void *)ph))
	// 		return (0);
	// pthread_detach(hunger_th);
	while (1)
	{
		// pthread_mutex_lock(&ph->p->death_mutex);
		if (ph->p->philo_is_dead)
			return (NULL);
		// pthread_mutex_unlock(&ph->p->death_mutex);
		if (!eating(ph))
			return (NULL);
		// pthread_mutex_lock(&ph->p->death_mutex);
		if (ph->p->philo_is_dead)
			return (NULL);
		// pthread_mutex_unlock(&ph->p->death_mutex);
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
	usleep(100000);
	if (!destroy_mutexes(p, ph_table))
		return (error_free("Error while destroying mutexes", th, p, ph_table));
	free_all(p, th, ph_table);
	return (1);
}

int	init_times_of_death(t_params *p)
{
	int	i;

	p->philos_full = NULL;
	p->times_of_death = malloc(sizeof(int) * p->nb_of_philo);
	if (!p->times_of_death)
	{
		printf("Malloc error\n");
		return (0);
	}
	i = 0;
	while (i < p->nb_of_philo)
	{
		p->times_of_death[i] = p->time_to_die;
		i++;
	}
	p->philos_full = malloc(sizeof(int) * p->nb_of_philo);
	if (!p->philos_full)
	{
		printf("Malloc error\n");
		return (0);
	}
	memset(p->philos_full, 0, sizeof(int) * p->nb_of_philo);
	return (1);
}

int main(int argc, char **argv)
{
	t_params	p;

	p.philo_is_dead = 0;
	p.mtx_forks = NULL;
	p.nb_of_eats = -1;
	if (argc < 5 || argc > 6)
	{
		printf("Unvalid number of arguments\n");
		return (1);
	}
	if (!parse_parameters(argv, &p, argc - 1))
	{
		printf("Argument is invalid\n");
		return (1);
	}
	// first malloc
	if (!init_times_of_death(&p))
		return (1);
	if (p.nb_of_eats != 0)
		start_simulation(&p);
	return (0);
}