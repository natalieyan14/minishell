/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:07:18 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:07:19 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	char	*cop_result;

	result = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) +1);
	if (!result)
		return (NULL);
	cop_result = result;
	while (*s1)
		*cop_result++ = *s1++;
	while (*s2)
		*cop_result++ = *s2++;
	*cop_result = '\0';
	return (result);
}
