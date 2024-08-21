#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include "runner.h"
#include "logger.h"


int main(int argc, char** argv)
{
	tested_program_t tp;
	if(!initialize(&tp, "test", "examples"))
	{
		printf("can't initialize testing framework!\n");
		return 1;
	}

	printf("succesfully initialized tests!\n");
	print_available_tests(&tp);

	
	run_all_tests(&tp);

	destroy_tp(&tp);
	return 0;
}
