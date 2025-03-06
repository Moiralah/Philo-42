/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huidris <huidris@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:45:24 by huidris           #+#    #+#             */
/*   Updated: 2025/03/06 04:31:23 by huidris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philo(t_data *data, t_philo *philo)
{
	int		i;

	i = 0;
	while (++i <= data->num_of_philo)
	{
		philo->id = i;
		if (pthread_mutex_init(&philo->fork, NULL) != 0)
			return (1);
		if (pthread_mutex_init(&philo->last_eat_lock, NULL) != 0)
			return (1);
		philo->data = data;
		if (i == data->num_of_philo)
			break ;
		philo->next = memset(malloc(sizeof(t_philo)), 0, sizeof(t_philo));
		if (!philo->next)
			error_exit("Malloc failed");
		philo = philo->next;
	}
	philo->next = data->first;
	data->last = philo;
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->num_of_philo = ft_argtoi(argv[1]);
	data->time_to_die = ft_argtoi(argv[2]);
	data->time_to_eat = ft_argtoi(argv[3]);
	data->time_to_sleep = ft_argtoi(argv[4]);
	if (argc == 6)
	{
		data->num_of_must_eat = ft_argtoi(argv[5]);
		if (pthread_mutex_init(&data->full_lock, NULL) != 0)
			return (1);
	}
	else
		data->num_of_must_eat = -1;
	data->full_philo = 0;
	data->is_dead = 0;
	data->start_time = get_time();
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	return (0);
}

int	print_status(t_data *data, t_philo *philo, int num)
{
	size_t	time;

	if (philo_die(data) == 1)
		return (1);
	pthread_mutex_lock(&data->print_mutex);
	time = get_time() - data->start_time;
	if (num == FORK)
		printf("%ld %d has taken a fork\n", time, philo->id);
	else if (num == EAT)
		printf("%ld %d is eating\n", time, philo->id);
	else if (num == SLEEP)
		printf("%ld %d is sleeping\n", time, philo->id);
	else if (num == THINK)
		printf("%ld %d is thinking\n", time, philo->id);
	pthread_mutex_unlock(&data->print_mutex);
	return (0);
}

void	clean_up(t_data *data)
{
	t_philo	*philo;
	t_philo	*temp;

	philo = data->first;
	data->last->next = NULL;
	while (philo)
	{
		if (pthread_mutex_destroy(&philo->fork) != 0)
			error_exit("Mutex destroy failed");
		if (pthread_mutex_destroy(&philo->last_eat_lock) != 0)
			error_exit("Mutex destroy failed");
		temp = philo->next;
		free(philo);
		philo = temp;
	}
	if (data->num_of_must_eat != -1
		&& pthread_mutex_destroy(&data->full_lock) != 0)
		error_exit("Mutex destroy failed");
	if (pthread_mutex_destroy(&data->print_mutex) != 0)
		error_exit("Mutex destroy failed");
}
