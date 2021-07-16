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
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;

	int				philo_is_dead;
}				t_params;

typedef struct s_philo
{
	int				philo;
	int				fork1;
	int				fork2;
	t_params		*p;
	struct timeval	begin;
	int				time_of_death;
}				t_philo;

// parameters.c
int	ft_strlen(char *str);
int	nb_len(int nb);
int	ft_atoi(char *str, int *nb);
int	parse_parameters(char **argv, t_params *p, int nb_of_params);

// utils.c
void	ft_putstr(char *str);
void	ft_putchar(char c);
void	ft_putnbr_pos(unsigned long long n);
void	print_actions(t_philo *ph, char *str);

// utils2.c
int		get_elapsed(t_philo *ph);
void	my_usleep(int time);

// errors.c
void	free_all(t_params *p, pthread_t *th, t_philo **ph_table);
int		error_free(char *error_msg, pthread_t *th, t_params *p, t_philo **ph_table);

// create_threads.c
int		init_mutexes(t_params *p);
int		destroy_mutexes(t_params *p);
t_philo	*create_philosopher(t_params *p, pthread_t *th, int i);
int		alloc_threads(t_philo ***ph_table, pthread_t **th, t_params *p);

// main.c
void	*simulation(void *philo);
int		start_simulation(t_params *p);

#endif