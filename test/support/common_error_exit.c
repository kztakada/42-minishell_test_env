#include "common_error_exit.h"

void	forced_error_exit_ABORT(const char *err_msg, const char *file, int line,
		int count)
{
	printf("run callback: forced_error_exit_ABORT\n");
	TEST_ABORT();
}
