/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emtemir <emtemir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:23:47 by emtemir           #+#    #+#             */
/*   Updated: 2024/02/07 15:23:49 by emtemir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/philo.h"

// Belirtilen karakterin bir dizi içinde bulunup bulunmadığını kontrol eden fonksiyon.
bool is_in_strings(char c, char *str)
{
    while (*str)
    {
        // Karakter dizi içinde bulunursa true döndürür.
        if (*str == c)
            return true;
        str++;
    }
    // Karakter dizi içinde bulunmazsa false döndürür.
    return false;
}

// Bir karakter dizisini (string) tamsayıya (integer) dönüştüren fonksiyon.
long ft_atoi(const char *str)
{
    int sign;
    long result;

    // Beyaz boşluk karakterlerini atlayarak başlar.
    while (is_in_strings(*(char *)str, "\t\n\v\f\r "))
        str++;

    // İlk karakterde negatif işaret varsa, işaret değişkenini günceller.
    sign = 1;
    if (*str == '-')
        sign *= -1;
    if (*str == '-' || *str == '+')
        str++;

    // Karakter dizisini tamsayıya dönüştürür.
    result = 0;
    while (*str >= '0' && *str <= '9')
    {
        // Taşma kontrolü yapar.
        if (result * 10 < 0)
            return -1;
        result *= 10;
        result += *str - '0';
        str++;
    }

    return result * sign;
}

// Program argümanlarının geçerliliğini kontrol eden fonksiyon.
int input_checker(char **argv)
{
    int i, j;

    i = 1;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j])
        {
            // Boşluk karakterlerini atlar.
            if (argv[i][j] == ' ')
            {
                j++;
                continue;
            }
            // Sayısal olmayan bir karakter varsa hata döndürür.
            if ((argv[i][j] < 48 || argv[i][j] > 57))
                return (error(ERR_IN_1, NULL));
            j++;
        }
        i++;
    }

    // Geçerli ise 0 döndürür.
    return 0;
}

// Belirtilen miktar kadar mikrosaniye boyunca bekleme yapan fonksiyon.
int ft_usleep(useconds_t time)
{
    u_int64_t start;

    // Başlangıç zamanını alır.
    start = get_time();

    // Belirtilen süre kadar uyur.
    while ((get_time() - start) < time)
        usleep(time / 10);

    return 0;
}

// İki karakter dizisini karşılaştıran fonksiyon.
int ft_strcmp(char *s1, char *s2)
{
    while (*s1 != '\0' && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    // Karakter dizileri eşitse 0, farklıysa farklarının ASCII değeri döndürülür.
    return (*(char *)s1 - *(char *)s2);
}
