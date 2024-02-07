/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:23:28 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/07 15:23:30 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/philo.h"
// Bellek tahsis işlemlerini gerçekleştiren fonksiyon.
int alloc(t_data *data)
{
    // Thread kimlikleri için bellek tahsis edilir.
    data->tid = malloc(sizeof(pthread_t) * data->philo_num);
    if (!data->tid)
        return (error(ALLOC_ERR_1, data));

    // Fork'lar için mutex'lerin bulunduğu dizi için bellek tahsis edilir.
    data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
    if (!data->forks)
        return (error(ALLOC_ERR_2, data));

    // Filozoflar için veri yapısının bulunduğu dizi için bellek tahsis edilir.
    data->philos = malloc(sizeof(t_philo) * data->philo_num);
    if (!data->philos)
        return (error(ALLOC_ERR_3, data));

    return (0);
}

// Fork'ların ve filozofların başlangıç durumlarını başlatan fonksiyon.
int init_forks(t_data *data)
{
    int i;

    // Fork'lar için mutex'leri başlatır.
    i = -1;
    while (++i < data->philo_num)
        pthread_mutex_init(&data->forks[i], NULL);

    // İlk filozofun sol çatalını sağdaki son çatala, sağ çatalını solundaki çatala bağlar.
    i = 0;
    data->philos[0].l_fork = &data->forks[0];
    data->philos[0].r_fork = &data->forks[data->philo_num - 1];
    i = 1;
    while (i < data->philo_num)
    {
        data->philos[i].l_fork = &data->forks[i];
        data->philos[i].r_fork = &data->forks[i - 1];
        i++;
    }

    return (0);
}

// Filozofların başlangıç durumlarını başlatan fonksiyon.
void init_philos(t_data *data)
{
    int i;

    i = 0;
    while (i < data->philo_num)
    {
        // Her filozofun bağlı olduğu veri yapısını ve kimliğini ayarlar.
        data->philos[i].data = data;
        data->philos[i].id = i + 1;
        data->philos[i].time_to_die = data->death_time;
        data->philos[i].eat_cont = 0;
        data->philos[i].eating = 0;
        data->philos[i].status = 0;
        pthread_mutex_init(&data->philos[i].lock, NULL);
        i++;
    }
}

// Giriş parametreleriyle program veri yapısını başlatan fonksiyon.
int init_data(t_data *data, char **argv, int argc)
{
    // Program argümanlarını veri yapısına atar.
    data->philo_num = (int)ft_atoi(argv[1]);
    data->death_time = (u_int64_t)ft_atoi(argv[2]);
    data->eat_time = (u_int64_t)ft_atoi(argv[3]);
    data->sleep_time = (u_int64_t)ft_atoi(argv[4]);

    // Opsiyonel olarak belirtilen öğün sayısını alır.
    if (argc == 6)
        data->meals_nb = (int)ft_atoi(argv[5]);
    else
        data->meals_nb = -1;

    // Geçerlilik kontrolü yapar.
    if (data->philo_num <= 0 || data->philo_num > 200 || data->death_time < 0 || data->eat_time < 0 || data->sleep_time < 0)
        return (error(ERR_IN_2, NULL));

    // Diğer veri yapısı elemanlarını başlatır.
    data->dead = 0;
    data->finished = 0;
    pthread_mutex_init(&data->write, NULL);
    pthread_mutex_init(&data->lock, NULL);

    return (0);
}

// Programın başlangıç noktası.
int	init(t_data *data, char **argv, int argc)
{
	if (init_data(data, argv, argc))
		return (1);
	if (alloc(data))
		return (1);
	if (init_forks(data))
		return (1);
	init_philos(data);
	return (0);
}