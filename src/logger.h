#pragma once

#include <stdio.h>
#include "runner.h"

void print_test_result(const char* test_name, test_result_t result);
void print_available_tests(tested_program_t* tp);
