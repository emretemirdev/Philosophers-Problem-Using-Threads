/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 01:47:04 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/13 17:24:54 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*Mutex Kilitleme: Aynı anda sadece bir filozofun çatal almasını sağlamak için pthread_mutex_lock kullanılır.
Bu, her birinin bir çatalı sonsuza kadar tuttuğu çıkmazları önler.
Tek Filozof Durumu: Asla her iki çatalı da alamayacak ve açlıktan ölecek tek bir filozofun olduğu durumu yönetir.
Yeme Simülasyonu: waitfunc muhtemelen filozofun belirli bir süre boyunca yemesini simüle etmek için iş parçacığını duraklatır.
Veri Koruma: Paylaşılan filozof verilerini güncellemeler sırasında korumak için mutexler (lasteatmutex, totaleatmutex) kullanılır.
*/

int	takeforkandeat(t_philo *philo)
{
	    // Sol çatalı al (uygun değilse bekle)
	pthread_mutex_lock(philo->leftfork);
	printphilo(philo, "has taken a fork");
	if (philo->args[PHILOCOUNT] == 1)
	{
		// Açlıktan 'ölme'yi simüle et
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
