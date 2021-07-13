#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_params
{
	int				nb_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_of_eats;
	int				*forks;
	pthread_mutex_t	*mtx_forks;
}				t_params;

typedef struct s_philo
{
	
}				t_philo;

// parameters.c
int	ft_strlen(char *str);
int	nb_len(int nb);
int	ft_atoi(char *str, int *nb);
int	parse_parameters(char **argv, t_params *p, int nb_of_params);

#endif