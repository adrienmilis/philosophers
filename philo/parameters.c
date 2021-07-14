#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

int	nb_len(int nb)
{
	int	len;

	if (nb == 0)
		return (1);
	if (nb < 0)
	{
		len = 1;
		nb = -nb;
	}
	else
		len = 0;
	while (nb)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

int	ft_atoi(char *str, int *nb)
{
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	*nb = 0;
	if (str[0] == '-')
		sign = -1;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		*nb *= 10;
		*nb += str[i++] - 48;
	}
	if (str[i] || (ft_strlen(str) != nb_len(*nb)))
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