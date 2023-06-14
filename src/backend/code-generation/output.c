#include "output.h"
#include <stdint.h>
#include <stdlib.h>
#include "../support/logger.h"
#include "../support/garbage_collector.h"

struct outputCDT {
    FILE * program;
    uint8_t tab_count;
};

outputADT output_init(const char * file_path, const char * file_mode) {
    outputADT out = Calloc(1, sizeof(struct outputCDT));
    if(out == NULL) {
        LogError("Cannot create outputADT");
        Free_All();
        return out;
    }
    out->program = fopen(file_path, file_mode);
    if(out->program == NULL) {
        LogError("Cannot open file '%s' with mode '%s'", file_path, file_mode);
        goto error;
    }
    out->tab_count = 0;
    return out;

error:
    Free_All();
    return NULL;
}

void write_output(outputADT out, const char * str){
    if(out == NULL){
        LogError("Error in outADT");
    }
    //for(int i=0; i!=out->tab_count; i++){
    //    fprintf(out->program, "\t");
    //}
    fprintf(out->program, "%s", str);
}

void new_line(outputADT out){
    fprintf(out->program, "\n");
    for(int i=0; i!=out->tab_count; i++){
        fprintf(out->program, "\t");
    }
}

void reset_tab(outputADT out){
        if(out == NULL){
        LogError("Error in outADT");
    }
    out->tab_count=0;
}

void add_tab(outputADT out){
    if(out == NULL){
        LogError("Error in outADT");
    }
    out->tab_count++;
}

void reduce_tab(outputADT out){
        if(out == NULL){
        LogError("Error in outADT");
    }
    out->tab_count--;
}

