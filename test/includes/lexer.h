#ifndef LEXER_H
# define LEXER_H
// tester
# include "unity.h"
// support
# include "common_error_exit.h"
# include "t_minishell.h"

t_list	*lexer(char *input);
void	delete_token(void *target);

#endif