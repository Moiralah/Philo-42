/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huidris <huidris@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:45:24 by huidris           #+#    #+#             */
/*   Updated: 2025/03/04 20:37:23 by huidris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void	ft_usleep(size_t time)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(100);
}

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
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

void	philo_sleep(t_data *data, t_philo *philo)
{
	print_status(data, philo, SLEEP);
	ft_usleep(data->time_to_sleep);
}

void	philo_think(t_data *data, t_philo *philo)
{
	print_status(data, philo, THINK);
}

int	philo_die(t_data *data)
{
	pthread_mutex_lock(&data->is_dead_lock);
	if (data->is_dead == 1)
		return (pthread_mutex_unlock(&data->is_dead_lock), 1);
	pthread_mutex_unlock(&data->is_dead_lock);
	if (data->num_of_must_eat == -1)
		return (0);
	pthread_mutex_lock(&data->full_lock);
	if (data->full_philo == data->num_of_must_eat)
		return (pthread_mutex_unlock(&data->full_lock), 1);
	pthread_mutex_unlock(&data->full_lock);
	return (0);
}

int	philo_last_eat(t_data *data, t_philo *philo)
{
	size_t	time;
	size_t	time_diff;

	pthread_mutex_lock(&philo->last_eat_lock);
	time_diff = get_time() - philo->last_eat_time;
	pthread_mutex_unlock(&philo->last_eat_lock);
	if (time_diff > data->time_to_die)
	{
		pthread_mutex_lock(&data->is_dead_lock);
		data->is_dead = 1;
		pthread_mutex_unlock(&data->is_dead_lock);
		//ft_usleep(1000);
		time = get_time() - data->start_time;
		printf("%ld %d died\n", time, philo->id);
		return (1);
	}
	return (0);
}

void	clean_up(t_data *data)
{
	t_philo	*philo;
	t_philo *temp;

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
	if (pthread_mutex_destroy(&data->is_dead_lock) != 0)
		error_exit("Mutex destroy failed");
	if (data->num_of_must_eat != -1
		&& pthread_mutex_destroy(&data->full_lock) != 0)
		error_exit("Mutex destroy failed");
	if (pthread_mutex_destroy(&data->print_mutex) != 0)
		error_exit("Mutex destroy failed");
}
