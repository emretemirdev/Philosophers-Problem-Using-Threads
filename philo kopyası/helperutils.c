/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helperutils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 01:46:58 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/13 17:09:36 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] || s2[j])
	{
		if (s1[i] == s2[j])
		{
			i++;
			j++;
		}
		else
			return (1);
	}
	return (0);
}

int	ft_atol(char *str)
{
	long	i;
	long	sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] <= '9' && str[i] >= '0')
	{
		result = result * 10 + (str[i++] - '0');
		if (result > 2147483648)
			return (-1);
	}
	if ((result * sign) == 2147483648)
		return (-1);
	return ((int)result * sign);
}

//initmutex tarafından oluşturulan mutexleri serbest bırakır.
//philo: t_philo yapısı
//forks: t_philo yapısındaki her bir filozofun solunda bulunan çatalı temsil eden mutexlerin bir dizisi
void	freemutex(t_philo *philo, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < philo->args[PHILOCOUNT])
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&philo[i].lasteatmutex);
		pthread_mutex_destroy(&philo[i].totaleatmutex);
		i++;
	}
	pthread_mutex_destroy(philo->death);
	free(forks);
}
