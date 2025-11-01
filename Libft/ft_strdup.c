/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:48:54 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 23:48:55 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char		*s2;
	char		*ret_s2;
	const char	*ptr;
	size_t		len;

	ptr = src;
	len = 0;
	while (*ptr++)
		len++;
	s2 = (char *)malloc((len + 1) * sizeof(char));
	if (s2 == NULL)
		return (NULL);
	ret_s2 = s2;
	while (*src)
		*s2++ = *src++;
	*s2 = '\0';
	return (ret_s2);
}
