/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:08:47 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:08:49 by nharutyu         ###   ########.fr       */
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
