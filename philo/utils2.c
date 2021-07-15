#include "philo.h"

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