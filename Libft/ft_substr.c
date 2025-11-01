/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:50:24 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 23:50:26 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret_s;
	size_t	s_len;
	size_t	substr_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		ret_s = (char *)malloc(1);
		if (!ret_s)
			return (NULL);
		*ret_s = '\0';
		return (ret_s);
	}
	substr_len = s_len - start;
	if (substr_len > len)
		substr_len = len;
	ret_s = (char *)malloc(substr_len + 1);
	if (!ret_s)
		return (NULL);
	ft_memcpy(ret_s, s + start, substr_len);
	ret_s[substr_len] = '\0';
	return (ret_s);
}
