#include "philo.h"

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

void	check_meals(t_philo *ph)
{
	if (ph->p->philos_full[ph->philo - 1] == 0)
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
}

int	create_all_philos(t_params *p, t_philo **ph_tab, pthread_t *th)
{
	int		i;
	t_philo	*ph;

	i = 0;
	while (i < p->nb_of_philo)
	{
		ph = create_philosopher(p, th, i);
		if (!ph)
			return (error_free("Error while creating thrds", th, p, ph_tab));
		ph_tab[i] = ph;
		i++;
	}
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

int	main(int argc, char **argv)
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
	if (p.nb_of_eats == 0)
		return (0);
	if (!init_times_of_death(&p))
		return (1);
	start_simulation(&p);
	return (0);
}
