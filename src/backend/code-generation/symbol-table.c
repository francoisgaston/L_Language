#include "symbol-table.h"
#include "../support/logger.h"
#include <stdlib.h>
#include <string.h>
#include "../support/garbage_collector.h"

static symbol_table* symbol_table_info;

void add_data_table_symbol(char *name, int input, int output, bool is_proc);

void init_symbol_table(){
    symbol_table_info = Malloc(sizeof(symbol_table));
    if(symbol_table_info == NULL){
        LogError("Not memory available");
        Free_All();
        exit(1);
    }
    symbol_table_info->variables_array = Calloc(CHUNK, sizeof(variable_info));
    if(symbol_table_info->variables_array == NULL){
        LogError("Not memory available");
        Free_All();
        exit(1);
    }
    symbol_table_info->index_scope=0;
    symbol_table_info->scopes[symbol_table_info->index_scope] = INITIAL_SCOPE;
    symbol_table_info->array_lenght = CHUNK;
    symbol_table_info->variables_count = 0;
}

void add_variable_symbol_table(char * name) {
    add_data_table_symbol(name,0, 0, false);
}

void add_proc_symbol_table(char * name, int input, int output){
    add_data_table_symbol(name, input, output, true);
}

void create_scope(){
    symbol_table* symbol_table = symbol_table_info;
    symbol_table->index_scope++;
    symbol_table->scopes_symbol++;
    symbol_table->scopes[symbol_table->index_scope] =symbol_table->scopes_symbol;
}

void remove_scope(){
    symbol_table* symbol_table = symbol_table_info;
    symbol_table->index_scope--;
}

bool exists_variable_symbol_table(char * name){
    for(int i=0; i<symbol_table_info->variables_count; i++){
        if(!symbol_table_info->variables_array[i]->is_proc &&symbol_table_info->scopes[symbol_table_info->index_scope] == symbol_table_info->variables_array[i]->scope
           && strcmp(symbol_table_info->variables_array[i]->name, name) == 0){
            return true;
        }
    }
    return false;
}

bool exists_proc_symbol_table(char * name){
    for(int i=0; i<symbol_table_info->variables_count; i++){
        if(symbol_table_info->variables_array[i]->is_proc && symbol_table_info->scopes[symbol_table_info->index_scope] == symbol_table_info->variables_array[i]->scope
           && strcmp(symbol_table_info->variables_array[i]->name, name) == 0){
            return true;
        }
    }
    return false;
}

int get_input(char * name){
    for(int i=0; i<symbol_table_info->variables_count; i++){
        if(strcmp(symbol_table_info->variables_array[i]->name, name) == 0){
            return symbol_table_info->variables_array[i]->input;
        }
    }
    return false;
}

int get_output(char * name){
    for(int i=0; i<symbol_table_info->variables_count; i++){
        if(strcmp(symbol_table_info->variables_array[i]->name, name) == 0){
            return symbol_table_info->variables_array[i]->output;
        }
    }
    return false;
}

//TODO: Preguntar fuertemente
void set_input_output_var(char* name, unsigned int value){
    for(int i=0; i<symbol_table_info->variables_count; i++){
        if(strcmp(symbol_table_info->variables_array[i]->name, name) == 0){
            symbol_table_info->variables_array[i]->input = value;
            symbol_table_info->variables_array[i]->output = value;
        }
    }
}


void add_data_table_symbol(char *name, int input, int output, bool is_proc){
    if(symbol_table_info->variables_count == symbol_table_info->array_lenght){
        symbol_table_info->array_lenght+=CHUNK;
        symbol_table_info->variables_array = Realloc(symbol_table_info->variables_array, symbol_table_info->array_lenght);
        if(symbol_table_info->variables_array == NULL) {
            LogError("Cannot reallocate space for symbol table");
            Free_All();
            exit(1);
        }
    }
    symbol_table_info->variables_array[symbol_table_info->variables_count] = Malloc(sizeof(variable_info));
    if(symbol_table_info->variables_array[symbol_table_info->variables_count] == NULL){
        LogError("Not memory available");
        Free_All();
        exit(1);
    }
    symbol_table_info->variables_array[symbol_table_info->variables_count]->name= name;
    symbol_table_info->variables_array[symbol_table_info->variables_count]->input=input;
    symbol_table_info->variables_array[symbol_table_info->variables_count]->output=output;
    symbol_table_info->variables_array[symbol_table_info->variables_count]->is_proc = is_proc;
    symbol_table_info->variables_array[symbol_table_info->variables_count]->scope=symbol_table_info->scopes[symbol_table_info->index_scope];
    symbol_table_info->variables_count++;
}


