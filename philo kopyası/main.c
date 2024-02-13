/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 01:47:08 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/13 13:42:15 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;
	int				check_dead;
	int				i;
	int				philo_num;

	philo_num = ft_atol(av[1]);
	i = 0;
	check_dead = 0;
	philo = malloc(sizeof(t_philo) * philo_num);
	while (i < philo_num)
		philo[i++].args = malloc(sizeof(int));
	forks = malloc(sizeof(pthread_mutex_t));
	if (!philo || !forks)
		return (1);
	if (argumentinit(philo, ac, av, &check_dead))
		return (1);
	initmutex(philo, forks);
	initthread(philo);
	freemutex(philo, forks);
	return (0);
}
