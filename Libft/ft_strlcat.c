/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:49:27 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 23:49:28 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = 0;
	src_len = 0;
	while (dst_len < dsize && dst[dst_len])
		dst_len++;
	while (src[src_len])
		src_len++;
	if (dst_len >= dsize)
		return (dsize + src_len);
	i = 0;
	while (dst_len + i + 1 < dsize && src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (dst_len + i < dsize)
		dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}
