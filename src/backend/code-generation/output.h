#ifndef __OUTPUT_H__
#define __OUTPUT_H__


typedef struct outputCDT * outputADT;

outputADT output_init(const char * file_path, const char * file_mode);

void write_output(outputADT out, const char * str);

void add_tab(outputADT out);

void reduce_tab(outputADT out);

void new_line(outputADT out);

void reset_tab(outputADT out);

#endif