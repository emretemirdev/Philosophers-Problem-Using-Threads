/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 18:35:49 by tterribi          #+#    #+#             */
/*   Updated: 2024/01/19 13:44:50 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/philo.h"

// Sistem saatinden geçerli bir zamanı alır.
u_int64_t get_time(void)
{
	struct timeval tv;

	// gettimeofday() fonksiyonu ile zaman bilgisi elde edilir.
	if (gettimeofday(&tv, NULL))
		return (error("gettimeofday() FAILURE\n", NULL));

	// Zaman bilgisi milisaniye cinsinden hesaplanır ve döndürülür.
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}

// Ekran çıktılarını belirli bir formatta gönderir.
void messages(char *str, t_philo *philo)
{
	u_int64_t time;

	// Mutex ile korunan kritik bölgeye girilir.
	pthread_mutex_lock(&philo->data->write);

	// Zamandan bağımsız olarak bir olayın gerçekleşme zamanı hesaplanır.
	time = get_time() - philo->data->start_time;

	// Eğer bir filozof öldüyse ve bu ilk ölümse, ölümü kaydedip bayrak değiştirilir.
	if (ft_strcmp(DIED, str) == 0 && philo->data->dead == 0)
	{
		printf("%llu %d %s\n", time, philo->id, str);
		philo->data->dead = 1;
	}

	// Eğer filozof ölmediyse, olayı ekrana yazdırır.
	if (!philo->data->dead)
		printf("%llu %d %s\n", time, philo->id, str);

	// Mutex ile korunan kritik bölgeden çıkılır.
	pthread_mutex_unlock(&philo->data->write);
}

// Filozofun sağ ve sol çatalını almasını sağlar.
void take_forks(t_philo *philo)
{
	// Sağ çatalı alır ve mesaj gönderir.
	pthread_mutex_lock(philo->r_fork);
	messages(TAKE_FORKS, philo);

	// Sol çatalı alır ve mesaj gönderir.
	pthread_mutex_lock(philo->l_fork);
	messages(TAKE_FORKS, philo);
}

// Filozofun çatalını bırakmasını ve bir süre uyumasını sağlar.
void drop_forks(t_philo *philo)
{
	// Sol ve sağ çatalı bırakır, ardından uyur ve mesaj gönderir.
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	messages(SLEEPING, philo);
	ft_usleep(philo->data->sleep_time);
}

// Filozofun yemek yemesini sağlar.
void eat(t_philo *philo)
{
	// Çatal alımını sağlayan fonksiyonu çağırır.
	take_forks(philo);

	// Kritik bölgeye girer ve yemek yeme işlemlerini gerçekleştirir.
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->time_to_die = get_time() + philo->data->death_time;
	messages(EATING, philo);
	philo->eat_cont++;
	ft_usleep(philo->data->eat_time);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);

	// Çatal bırakma ve uyuma işlemlerini gerçekleştirir.
	drop_forks(philo);
}