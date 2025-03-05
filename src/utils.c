/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huidris <huidris@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:25:36 by huidris           #+#    #+#             */
/*   Updated: 2025/03/06 03:37:20 by huidris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_args(char **argv)
{
	int	i;

	i = 0;
	while (argv[++i] != NULL)
		ft_argtoi(argv[i]);
	if (ft_argtoi(argv[1]) > 200)
		error_exit("Too much philo");
	return (0);
}

int	ft_argtoi(const char *str)
{
	int	i;
	int	num;

	i = 0;
	num = 0;
	if ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		error_exit("Only digits are allowed");
	else if (str[i] == '+')
		error_exit("No sign are allowed");
	else if (str[i] == '-')
		error_exit("No negative number allowed");
	else if (str[i] < '0' || str[i] > '9')
		error_exit("Only digits are allowed");
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + str[i] - '0';
		i++;
	}
	return (num);
}

void	error_exit(char *msg)
{
	write(2, "Error: ", 7);
	printf("%s\n", msg);
	exit(1);
}

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
