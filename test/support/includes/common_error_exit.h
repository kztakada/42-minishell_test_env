#ifndef COMMON_ERROR_EXIT_H
# define COMMON_ERROR_EXIT_H

// tester
# include "unity.h"

void	forced_error_exit_ABORT(const char *err_msg, const char *file, int line,
			int count);

#endif