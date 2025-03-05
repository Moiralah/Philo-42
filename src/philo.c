/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huidris <huidris@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:44:59 by huidris           #+#    #+#             */
/*   Updated: 2025/03/06 03:59:33 by huidris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_life(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		ft_usleep(100);
	while (philo_die(data) == 0)
	{
		philo_eat(data, philo);
		print_status(data, philo, SLEEP);
		ft_usleep(data->time_to_sleep);
		print_status(data, philo, THINK);
	}
	return (NULL);
}

int	living(t_data *data)
{
	t_philo	*philo;

	philo = data->first;
	data->start_time = get_time();
	while (1)
	{
		philo->last_eat_time = get_time();
		if (pthread_create(&philo->philo_id, NULL, &philo_life, philo) != 0)
			return (1);
		philo = philo->next;
		if (philo == data->first)
			break ;
	}
	return (0);
}

void	join_philo(t_data *data)
{
	t_philo	*philo;

	philo = data->first;
	while (1)
	{
		if (pthread_join(philo->philo_id, NULL) != 0)
			error_exit("Join failed");
		philo = philo->next;
		if (philo == data->first)
			break ;
	}
}

int	monitoring(t_data *data)
{
	t_philo	*philo;

	if (living(data))
		return (1);
	if (data->num_of_must_eat == 0)
		return (printf("No eating. Philo hangry.\n"), 0);
	philo = data->first;
	while (1)
	{
		if (philo_last_eat(data, philo) == 1)
			break ;
		philo = philo->next;
		if (data->num_of_must_eat != -1
			&& data->full_philo == data->num_of_philo)
			break ;
	}
	join_philo(data);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		error_exit("Invalid number of arguments");
	if (validate_args(argv))
		return (1);
	data = (t_data){0};
	if (init_data(&data, argc, argv))
		return (1);
	data.first = memset(malloc(sizeof(t_philo)), 0, sizeof(t_philo));
	if (!data.first)
		error_exit("Malloc failed");
	if (create_philo(&data, data.first))
		return (1);
	if (monitoring(&data))
		return (1);
	clean_up(&data);
	return (0);
}
