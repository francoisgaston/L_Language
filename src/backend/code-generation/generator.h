#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"

#define DEFAULT_FILENAME "salida.py"


void generatorCode(const program_node * program_node, const char * filename);

#endif
