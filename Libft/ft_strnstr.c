/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:08:33 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:08:35 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*hays_s;
	char	*need_s;
	size_t	size;

	hays_s = (char *)haystack;
	need_s = (char *)needle;
	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack != '\0' && len > 0)
	{
		hays_s = (char *)haystack;
		need_s = (char *)needle;
		size = len;
		while (*hays_s && *need_s && *hays_s == *need_s && size != 0)
		{
			size--;
			hays_s++;
			need_s++;
		}
		if (*need_s == '\0')
			return ((char *)haystack);
		haystack++;
		len--;
	}
	return (NULL);
}
