/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:23:52 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/07 15:23:54 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../lib/philo.h"

// Bu fonksiyon, bir filozofun ölüm durumunu izler ve gerektiğinde bildirimler yapar.
void	*monitor(void *data_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) data_pointer;

	// write mutex ile kritik bölgeyi kilitleyerek ekrana yazdırma işlemi yapılır
	pthread_mutex_lock(&philo->data->write);
	printf("data val: %d", philo->data->dead);
	pthread_mutex_unlock(&philo->data->write);

	// Bir döngü içinde filozofun ölüm durumunu izler
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);

		// Eğer tüm filozoflar yemeklerini bitirdiyse, genel ölüm durumunu işaretler
		if (philo->data->finished >= philo->data->philo_num)
			philo->data->dead = 1;

		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

// Bu fonksiyon, bir filozofun davranışını denetler ve gerektiğinde ölüm durumu kontrol eder.
void	*supervisor(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;

	// Filozof ölmediği sürece davranışını kontrol eder
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);

		// Eğer filozofun ölüm zamanı geçmişse ve yemek yemiyorsa, ölüm mesajını verir
		if (get_time() >= philo->time_to_die && philo->eating == 0)
			messages(DIED, philo);

		// Eğer filozof tüm öğünlerini yedi ise, genel bitirilen filozof sayısını artırır
		if (philo->eat_cont == philo->data->meals_nb)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++;
			philo->eat_cont++;
			pthread_mutex_unlock(&philo->data->lock);
		}

		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

// Bu fonksiyon, bir filozofun yaşam döngüsünü simüle eder ve gerekirse bir denetim iş parçacığı başlatır.
void	*routine(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;

	// Filozofun ölüm zamanını hesaplar
	philo->time_to_die = philo->data->death_time + get_time();

	// Supervisor iş parçacığını başlatır
	if (pthread_create(&philo->t1, NULL, &supervisor, (void *)philo))
		return ((void *)1);

	// Filozof ölmediği sürece yemek yer ve düşünür
	while (philo->data->dead == 0)
	{
		eat(philo);  // Bir filozofun yemek yeme işlemi
		messages(THINKING, philo);  // "Düşünüyor" mesajı verilir
	}

	// Supervisor iş parçacığının tamamlanmasını bekler
	if (pthread_join(philo->t1, NULL))
		return ((void *)1);

	return ((void *)0);
}

// Bu işlev, verilen veri yapısına dayalı olarak çoklu iş parçacıklarını başlatır ve sonuçlarını yönetir.
int	thread_init(t_data *data)
{
	int			i;
	pthread_t	t0;

	i = -1;

	// Başlangıç zamanını kaydeder
	data->start_time = get_time();

	// Eğer her filozof belirli bir sayıda yemek yiyorsa, bir kontrol iş parçacığı başlatır
	if (data->meals_nb > 0)
	{
		if (pthread_create(&t0, NULL, &monitor, &data->philos[0]))
			return (error(TH_ERR, data));
	}

	// Her filozof için ayrı bir iş parçacığı başlatır
	while (++i < data->philo_num)
	{
		if (pthread_create(&data->tid[i], NULL, &routine, &data->philos[i]))
			return (error(TH_ERR, data));
		ft_usleep(1);
	}

	i = -1;

	// Tüm filozofların iş parçacıklarının tamamlanmasını bekler
	while (++i < data->philo_num)
	{
		if (pthread_join(data->tid[i], NULL))
			return (error(JOIN_ERR, data));
	}

	return (0);
}