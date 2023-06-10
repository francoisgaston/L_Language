#ifndef COMPILER_SYMBOL_TABLE_H
#define COMPILER_SYMBOL_TABLE_H

#include <stdbool.h>

#define MAX_STATES 3
#define CHUNK 20
#define INITIAL_SCOPE 0

typedef struct{
    char * name;
    int scope;
    int columns;
} variable_info;

typedef struct{
    variable_info** variables_array;
    int variables_count;
    int array_lenght;
    int scopes[MAX_STATES];
    int index_scope;
    int scopes_symbol;
} symbol_table;


void init_symbol_table();

void destroy_symbol_table();

bool add_variable_symbol_table(char * name, int value);

bool exists_variable_symbol_table(char * name);

variable_info* get_variable_symbol_table(char * name);

void create_scope();

void remove_scope();

#endif //COMPILER_SYMBOL_TABLE_H
