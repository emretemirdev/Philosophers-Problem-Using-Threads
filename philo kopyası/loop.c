/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 01:47:04 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/13 01:47:05 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	takeforkandeat(t_philo *philo)
{
	pthread_mutex_lock(philo->leftfork);
	printphilo(philo, "has taken a fork");
	if (philo->args[PHILOCOUNT] == 1)
	{
		waitfunc(philo->args[DIETIME]);
		printphilo(philo, "is died");
		pthread_mutex_unlock(philo->leftfork);
		return (1);
	}
	pthread_mutex_lock(philo->rightfork);
	printphilo(philo, "has taken a fork");
	printphilo(philo, "is eating");
	pthread_mutex_lock(&philo->lasteatmutex);
	philo->lasteat = gettime();
	pthread_mutex_unlock(&philo->lasteatmutex);
	waitfunc(philo->args[EATTIME]);
	pthread_mutex_lock(&philo->totaleatmutex);
	philo->totaleat++;
	pthread_mutex_unlock(&philo->totaleatmutex);
	pthread_mutex_unlock(philo->leftfork);
	pthread_mutex_unlock(philo->rightfork);
	return (0);
}

void	sleepfunc(t_philo *philo)
{
	printphilo(philo, "is sleeping");
	waitfunc(philo->args[SLEEPTIME]);
}

void	*lifecycle(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		pthread_mutex_lock(philo->death);
		if (*philo->checkdead)
		{
			pthread_mutex_unlock(philo->death);
			break ;
		}
		pthread_mutex_unlock(philo->death);
		if (takeforkandeat(philo))
			break ;
		pthread_mutex_lock(&philo->lasteatmutex);
		if (philo->totaleat == philo->args[MINEAT])
			break ;
		pthread_mutex_unlock(&philo->lasteatmutex);
		sleepfunc(philo);
		printphilo(philo, "is thinking");
	}
	pthread_mutex_unlock(&philo->lasteatmutex);
	return (NULL);
}
