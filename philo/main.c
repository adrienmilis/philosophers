#include "philo.h"

int	error_free(char *error_msg, pthread_t *th)
{
	printf("%s\n", error_msg);
	free(th);
	return (0);
}

void	*func(void *params)
{
	// all philosophers are created
	// now in this function they will try to take two forks. If they can't, they should wait until they are available.
	int				*forks;
	t_params		*p;
	pthread_mutex_t	mutex;
	
	p = (t_params*)params;

	
	return (NULL);
}

int	start_simulation(t_params *p)
{
	pthread_t	*th;
	int			i;

	i = 0;
	th = malloc(sizeof(pthread_t) * p->nb_of_philo);
	if (!th)
		return (0);
	while (i < p->nb_of_philo)
	{
		if (pthread_create(th + i, NULL, &func, (void *)p))	// returns 0 if no error
			return (error_free("Error while creating threads", th));
		i++;
	}
	i = 0;
	while (i < p->nb_of_philo)
	{
		if (pthread_join(th[i], NULL))
			return (error_free("Error while joining threads", th));
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_params	p;

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
