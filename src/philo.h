/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huidris <huidris@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:51:29 by huidris           #+#    #+#             */
/*   Updated: 2025/03/11 01:23:45 by huidris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t		philo_id;
	int				id;
	pthread_mutex_t	fork;
	int				must_eat;
	size_t			last_eat_time;
	pthread_mutex_t	last_eat_lock;
	int				full_check;
	struct s_data	*data;
	struct s_philo	*next;
}					t_philo;

typedef struct s_data
{
	int				num_of_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	int				time_to_sleep;
	int				num_of_must_eat;
	int				is_dead;
	pthread_mutex_t	is_dead_lock;
	int				full_philo;
	pthread_mutex_t	full_lock;
	size_t			start_time;
	pthread_mutex_t	print_mutex;
	t_philo			*first;
	t_philo			*last;
}					t_data;

enum e_life
{
	FORK,
	EAT,
	SLEEP,
	THINK,
};

// philo.c
void	*philo_life(void *arg);
int		living(t_data *data);
void	join_philo(t_data *data);
int		monitoring(t_data *data);
// philo_action.c
void	grab_forks(t_data *data, t_philo *philo);
void	put_forks(t_data *data, t_philo *philo);
void	philo_eat(t_data *data, t_philo *philo);
int		philo_last_eat(t_data *data, t_philo *philo);
int		philo_die(t_data *data);
// philo_utils.c
int		create_philo(t_data *data, t_philo *philo);
int		init_data(t_data *data, int argc, char **argv);
int		print_status(t_data *data, t_philo *philo, int num);
void	clean_up(t_data *data);
// utils.c
int		validate_args(char **argv);
int		ft_argtoi(const char *str);
void	error_exit(char *msg);
void	ft_usleep(size_t time);
size_t	get_time(void);

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

// memset, printf, malloc, free, write,
// usleep, gettimeofday, pthread_create,
// pthread_detach, pthread_join, pthread_mutex_init,
// pthread_mutex_destroy, pthread_mutex_lock,
// pthread_mutex_unlock

// number_of_philosophers
// time_to_die
// time_to_eat
// time_to_sleep
// number_of_times_each_philosopher_must_eat

#endif
