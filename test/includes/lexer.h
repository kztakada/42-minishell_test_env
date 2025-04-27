#ifndef LEXER_H
# define LEXER_H
// tester
# include "unity.h"
// support
# include "common_error_exit.h"
# include "t_minishell.h"

int		lexer(char *input, t_list **token_list);
void	free_token(void *target);

#endif