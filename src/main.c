/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:23:22 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/12 22:56:33 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/philo.h"

// Tek bir filozof bulunduğu durumu işleyen fonksiyon.
int case_one(t_data *data)
{
    // Başlangıç zamanını alır.
    data->start_time = get_time();

    // Birinci thread'i oluşturur ve filo rutinini başlatır.
    if (pthread_create(&data->tid[0], NULL, &routine, &data->philos[0]))
        return (error(TH_ERR, data));

    // Birinci thread'in bağlantısını koparır, bağımsız bir şekilde çalışmasını sağlar.
    pthread_detach(data->tid[0]);

    // Tüm filozof ölmediği sürece bekleme devam eder.
    while (data->dead == 0)
        ft_usleep(0);

    // Programı sonlandırır ve kullanılan kaynakları temizler.
    ft_exit(data);
    return (0);
}

// Kullanılan veri yapılarını temizleyen fonksiyon.
void clear_data(t_data *data)
{
    // Thread ve mutex'leri temizler.
    if (data->tid)
        free(data->tid);
    if (data->forks)
        free(data->forks);
    if (data->philos)
        free(data->philos);
}

// Programın sonlandırılmasını sağlayan fonksiyon.
void ft_exit(t_data *data)
{
    int i;

    // Her filozof için kullanılan mutex'leri temizler.
    i = -1;
    while (++i < data->philo_num)
    {
        pthread_mutex_destroy(&data->forks[i]);
        pthread_mutex_destroy(&data->philos[i].lock);
    }
    // Diğer mutex'leri temizler.
    pthread_mutex_destroy(&data->write);
    pthread_mutex_destroy(&data->lock);

    // Kullanılan veri yapılarını temizler.
    clear_data(data);
}

// Hata durumunda programı sonlandıran fonksiyon.
int error(char *str, t_data *data)
{
    // Hata mesajını ekrana yazdırır.
    printf("%s\n", str);

    // Veri yapıları kullanıldıysa temizler.
    if (data)
        ft_exit(data);

    // Hata durumunu işaret eder.
    return (1);
}

// Programın giriş noktası.
int main(int argc, char **argv)
{
    // Program için gerekli veri yapısını oluşturur.
    t_data data;

    // Geçerli argüman sayısı kontrolü yapılır.
    if (argc < 5 || argc > 6)
        return (1);

    // Argümanların geçerliliğini kontrol eder.
    if (input_checker(argv))
        return (1);

    // Programın temel veri yapılarını başlatır.
    if (init(&data, argv, argc))
        return (1);

    // Tek bir filozof durumu için özel işleme geçer.
    if (data.philo_num == 1)
        return (case_one(&data));

    // Birden fazla filozof durumu için thread'leri başlatır.
    if (thread_init(&data))
        return (1);

    // Program sonlandığında kullanılan kaynakları temizler.
    ft_exit(&data);

    // Program başarıyla sonlandırılır.
    return (0);
}
