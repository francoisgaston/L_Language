#ifndef COMPILER_SYMBOL_TABLE_H
#define COMPILER_SYMBOL_TABLE_H

#include <stdbool.h>

#define MAX_STATES 5
#define CHUNK 5
#define INITIAL_SCOPE 'G'

typedef struct{
    char * name;
    char scope;
    int value;
} variable_info;

typedef struct{
    variable_info** variables_array;
    int variables_count;
    int array_lenght;
    char scopes[MAX_STATES];
    int index_scope;
} symbol_table;

symbol_table* init_symbol_table();

void destroy_sybol_table(symbol_table* symbol_table);

bool add_variable_symbol_table(symbol_table* symbol_table, char * name, int value);

bool exists_variable_symbol_table(symbol_table* symbol_table, char * name);

variable_info* get_variable_symbol_table(symbol_table* symbol_table, char * name);

#endif //COMPILER_SYMBOL_TABLE_H
