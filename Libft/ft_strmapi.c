/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nharutyu <nharutyu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:07:41 by nharutyu          #+#    #+#             */
/*   Updated: 2025/11/13 17:07:43 by nharutyu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*result;
	char			*work_result;
	size_t			s_len;
	unsigned int	i;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	result = (char *)malloc((s_len +1) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	work_result = result;
	while (*s)
	{
		*work_result++ = f(i++, *s++);
	}
	*work_result = '\0';
	return (result);
}
