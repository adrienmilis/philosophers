#include "philo.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

int	print_actions(int philo_nb, char *str, t_params *p)
{
	pthread_mutex_lock(&p->print_mutex);
	if (p->philo_is_dead)
	{
		pthread_mutex_unlock(&p->print_mutex);
		return (0);
	}
	printf("%d %d %s\n", get_elapsed(p), philo_nb, str);
	pthread_mutex_unlock(&p->print_mutex);
	return (1);
}

int	get_elapsed(t_params *p)
{
	struct timeval	curr;
	int				elapsed;

	gettimeofday(&curr, NULL);
	elapsed = (curr.tv_sec - p->begin.tv_sec) * 1000.0
		+ (curr.tv_usec - p->begin.tv_usec) / 1000.0;
	return (elapsed);
}

void	my_usleep(int time)
{
	struct timeval	begin;
	struct timeval	curr;
	double			elapsed;

	elapsed = 0;
	gettimeofday(&begin, NULL);
	while (elapsed < (float)time - 0.10)
	{
		usleep(100);
		gettimeofday(&curr, NULL);
		elapsed = ((curr.tv_sec - begin.tv_sec) * 1000.0)
			+ (((curr.tv_usec - begin.tv_usec)) / 1000.0);
	}
}
