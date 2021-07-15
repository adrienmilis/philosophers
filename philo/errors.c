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