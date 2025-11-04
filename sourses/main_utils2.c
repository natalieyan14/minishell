/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armtoros <armtoros@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 23:16:29 by armtoros          #+#    #+#             */
/*   Updated: 2025/11/03 23:26:13 by armtoros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_token	*tokenize_and_validate(char *line, int *token_count)
{
	t_token	*tokens;

	*token_count = 0;
	tokens = tokenisation(line, token_count);
	if (!tokens || if_invalid_input(tokens, *token_count) < 0)
	{
		free(line);
		free_tokens(tokens, *token_count);
		return (NULL);
	}
	return (tokens);
}

t_command	*prepare_commands(t_token *tokens, int token_count,
					t_env *env_list, int *original_stdin)
{
	t_command	*cmd_list;

	expand_dollar_vars(tokens, token_count, env_list);
	*original_stdin = process_heredocs(tokens, token_count);
	cmd_list = parse_tokens(tokens, token_count);
	return (cmd_list);
}

void	restore_stdin(int original_stdin)
{
	if (original_stdin >= 0)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
}

int	handle_input(char *line, t_env **env_list)
{
	t_token		*tokens;
	t_command	*cmd_list;
	int			token_count;
	int			original_stdin;

	if (!handle_syntax_check(line))
		return (0);
	tokens = tokenize_and_validate(line, &token_count);
	if (!tokens)
		return (0);
	cmd_list = prepare_commands(tokens, token_count,*env_list, &original_stdin);
	if (!cmd_list)
	{
		free_tokens(tokens, token_count);
		free(line);
		return (0);
	}
	execute_pipeline(cmd_list, env_list);
	restore_stdin(original_stdin);
	free_tokens(tokens, token_count);
	free_cmd_list(cmd_list);
	free(line);
	return (1);
}

int	has_unclosed_quotes(char *str)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		if (str[i] == '\'' && dq == 0)
			sq = !sq;
		else if (str[i] == '"' && sq == 0)
			dq = !dq;
		i++;
	}
	return (sq || dq);
}
