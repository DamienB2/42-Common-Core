/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 16:22:43 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/21 12:26:53 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if (sign < 0)
		return (-result);
	else
		return (result);
}

/*#include <stdio.h>
int	main(void)
{
	const char *t1 = "   42";
	const char *t2 = "\t\n\r\v\f  -42";
	const char *t3 = " +440";
	const char *t4 = "  --42";
	const char *t5 = "42abc50";
	const char *t6 = "2147483647";
	const char *t7 = "-2147483648";

	printf("--- TEST FT_ATOI ---\n\n");

	printf("Test 1 (Espaces simples)  : %d", ft_atoi(t1));
	printf("Test 2 (Espaces blancs)   : %d", ft_atoi(t2));
	printf("Test 3 (Signe +)          : %d", ft_atoi(t3));
	printf("Test 4 (Double signe)     : %d", ft_atoi(t4));
	printf("Test 5 (Texte au milieu)  : %d", ft_atoi(t5));
	printf("Test 6 (Max Int)          : %d", ft_atoi(t6));
	printf("Test 7 (Min Int)          : %d", ft_atoi(t7));

	return (0);
}*/
