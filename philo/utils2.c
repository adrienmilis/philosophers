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
	double			elapsed;

	elapsed = 0;
	gettimeofday(&begin, NULL);	// proteger
	while (elapsed < (float)time - 0.0425)
	{
		usleep(40);
		gettimeofday(&curr, NULL);
		elapsed = ((curr.tv_sec - begin.tv_sec) * 1000.0) + (((curr.tv_usec - begin.tv_usec)) / 1000.0);
	}
}