#include "philo.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	write(1, str,ft_strlen(str));
}

int	print_actions(t_philo *ph, char *str, int unlock)
{
	pthread_mutex_lock(&ph->p->print_mutex);
	if (ph->p->philo_is_dead)
	{
		pthread_mutex_unlock(&ph->p->print_mutex);
		return (0);
	}
	printf("%d %d %s\n", get_elapsed(ph), ph->philo, str);
	if (unlock)
		pthread_mutex_unlock(&ph->p->print_mutex);
	return (1);
}

int	get_elapsed(t_philo *ph)
{
	struct timeval	curr;
	int				elapsed;

	gettimeofday(&curr, NULL);
	elapsed = (curr.tv_sec - ph->begin.tv_sec) * 1000.0 + (curr.tv_usec - ph->begin.tv_usec) / 1000.0;
	return (elapsed);
}

void	my_usleep(int time)
{
	struct timeval	begin;
	struct timeval	curr;
	double			elapsed;

	elapsed = 0;
	gettimeofday(&begin, NULL);	// proteger
	while (elapsed < (float)time - 0.065)
	{
		usleep(100);
		gettimeofday(&curr, NULL);
		elapsed = ((curr.tv_sec - begin.tv_sec) * 1000.0) + (((curr.tv_usec - begin.tv_usec)) / 1000.0);
	}
}
