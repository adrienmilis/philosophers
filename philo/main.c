#include "philo.h"

void	free_all(t_params *p, pthread_t *th, t_philo **ph_table)
{
	int	i;

	i = 0;
	if (ph_table)
	{
		while (i < p->nb_of_philo)
			free(ph_table[i++]);
		free(ph_table);
	}
	free(th);
	free(p->mtx_forks);
	free(p->forks);
}

int	error_free(char *error_msg, pthread_t *th, t_params *p, t_philo **ph_table)
{
	ft_putstr(error_msg);
	ft_putchar('\n');
	free_all(p, th, ph_table);
	return (0);
}

int	get_elapsed(t_philo *ph)
{
	struct timeval	curr;
	int				elapsed;

	gettimeofday(&curr, NULL);
	elapsed = (curr.tv_sec - ph->begin.tv_sec) * 1000.0 + (curr.tv_usec - ph->begin.tv_usec) / 1000.0;
	return (elapsed);
}

void	print_actions(t_philo *ph, char *str)
{
	pthread_mutex_lock(&ph->p->print_mutex);
	ft_putnbr_pos(get_elapsed(ph));
	ft_putchar(' ');
	ft_putnbr_pos(ph->philo);
	ft_putchar(' ');
	ft_putstr(str);
	ft_putchar('\n');
	pthread_mutex_unlock(&ph->p->print_mutex);
}

void	my_usleep(int time)
{
	struct timeval	begin;
	struct timeval	curr;
	int				elapsed;

	elapsed = 0;
	gettimeofday(&begin, NULL);	// proteger
	while (elapsed < time)
	{
		usleep(50);
		gettimeofday(&curr, NULL);
		elapsed = ((curr.tv_sec - begin.tv_sec) * 1000) + (((curr.tv_usec - begin.tv_usec)) / 1000);
	}
}

void	*take_forks(void *philo)
{
	t_philo			*ph;

	ph = (t_philo*)philo;
	gettimeofday(&ph->begin, NULL);
	if (ph->philo % 2 == 0)
	{
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork1]));
		print_actions(ph, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork2]));
		print_actions(ph, "has taken a fork");
	}
	if (ph->philo % 2 == 0)
	{
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork2]));
		print_actions(ph, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&(ph->p->mtx_forks[ph->fork1]));
		print_actions(ph, "has taken a fork");
	}
	print_actions(ph, "is eating");
	my_usleep(ph->p->time_to_eat);
	pthread_mutex_unlock(&(ph->p->mtx_forks[ph->fork1]));
	pthread_mutex_unlock(&(ph->p->mtx_forks[ph->fork2]));
	return (NULL);
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
	return (1);
}

int	destroy_mutexes(t_params *p)
{
	int	i;

	i = 0;
	while (i < p->nb_of_philo)
	{
		if (pthread_mutex_destroy(&p->mtx_forks[i]))
			return (0);
		i++;
	}
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
	if (pthread_create(th + i, NULL, &take_forks, (void *)ph))
		return (NULL);
	return (ph);
}

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

int	start_simulation(t_params *p)
{
	t_philo		**ph_table;
	t_philo		*ph;
	pthread_t	*th;
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
		ph = create_philosopher(p, th, i);	// proteger
		if (!ph)
			return (error_free("Error while creating threads", th, p, ph_table));
		ph_table[i++] = ph;
	}
	i = 0;
	while (i < p->nb_of_philo)
	{
		if (pthread_join(th[i++], NULL))
			return (error_free("Error while joining threads", th, p, ph_table));
	}
	if (!destroy_mutexes(p))
		return (error_free("Error while destroying mutexes", th, p, ph_table));
	free_all(p, th, ph_table);
	return (1);
}

int	main(int argc, char **argv)
{
	t_params		p;
	
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
