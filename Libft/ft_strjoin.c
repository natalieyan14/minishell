/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:49:20 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 23:49:22 by natalieyan       ###   ########.fr       */
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
