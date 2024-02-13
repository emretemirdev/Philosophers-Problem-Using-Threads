/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deadcontrol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 01:46:52 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/13 01:46:54 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philoisdead(t_philo *philo)
{
	pthread_mutex_lock(&philo->lasteatmutex);
	if ((gettime() - philo->lasteat) > philo->args[DIETIME])
	{
		pthread_mutex_unlock(&philo->lasteatmutex);
		printphilo(philo, "is died");
		return (1);
	}
	pthread_mutex_unlock(&philo->lasteatmutex);
	return (0);
}

int	ismaxeat(t_philo *philo)
{
	if (philo->args[MINEAT] != -1)
	{
		pthread_mutex_lock(&philo->totaleatmutex);
		if (philo->totaleat == philo->args[MINEAT])
		{
			pthread_mutex_unlock(&philo->totaleatmutex);
			return (1);
		}
		pthread_mutex_unlock(&philo->totaleatmutex);
	}
	return (0);
}

int	checkdeath(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->args[PHILOCOUNT])
	{
		if (philoisdead(&philo[i]) || ismaxeat(&philo[i]))
			return (1);
		i++;
	}
	return (0);
}
