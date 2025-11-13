/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:08:43 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:08:45 by nharutyu         ###   ########.fr       */
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
