#ifndef CHECK_GRAMMAR_H
# define CHECK_GRAMMAR_H
// tester
# include "unity.h"
// support
# include "common_error_exit.h"
# include "t_minishell.h"

int	check_tokens_grammar(t_list **current_tokens, int *subshell_count);

#endif