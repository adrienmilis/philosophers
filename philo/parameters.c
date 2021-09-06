#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

static int	is_space(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(char *str, int *param)
{
	int					i;
	unsigned long long	nb;

	nb = 0;
	i = 0;
	if (!only_digits(str))
		return (0);
	while (is_space(str[i]) && str[i])
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb *= 10;
		nb += str[i++] - 48;
	}
	if (nb >= 0 && nb <= INT_MAX)
		*param = (int)(nb);
	else
		return (0);
	return (1);
}

int	parse_parameters(char **argv, t_params *p, int nb_of_params)
{
	if (!ft_atoi(argv[1], &p->nb_of_philo))
		return (0);
	if (!ft_atoi(argv[2], &p->time_to_die))
		return (0);
	if (!ft_atoi(argv[3], &p->time_to_eat))
		return (0);
	if (!ft_atoi(argv[4], &p->time_to_sleep))
		return (0);
	if (nb_of_params == 5)
	{
		if (!ft_atoi(argv[5], &p->nb_of_eats))
			return (0);
	}
	return (1);
}
