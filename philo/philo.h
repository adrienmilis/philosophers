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
	pthread_mutex_t	*mtx_forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	int				philo_is_dead;
	struct timeval	begin;
	int				*times_of_death;
	int				*philos_full;
}				t_params;

typedef struct s_philo
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_of_eats;

	int				philo;
	int				fork1;
	int				fork2;
	t_params		*p;
	int				time_of_death;
	pthread_mutex_t	tod_mutex;	// mutex for time of death
	int				nb_of_meals;
}				t_philo;

// parameters.c
int	ft_strlen(char *str);
int	nb_len(int nb);
int	ft_atoi(char *str, int *nb);
int	parse_parameters(char **argv, t_params *p, int nb_of_params);

// utils.c
void	ft_putstr(char *str);
void	ft_putchar(char c);
int		print_actions(int philo_nb, char *str, t_params *p);
int		get_elapsed(t_params *p);
void	my_usleep(int time);

// errors.c
void	free_all(t_params *p, pthread_t *th, t_philo **ph_table);
int		error_free(char *error_msg, pthread_t *th, t_params *p, t_philo **ph_table);

// create_threads.c
int		init_mutexes(t_params *p);
int		destroy_mutexes(t_params *p, t_philo **ph_table);
t_philo	*create_philosopher(t_params *p, pthread_t *th, int i);
int		alloc_threads(t_philo ***ph_table, pthread_t **th, t_params *p);

// main.c
void	*simulation(void *philo);
int		start_simulation(t_params *p);

#endif