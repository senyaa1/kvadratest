#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>

#define MAX_PATH_LEN 1024

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
	TEST_FAIL = 4
} test_result_t;

static bool read_file_to_buff(const char* filename, char* buffer)
{
	long length;
	FILE * f = fopen(filename, "rb");

	if (f)
	{
  		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
  		buffer = malloc(length);

  		if (!buffer)
    			fread (buffer, 1, length, f);

  		fclose (f);
	}

	if (!buffer)
		return false;

	return true;
}


void list_tests(tested_program_t* tp)
{
	DIR *d;
	struct dirent *dir;
	d = opendir(tp->tests_path);
  	if (d) 
	{
		while ((dir = readdir(d)) != NULL) 
      			printf("%s\n", dir->d_name);
 		closedir(d);
  	}
}

bool read_test_files(tested_program_t* tp, const char* test_name, char* input_data, char* output_data)
{
	bool result = true;
	char* test_path = malloc(sizeof(char) * MAX_PATH_LEN);

	sprintf(test_path, "%s/%s.in", tp->tests_path, test_name);
	result &= read_file_to_buff(test_path, input_data))

	sprintf(test_path, "%s/%s.out", tp->tests_path, test_name);
	result &= read_file_to_buff(test_path, output_data);

	free(test_path);

	return result;
}

test_result_t run_test(tested_program_t* tp, const char* test_name)
{
	char* input_data;
	char* output_data;

	if(!read_test_files(tp, test_name, input_data, output_data))
	{
		free(input_data);
		free(output_data);

		printf("Can't read test files!\n");
		
		return TEST_FAIL;
	}



	free(input_data);
	free(output_data);
}

int main(int argc, char** argv)
{
	return 0;
}
