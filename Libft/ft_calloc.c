/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:10 by natalieyan        #+#    #+#             */
/*   Updated: 2025/10/30 23:45:13 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char			*ret_pt;
	unsigned char	*ptr;
	size_t			total_size;

	if (nmemb == 0 || size == 0)
		return (malloc(0));
	total_size = nmemb * size;
	ret_pt = (char *)malloc(total_size);
	if (!ret_pt)
		return (NULL);
	ptr = (unsigned char *)ret_pt;
	while (total_size--)
	{
		*ptr++ = 0;
	}
	return (ret_pt);
}
