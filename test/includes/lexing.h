#ifndef LEXING_H
# define LEXING_H
// tester
# include "unity.h"
// support
# include "common_error_exit.h"
# include "t_minishell.h"

t_list	*lexer(char *input);
void	delete_token(void *target);

#endif