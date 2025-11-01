/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:50:19 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 23:50:21 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	const char	*ptr;
	char		*trimmed_str;
	size_t		length;

	if (!s1 || !set)
		return (NULL);
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	ptr = s1;
	while (*ptr != '\0')
		ptr++;
	length = (ptr - s1);
	while (length > 0 && ft_strchr(set, *(s1 + length -1)))
		length--;
	trimmed_str = (char *)malloc(length + 1);
	if (!trimmed_str)
		return (NULL);
	ft_strlcpy(trimmed_str, s1, length + 1);
	return (trimmed_str);
}
