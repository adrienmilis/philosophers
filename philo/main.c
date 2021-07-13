#include "philo.h"

int	error_free(char *error_msg, pthread_t *th, t_params *p)
{
	printf("%s\n", error_msg);
	free(th);
	free(p->mtx_forks);
	return (0);
}

void	*func(void *params)
{
	// all philosophers are created
	// now in this function they will try to take two forks. If they can't, they should wait until they are available.
	t_params	*p;
	
	p = (t_params*)params;
	// fork n et n-1

	pthread_mutex_lock(&p->mtx_fork);
	pthread_mutex_unlock(&p->mtx_fork)

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

int	start_simulation(t_params *p)
{
	pthread_t	*th;
	int			i;

	i = 0;
	th = malloc(sizeof(pthread_t) * p->nb_of_philo);
	if (!th)
		return (0);
	if (!init_mutexes(p))
		return (error_free("Error while init mutexes", th, p));
	while (i < p->nb_of_philo)
	{
		if (pthread_create(th + i++, NULL, &func, (void *)p))	// returns 0 if no error
			return (error_free("Error while creating threads", th, p));
	}
	i = 0;
	while (i < p->nb_of_philo)
	{
		if (pthread_join(th[i++], NULL))
			return (error_free("Error while joining threads", th, p));
	}
	if (!destroy_mutexes(p))
		return (error_free("Error while destroying mutexes", th, p));
	return (1);
}

int	main(int argc, char **argv)
{
	t_params	p;

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
	memset(p.forks, 0, sizeof(int) * p.nb_of_philo);
	start_simulation(&p);
	return (0);
}
