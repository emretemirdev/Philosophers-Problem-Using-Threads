/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 01:46:58 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/13 17:10:14 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

int	checkargument(int ac, char **av)
{
	int	i;
	int	j;

	if (ac == 5 || ac == 6)
	{
		i = 1;
		while (av[i])
		{
			j = 0;
			while (av[i][j])
			{
				if (!(av[i][j] >= '0' && av[i][j] <= '9'))
					return (0);
				j++;
			}
			i++;
		}
	}
	else
		return (0);
	return (1);
}

int	argumentinit(t_philo *philo, int ac, char **av, int *check_dead)
{
	int	i;

	i = -1;
	if (!checkargument(ac, av) || ft_atol(av[1]) == 0)
		return (1);
	while (++i < ft_atol(av[1]))
	{
		philo[i].id = i + 1;
		philo[i].args[PHILOCOUNT] = ft_atol(av[1]);
		philo[i].args[DIETIME] = ft_atol(av[2]);
		philo[i].args[EATTIME] = ft_atol(av[3]);
		philo[i].args[SLEEPTIME] = ft_atol(av[4]);
		if (ac == 6)
			philo[i].args[MINEAT] = ft_atol(av[5]);
		else
			philo[i].args[MINEAT] = -1;
		if (!philo[i].args[DIETIME] || !philo[i].args[EATTIME]
			|| !philo[i].args[SLEEPTIME] || !philo[i].args[MINEAT])
			return (1);
		philo[i].lasteat = gettime();
		philo[i].time = gettime();
		philo[i].totaleat = 0;
		philo[i].checkdead = check_dead;
	}
	return (0);
}

void	initmutex(t_philo *philo, pthread_mutex_t *forks)
{
	pthread_mutex_t	death;
	int				i;

	i = 0;
	while (i < philo->args[PHILOCOUNT])
	{
		philo[i].leftfork = &forks[i];
		if (i == philo->args[PHILOCOUNT] - 1)
			philo[i].rightfork = &forks[0];
		else
			philo[i].rightfork = &forks[i + 1];
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&philo[i].lasteatmutex, NULL);
		pthread_mutex_init(&philo[i].totaleatmutex, NULL);
		philo[i].death = &death;
		i++;
	}
	pthread_mutex_init(philo->death, NULL);
}

//initthread: Her bir filozof için bir thread başlatır ve yaşam döngüsülerini yönetir.
//philo: t_philo yapısı
void	initthread(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->args[PHILOCOUNT])
	{
		if (pthread_create(&philo[i].thread, NULL, lifecycle, &philo[i]))
			return ;
		usleep(50);
		i++;
	}
	while (1)
	{
		if (checkdeath(philo))
			break ;
	}
	i = 0;
	while (i < philo->args[PHILOCOUNT])
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}
