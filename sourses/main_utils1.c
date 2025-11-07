/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armtoros <armtoros@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 23:15:48 by armtoros          #+#    #+#             */
/*   Updated: 2025/11/03 23:15:58 by armtoros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	toggle_quotes(char c, int *sq, int *dq)
{
	if (c == '\'' && *dq == 0)
		*sq = !(*sq);
	else if (c == '"' && *sq == 0)
		*dq = !(*dq);
}

int	is_forbidden_char(char c)
{
	if (c == '(' || c == ')' || c == '{' || c == '}')
		return (1);
	return (0);
}

int	report_syntax_error(void)
{
	write(STDERR_FILENO,
		"minishell: syntax error near unexpected token\n", 46);
	set_exit_status(2);
	return (1);
}

int	has_syntax_errors(char *str)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		toggle_quotes(str[i], &sq, &dq);
		if (!sq && !dq && is_forbidden_char(str[i]))
			return (report_syntax_error());
		i++;
	}
	return (0);
}

int	handle_syntax_check(char *line)
{
	if (has_syntax_errors(line))
	{
		free(line);
		return (0);
	}
	return (1);
}
