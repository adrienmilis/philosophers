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

void	my_usleep(int time, int nb_of_philo)
{
	struct timeval	begin;
	struct timeval	curr;
	double			elapsed;
	float			constant;

	constant = 0.1;
	if (nb_of_philo > 50 && nb_of_philo <= 100)
		constant = 0.25;
	else if (nb_of_philo > 100 && nb_of_philo <= 150)
		constant = 0.4;
	else if (nb_of_philo > 150)
		constant = 0.8;
	elapsed = 0;
	gettimeofday(&begin, NULL);
	while (elapsed < (float)time - constant)
	{
		usleep(100);
		gettimeofday(&curr, NULL);
		elapsed = ((curr.tv_sec - begin.tv_sec) * 1000.0)
			+ (((curr.tv_usec - begin.tv_usec)) / 1000.0);
	}
}
