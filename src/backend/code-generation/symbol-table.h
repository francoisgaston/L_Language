#ifndef COMPILER_SYMBOL_TABLE_H
#define COMPILER_SYMBOL_TABLE_H

#include <stdbool.h>

#define INITIAL_SCOPE 0
#define MAX_STATES 3
#define CHUNK 30


typedef struct{
    char * name;
    int scope;
    int input;
    int output;
    bool is_proc;
} variable_info;

typedef struct{
    variable_info** variables_array;
    int variables_count;
    int array_lenght;
    int scopes[MAX_STATES];
    int index_scope;
    int scopes_symbol;
} symbol_table;

int get_input(char * name);

int get_output(char * name);

void init_symbol_table();

void add_variable_symbol_table(char * name);

void add_proc_symbol_table(char * name, int input, int output);

bool exists_variable_symbol_table(char * name);

bool exists_proc_symbol_table(char * name);

void create_scope();

void remove_scope();

void set_input_output_var(char* name, unsigned int value);

#endif //COMPILER_SYMBOL_TABLE_H
