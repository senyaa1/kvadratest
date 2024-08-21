#include "logger.h"

void print_test_result(const char* test_name, test_result_t result)
{
	printf("%s: ", test_name);
	switch(result)
	{
		case PASSED:
			printf("PASSED!\n");
			break;
		case FAILED:
			printf("FAILED!\n");
			break;
		default:
		case TESTING_ERROR:
			printf("Error during testing! Verify test files!\n");
			break;
	}
}

void print_available_tests(tested_program_t* tp)
{
	printf("Available tests (%ld): \n", tp->test_cnt);
	
	for(int i = 0; i < tp->test_cnt; i++)
	{
		printf("\t%s\n", tp->tests[i]);
	}
}
