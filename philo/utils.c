#include "philo.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	write(1, str,ft_strlen(str));
}

void	ft_putnbr_pos(unsigned long long n)
{
	if (n < 10)
		ft_putchar(48 + n);
	else
	{
		ft_putnbr_pos(n / 10);
		ft_putnbr_pos(n % 10);
	}
}
