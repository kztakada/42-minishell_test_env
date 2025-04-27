#ifndef LEXER_H
# define LEXER_H
// tester
# include "unity.h"
// support
# include "common_error_exit.h"
# include "t_minishell.h"

t_list	*lexer(char *input);
void	free_token(void *target);

#endif