#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

#include "runner.h"


static char* get_filename_wo_extension(const char* filepath)
{
	char* name = malloc(sizeof(char) * 256);
	int dot_index = strchr(filepath, '.') - filepath;

	memcpy(name, filepath, dot_index);
	name[dot_index] = 0x0; 

	return name;
}

static char* get_file_extension(const char* filepath)
{
	char* ext = malloc(sizeof(char) * 256);
	int dot_index = strchr(filepath, '.') - filepath;

	memcpy(ext, filepath + dot_index, strlen(filepath) - dot_index + 1);
	return ext;
}

static bool list_tests(tested_program_t* tp)
{
	tp->tests = malloc(sizeof(char*) * MAX_TEST_CNT);
	tp->test_cnt = 0;

	DIR *d;
	struct dirent *dir;
	d = opendir(tp->tests_path);

  	if (!d) return false;

	while ((dir = readdir(d)) != NULL) 
	{
		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
			continue;
		
		char* ext = get_file_extension(dir->d_name);
		int res = strcmp(ext, ".inp");
		free(ext);
		if (res) continue;
		
		tp->tests[tp->test_cnt++] = get_filename_wo_extension(dir->d_name);
	}
 	closedir(d);

	return true;
}

static bool read_file_to_buff(const char* filename, char* buffer)
{
	if(!buffer) return false;

	FILE *fp = fopen(filename, "r");
	char sym;

	if(fp == NULL) return false;

	size_t i = 0;
	while((sym = getc(fp)) != EOF)
	{
		if(!isprint(sym)) continue;
		buffer[i++] = sym;
	}

	fclose(fp);
	return true;
}

static bool read_test_files(tested_program_t* tp, const char* test_name, char* input_data, char* output_data)
{
	bool result = true;
	char* test_path = malloc(sizeof(char) * MAX_PATH_LEN);

	sprintf(test_path, "%s/%s.inp", tp->tests_path, test_name);
	result &= read_file_to_buff(test_path, input_data);


	printf("test_path: %s\n", test_path);
	sprintf(test_path, "%s/%s.out", tp->tests_path, test_name);
	result &= read_file_to_buff(test_path, output_data);

	free(test_path);

	return result;
}

static test_result_t run_program(const char* program_path, const char* args, const char* expected_output)
{
	char to_exec[MAX_INPUT_SZ];
	char output[MAX_OUTPUT_SZ];

	sprintf(to_exec, "./%s %s", program_path, args);
	printf("to_exec: %s\n", to_exec);
	return TESTING_ERROR;


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
	char* input_data = calloc(sizeof(char), MAX_INPUT_SZ);
	char* output_data = calloc(sizeof(char), MAX_OUTPUT_SZ);

	if(!read_test_files(tp, test_name, input_data, output_data))
	{
		free(input_data);
		free(output_data);

		printf("Can't read test files!\n");
		
		return TESTING_ERROR;
	}

	printf("input_data: %s\n", input_data);
	printf("output_data: %s\n", output_data);

	test_result_t res = run_program(tp->executable_path, input_data, output_data);

	free(input_data);
	free(output_data);

	return res;
}

void run_all_tests(tested_program_t* tp)
{

}

bool initialize(tested_program_t *tp, char* program_path, char* tests_directory)
{
	tp->executable_path = program_path;
	tp->tests_path = tests_directory;

	return list_tests(tp);
}

void destroy_tp(tested_program_t* tp)
{
	for(int i = 0; i < tp->test_cnt; i++)
	{
		free(tp->tests[i]);
	}

	free(tp->tests);
}
