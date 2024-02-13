/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 04:12:41 by gotunc            #+#    #+#             */
/*   Updated: 2024/02/13 16:14:03 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# define PHILOCOUNT 0
# define DIETIME 1
# define EATTIME 2
# define SLEEPTIME 3
# define MINEAT 4

typedef struct s_philo
{
	int						id;
	int						totaleat;
	int						*checkdead;
	int						*args;
	time_t					time;
	time_t					lasteat;
	pthread_t				thread;
	pthread_mutex_t			*death;
	pthread_mutex_t			lasteatmutex;
	pthread_mutex_t			totaleatmutex;
	pthread_mutex_t			*leftfork;
	pthread_mutex_t			*rightfork;
}							t_philo;

int		checkargument(int ac, char **av); // helperutils.c Komut satırı argümanlarını doğrular.
int		checkdeath(t_philo *philo); 
int		waitfunc(time_t waitime_t);
int		ft_atol(char *str);
time_t	gettime(void);
void	initmutex(t_philo *philo, pthread_mutex_t *forks);
int		argumentinit(t_philo *philo, int ac, char **av, int *a);
void	initthread(t_philo *philo);
void	*lifecycle(void *args);
int		takeforkandeat(t_philo *philo);
void	sleepfunc(t_philo *philo);
void	printphilo(t_philo *philo, char *status);
int		ft_strcmp(char *s1, char *s2);
void	freemutex(t_philo *philo, pthread_mutex_t *forks);

#endif