#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

#include "runner.h"

/*void list_tests(tested_program_t* tp)*/
/*{*/
/*	DIR *d;*/
/*	struct dirent *dir;*/
/*	d = opendir(tp->tests_path);*/
/*  	if (d) */
/*	{*/
/*		while ((dir = readdir(d)) != NULL) */
/*      			printf("%s\n", dir->d_name);*/
/* 		closedir(d);*/
/*  	}*/
/*}*/

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

static bool read_test_files(tested_program_t* tp, const char* test_name, char* input_data, char* output_data)
{
	bool result = true;
	char* test_path = malloc(sizeof(char) * MAX_PATH_LEN);

	sprintf(test_path, "%s/%s.in", tp->tests_path, test_name);
	result &= read_file_to_buff(test_path, input_data);

	sprintf(test_path, "%s/%s.out", tp->tests_path, test_name);
	result &= read_file_to_buff(test_path, output_data);

	free(test_path);

	return result;
}

static test_result_t run_program(const char* program_path, const char* args, const char* expected_output)
{
	char to_exec[MAX_INPUT_SZ];
	char output[MAX_OUTPUT_SZ];

	sprintf(to_exec, "%s %s", program_path, args);

	FILE *fp = popen(to_exec, "r");
	if (fp == 0)
	{
		fprintf(stderr, "failed to create pipeline\n");
		return TESTING_ERROR;
	}
	size_t nbytes = fread(output, sizeof(char), sizeof(output), fp);

	pclose(fp);
	if (nbytes == 0 && strlen(expected_output) == 0)
	{
		printf("no output available!\n");
		return TESTING_ERROR;
	}

	if (strcmp(output, expected_output) != 0)
	{
		printf("output doesn't match the expected one!\n");
		return FAILED;
	}

	return PASSED;
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
		
		return TESTING_ERROR;
	}

	test_result_t res = run_program(tp->executable_path, input_data, output_data);

	free(input_data);
	free(output_data);

	return res;
}
