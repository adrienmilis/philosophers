#include "philo.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	write(1, str,ft_strlen(str));
}

// void	ft_putnbr_pos(unsigned long long n)
// {
// 	if (n < 10)
// 		ft_putchar(48 + n);
// 	else
// 	{
// 		ft_putnbr_pos(n / 10);
// 		ft_putnbr_pos(n % 10);
// 	}
// }

void	print_actions(t_philo *ph, char *str)
{
	pthread_mutex_lock(&ph->p->print_mutex);
	// if (!ph->p->philo_is_dead)
	// 	ft_putnbr_pos(get_elapsed(ph));
	// if (!ph->p->philo_is_dead)
	// 	ft_putchar(' ');
	// if (!ph->p->philo_is_dead)
	// 	ft_putnbr_pos(ph->philo);
	// if (!ph->p->philo_is_dead)
	// 	ft_putchar(' ');
	// if (!ph->p->philo_is_dead)
	// 	ft_putstr(str);
	// if (!ph->p->philo_is_dead)
	// 	ft_putchar('\n');
	printf("%d %d %s\n", get_elapsed(ph), ph->philo, str);
	pthread_mutex_unlock(&ph->p->print_mutex);
}