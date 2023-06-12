#include "symbol-table.h"
#include "../support/logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static symbol_table* symbol_table_info;

void add_data_table_symbol(char *name, int columns, bool is_proc);


void init_symbol_table(){
    symbol_table_info = malloc(sizeof(symbol_table));
    if(symbol_table_info == NULL){
        LogError("Not memory available\n");
        exit(1);
    }
    symbol_table_info->variables_array = calloc(CHUNK, sizeof(variable_info));
    if(symbol_table_info->variables_array == NULL){
        LogError("Not memory available\n");
        free(symbol_table_info);
        exit(1);
    }
    symbol_table_info->index_scope=0;
    symbol_table_info->scopes[symbol_table_info->index_scope] = INITIAL_SCOPE;
    symbol_table_info->array_lenght = CHUNK;
    symbol_table_info->variables_count = 0;
}

void destroy_symbol_table(){
    symbol_table* symbol_table = symbol_table_info;
    for(int i=0; i<symbol_table->variables_count; i++){
        free(symbol_table->variables_array[i]->name);
        free(symbol_table->variables_array[i]);
    }
    free(symbol_table);
}

void add_variable_symbol_table(char * name, int column) {
    add_data_table_symbol(name, column, false);
}

void add_proc_symbol_table(char * name, int columns){
    add_data_table_symbol(name, columns, true);
}

void create_scope(){
    symbol_table* symbol_table = symbol_table_info;
    symbol_table->index_scope++;
    printf("%d", symbol_table->index_scope);
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

void add_data_table_symbol(char *name, int columns, bool is_proc){
    if(symbol_table_info->variables_count == symbol_table_info->array_lenght){
        symbol_table_info->array_lenght+=CHUNK;
        symbol_table_info->variables_array = realloc(symbol_table_info->variables_array, symbol_table_info->array_lenght);
    }
    symbol_table_info->variables_array[symbol_table_info->variables_count] = malloc(sizeof(variable_info));
    if(symbol_table_info->variables_array[symbol_table_info->variables_count] == NULL){
        LogError("Not memory available\n");
        destroy_symbol_table();
        exit(1);
    }
    symbol_table_info->variables_array[symbol_table_info->variables_count]->name= name;
    symbol_table_info->variables_array[symbol_table_info->variables_count]->columns=columns;
    symbol_table_info->variables_array[symbol_table_info->variables_count]->is_proc = is_proc;
    symbol_table_info->variables_array[symbol_table_info->variables_count]->scope=symbol_table_info->scopes[symbol_table_info->index_scope];
    symbol_table_info->variables_count++;

    for(int i=0; i<symbol_table_info->variables_count; i++){
        printf("nombre: %s:\n", symbol_table_info->variables_array[i]->name);
        printf("scope: %d\n\n\n", symbol_table_info->variables_array[i]->scope);
    }
}


