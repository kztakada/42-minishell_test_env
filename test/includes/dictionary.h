#ifndef DICTIONARY_H
# define DICTIONARY_H
// tester
# include "unity.h"
// support
# include "common_error_exit.h"
# include "t_minishell.h"

int			ft_strncmp(const char *s1, const char *s2, size_t n);
t_dict_out	lookup_dict(char *subject, char *dict);

#endif