/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huidris <huidris@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:25:36 by huidris           #+#    #+#             */
/*   Updated: 2025/03/06 23:54:47 by huidris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_args(char **argv)
{
	int	i;

	i = 5;
	while (--i > 1)
	{
		if (ft_argtoi(argv[i]) < 60)
			error_exit("Philo need time to be alive");
	}
	if (ft_argtoi(argv[1]) > 200 || ft_argtoi(argv[1]) < 1)
		error_exit("Philo number must be between 1-200");
	if (argv[5] && ft_argtoi(argv[5]) < 0)
		error_exit("Invalid amount");
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
		if (num > INT_MAX / 10 || (num == INT_MAX / 10
				&& (str[i] - '0') > INT_MAX % 10))
			error_exit("Number too large");
		num = num * 10 + str[i] - '0';
		i++;
	}
	if (str[i] != '\0')
		error_exit("Only digits are allowed");
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
