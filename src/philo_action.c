/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huidris <huidris@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 03:37:54 by huidris           #+#    #+#             */
/*   Updated: 2025/03/07 23:41:16 by huidris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	grab_forks(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	print_status(data, philo, FORK);
	if (data->num_of_philo == 1)
		return (ft_usleep(data->time_to_die + 1));
	pthread_mutex_lock(&philo->next->fork);
	print_status(data, philo, FORK);
}

void	put_forks(t_data *data, t_philo *philo)
{
	pthread_mutex_unlock(&philo->fork);
	if (data->num_of_philo == 1)
		return ;
	pthread_mutex_unlock(&philo->next->fork);
}

void	philo_eat(t_data *data, t_philo *philo)
{
	grab_forks(data, philo);
	if (print_status(data, philo, EAT) == 1)
		return (put_forks(data, philo));
	pthread_mutex_lock(&philo->last_eat_lock);
	philo->last_eat_time = get_time();
	pthread_mutex_unlock(&philo->last_eat_lock);
	if (data->time_to_eat > data->time_to_die)
		ft_usleep(data->time_to_die + 1);
	else
		ft_usleep(data->time_to_eat);
	philo->must_eat++;
	if (data->num_of_must_eat != -1 && philo->full_check == 0
		&& philo->must_eat == data->num_of_must_eat)
	{
		pthread_mutex_lock(&data->full_lock);
		data->full_philo++;
		pthread_mutex_unlock(&data->full_lock);
		philo->full_check = 1;
	}
	put_forks(data, philo);
	return ;
}

int	philo_last_eat(t_data *data, t_philo *philo)
{
	size_t	time_diff;

	pthread_mutex_lock(&philo->last_eat_lock);
	time_diff = get_time() - philo->last_eat_time;
	pthread_mutex_unlock(&philo->last_eat_lock);
	if (time_diff > data->time_to_die)
	{
		data->is_dead = 1;
		printf("%ld %d died\n", get_time() - data->start_time, philo->id);
		return (1);
	}
	return (0);
}

int	philo_die(t_data *data)
{
	if (data->is_dead == 1)
		return (1);
	if (data->num_of_must_eat == -1)
		return (0);
	pthread_mutex_lock(&data->full_lock);
	if (data->full_philo == data->num_of_philo)
		return (pthread_mutex_unlock(&data->full_lock), 1);
	pthread_mutex_unlock(&data->full_lock);
	return (0);
}
