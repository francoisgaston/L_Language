#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"

void Generator(int result);

void generatorCode(const program_node * program_node);


void output(char * str);
#endif
