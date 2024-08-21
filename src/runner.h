#include <stdio.h>

#define MAX_PATH_LEN 1024
#define MAX_OUTPUT_SZ 4096
#define MAX_INPUT_SZ 4096

typedef struct tested_program
{
	char* tests_path;
	char* executable_path;

	char** tests;
	size_t test_cnt;
} tested_program_t;

typedef enum test_result
{
	PASSED = 1,
	FAILED = 2,
	TESTING_ERROR = 4
} test_result_t;


test_result_t run_test(tested_program_t* tp, const char* test_name);
