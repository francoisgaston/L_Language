#include "symbol-table.h"
#include <stdlib.h>
#include <string.h>

symbol_table* init_symbol_table(){
    symbol_table* table = malloc(sizeof(symbol_table));
    if(table == NULL){
        return NULL;
    }
    table->variables_array = calloc(CHUNK, sizeof(variable_info));
    if(table->variables_array == NULL){
        free(table);
        return NULL;
    }
    table->index_scope=0;
    table->scopes[table->index_scope] = INITIAL_SCOPE;
    table->array_lenght = CHUNK;
    table->variables_count = 0;
    return table;
}

void destroy_sybol_table(symbol_table* symbol_table){
    for(int i=0; i<symbol_table->variables_count; i++){
        free(symbol_table->variables_array[i]->name);
        free(symbol_table->variables_array[i]);
    }
    free(symbol_table);
}

bool add_variable_symbol_table(symbol_table* symbol_table, char * name, int value){
    symbol_table->variables_count++;
    if(symbol_table->variables_count == symbol_table->array_lenght){
        symbol_table->array_lenght+=CHUNK;
        symbol_table->variables_array = realloc(symbol_table->variables_array, symbol_table->array_lenght);
    }
    symbol_table->variables_array[symbol_table->variables_count] = malloc(sizeof(variable_info));
    symbol_table->variables_array[symbol_table->variables_count]->name=name;
    symbol_table->variables_array[symbol_table->variables_count]->value=value;
    symbol_table->variables_array[symbol_table->variables_count]->scope=symbol_table->scopes[symbol_table->index_scope];
    return true;
}

bool exists_variable_symbol_table(symbol_table* symbol_table, char * name){
    for(int i=0; i<symbol_table->array_lenght; i++){
        if(symbol_table->scopes[symbol_table->index_scope] == symbol_table->variables_array[i]->scope
            && strcmp(symbol_table->variables_array[i]->name, name) == 0){
            return true;
        }
    }
    return false;
}

variable_info* get_variable_symbol_table(symbol_table* symbol_table, char * name){
    for(int i=0; i<symbol_table->array_lenght; i++){
        if(symbol_table->scopes[symbol_table->index_scope] == symbol_table->variables_array[i]->scope
           && strcmp(symbol_table->variables_array[i]->name, name) == 0){
            return symbol_table->variables_array[i];
        }
    }
    return NULL;
}
