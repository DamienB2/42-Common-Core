/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 12:34:36 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/17 16:27:12 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_number_size(long n, int number_size)
{
	if (n == 0)
		return (number_size + 1);
	while (n != 0)
	{
		n = n / 10;
		number_size += 1;
	}
	return (number_size);
}

static void	fill_result(char *result, long n, int is_negative, int total_size)
{
	int	i;

	result[total_size] = '\0';
	i = total_size - 1;
	if (n == 0)
	{
		result[i] = '0';
		return ;
	}
	while (n != 0)
	{
		result[i] = (n % 10) + '0';
		n = n / 10;
		i--;
	}
	if (is_negative)
		result[i] = '-';
}

char	*ft_itoa(int n)
{
	int		number_size;
	int		is_negative;
	long	nbr;
	char	*result;

	nbr = (long) n;
	if (nbr < 0)
	{
		is_negative = 1;
		number_size = 1;
		nbr = -nbr;
	}
	else
	{
		is_negative = 0;
		number_size = 0;
	}
	number_size = get_number_size(nbr, number_size);
	result = malloc(sizeof(char) * (number_size + 1));
	if (!result)
		return (NULL);
	fill_result(result, nbr, is_negative, number_size);
	return (result);
}

/*#include <stdio.h>
  int main(int argc, char const *argv[])
  {
  printf("nombre = %s", ft_itoa(-2147483648));

  return 0;
  }*/
