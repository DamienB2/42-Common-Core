# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    ft_count_harvest_iterative.py                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/25 23:43:51 by dbaise            #+#    #+#              #
#    Updated: 2026/05/25 23:43:51 by dbaise           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

def ft_count_harvest_iterative():
    days = int(input("Days until harvest: "))
    for i in range(days):
        print("Day", i + 1)
    print("Harvest time!")