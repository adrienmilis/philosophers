#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

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
	pthread_mutex_t	tod_mutex;
	int				nb_of_meals;
}				t_philo;

// parameters.c
int		ft_strlen(char *str);
int		nb_len(int nb);
int		ft_atoi(char *str, int *nb);
int		parse_parameters(char **argv, t_params *p, int nb_of_params);

// utils.c
void	ft_putstr(char *str);
void	ft_putchar(char c);
int		print_actions(int philo_nb, char *str, t_params *p);
int		get_elapsed(t_params *p);
void	my_usleep(int time, int nb_of_philo);

// errors.c
void	free_all(t_params *p, pthread_t *th, t_philo **ph_table);
int		error_free(char *emsg, pthread_t *th, t_params *p, t_philo **ph_table);

// threads_and_mutexes.c
int		init_mutexes(t_params *p);
int		destroy_mutexes(t_params *p, t_philo **ph_table);
t_philo	*create_philosopher(t_params *p, pthread_t *th, int i);
int		alloc_threads(t_philo ***ph_table, pthread_t **th, t_params *p);
void	unlock_forks(t_philo *ph);

// simulation.c
void	*simulation(void *philo);
int		start_simulation(t_params *p);
void	*eat_timer(void *params);
int		eating(t_philo *ph);
int		sleeping_thinking(t_philo *ph);

// main.c
int		tab_is_full(int *tab, int len);
void	check_meals(t_philo *ph);
int		create_all_philos(t_params *p, t_philo **ph_tab, pthread_t *th);
int		init_times_of_death(t_params *p);
int		main(int argc, char **argv);

#endif