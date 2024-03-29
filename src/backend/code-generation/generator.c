#include "../support/logger.h"
#include "generator.h"
#include "output.h"
#include "template.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "symbol-table.h"
#include "../support/garbage_collector.h"

/**
 * Implementación de "generator.h".
 */

static outputADT output_info;
//FUNCIONES PARA RECORRER EL ARBOL
static void generateProgramNode(const program_node * program_node);
static void generateProcessorNode(const processor_node* processor_node);
static void generateBlockNode(const block_node* block_node);
static void generateLineNode(const line_node* line_node);
static void generateExitVarNode(const exit_var_node* exit_var_node);
static void generateOperatorNode(const operator_node* operator_node);
static void generateArgumentNode(const argument_node* argument_node);
static void generateInputNode(const input_node* input_node);
static void generateConnectionNode(const connection_node* connection_node);
static void generateArrowNode(const arrow_node* arrow_node,char* var);
static void generateGroupAuxNode(const group_aux_node* group_aux_node, char* var);
static void generateGroupNode(const group_node* group_node, char* var);
static void generateNewLineArrowNode(const new_line_arrow_node* new_line_arrow_node, char* var);
static void generateGroupVarNode(const group_var_node* group_var_node);
static void generateGroupAuxVarNode(const group_aux_var_node * group_aux_var_node);
//FUNCIONES AUXILIARES
void output(char * str);
void output_LF(char * str);
void output_ATAB_LF(char * str);
void output_RTAB_LF(char * str);
void printError(char * err);
char* strToupper(char* str);


void generatorCode(const program_node * program_node, const char * filename){
	LogDebug("Entre a generatorCode");
    if (program_node == NULL) {
        LogError("Invalid program_node");
        return;
    }
    if (filename == NULL || strcmp(filename, "") == 0) {
        LogDebug("No se especifico un nombre de archivo, se utilizara el nombre por defecto: '%s'", DEFAULT_FILENAME);
        filename = DEFAULT_FILENAME;
    }
	output_info = output_init(filename, "w+");
	exec_template(output_info);
	new_line(output_info);
    generateProgramNode(program_node);
}

static void generateProgramNode(const program_node * program_node){
	LogDebug("Entre a generateProgramNode");
	switch (program_node->program_node_type){
		case processor_node_type:
			generateProcessorNode(program_node->processor_node);
			generateProgramNode(program_node->program_node);
			break;
		case connection_node_type:
			generateConnectionNode(program_node->connection_node);
			generateInputNode(program_node->input_node);
			break;
		default:
			printError("Invalid program_node");	
			break;	
	}
}

static void generateProcessorNode(const processor_node* processor_node){
    char aux [10];
	LogDebug("Entre a generateProcessorNode");
	output("def proc_");
	output(processor_node->identifier.text);
	output("(input):");
	add_tab(output_info);
	new_line(output_info);
	output("out = [2]*");
    sprintf(aux, "%d", get_output(processor_node->identifier.text));
    output_LF(aux);
	generateBlockNode(processor_node->block_node);
	output_RTAB_LF("return out");
	new_line(output_info);
	reset_tab(output_info);
}

static void generateBlockNode(const block_node* block_node){
	LogDebug("Entre a generateBlockNode");
	switch (block_node->block_node_type) {
		case multi_line_block_type:
			generateLineNode(block_node->line_node);
			generateBlockNode(block_node->block_node);
			break;
		case single_line_block_type:
			generateLineNode(block_node->line_node);
			break;
		default:
			printError("Invalid block_node");
			break;
	}
}

static void generateLineNode(const line_node* line_node){
	LogDebug("Entre a generateLineNode");
	switch (line_node->line_node_type){
		case local_assigment_type:
            output("var_");
			output(line_node->identifier.text);
			output(" = ");
			generateOperatorNode(line_node->operator_node);
			break;
		case exit_assigment_type:
			generateExitVarNode(line_node->exit_var_node);
			output(" = ");
			generateOperatorNode(line_node->operator_node);
			break;
		default:
			printError("Invalid line_node");
			break;
	}
}

static void generateExitVarNode(const exit_var_node* exit_var_node){
	LogDebug("Entre a generateExitVarNode");
	output("out[");
	output(exit_var_node->exit_var_index.text);
	output("]");
}

static void generateOperatorNode(const operator_node* operator_node){
	LogDebug("Entre a generateOperatorNode");
	char * aux;
	switch (operator_node->operator_node_type)
	{
	case binary_operator_type:
		aux = strToupper(operator_node->binary_operator.op);
		output(aux);
		output(" ( ");
		generateArgumentNode(operator_node->argument_node_1);
		output(" , ");
		generateArgumentNode(operator_node->argument_node_2);
		output(" )");
		new_line(output_info);
		break;
	
	case unary_operator_type:
		aux = strToupper(operator_node->unary_operator.op);
		output(aux);
		output(" ( ");
		generateArgumentNode(operator_node->argument_node_1);
		output(" )");
		new_line(output_info);
		break;

	default:
		printError("Invalid operator_node");
		break;
	}
	
}

static void generateArgumentNode(const argument_node* argument_node){
	LogDebug("Entre a generateArgumentNode");
	switch(argument_node->argument_node_type) {
		case identifier_argument_type:
            output("var_");
			output(argument_node->identifier.text);
			break;
		case input_argument_type:
			output("input[");
			output(argument_node->input_variable.text);
			output("]");
			break;
		case constant_argument_type:
			if(argument_node->constant_value.value){
				output("True");
			}else{
				output("False");
			}
			break;
		default:
			printError("Invalid argument_node");
			break;
	}
}


static void generateInputNode(const input_node* input_node){
	LogDebug("Entre a generateInputNode");
	reset_tab(output_info);
	new_line(output_info);
	output_ATAB_LF("if __name__ == '__main__':");
	output_ATAB_LF("for line in fileinput.input():");
    output_LF("line = line.rstrip()");
	output_LF("input = []");
	output_ATAB_LF("for bit in line:");
	output_LF("bit = int(bit)");
	output_RTAB_LF("input.append(bit)");
	output("if(len(input) < ");
	output(input_node->integer.text);
	output_ATAB_LF("): ");
	output("print('Missing bits in the array, must be ");
	output(input_node->integer.text);
	output_LF("', file = sys.stderr)");
	output_RTAB_LF("exit(1)");
	output_LF("output = main_proc(input)");
	output_LF("output_str = bitarr_to_str(output)");
	output_LF("print(output_str)");
	reduce_tab(output_info);

}

static void generateConnectionNode(const connection_node* connection_node){
	LogDebug("Entre a generateConnectionNode");
	reset_tab(output_info);
	new_line(output_info);
	new_line(output_info);
	output_ATAB_LF("def main_proc(input):");
	output_LF("output = [2]*1024");
	generateArrowNode(connection_node->arrow_node,"input");
}

static void generateArrowNode(const arrow_node* arrow_node, char * var){
	LogDebug("Entre a generateArrowNode");
	switch (arrow_node->arrow_node_type)
	{
	case single_identifier_type:
		LogDebug("Entre en single_identifier_type");
		output("output = proc_");
		output(arrow_node->identifier.text);
		output("(");
		output(var);
		output_LF(").copy()");
		generateArrowNode(arrow_node->arrow_node, "output");
		break;
	case group_identifier_type:
		LogDebug("Entre en grup_identifier_type");
		output("output = ");
		generateGroupNode(arrow_node->group_node,var);
		generateArrowNode(arrow_node->arrow_node, "output");
		break;
	case output_identifier_type:
		LogDebug("Entre en output_identifier_type");
		output_LF("salida = []");
        output_ATAB_LF("for bit in output:");
        output_ATAB_LF("if bit == 2:");
        output_RTAB_LF("break");
        output_RTAB_LF("salida.append(bit)");
        output_RTAB_LF("return salida");
		break;
	case identifier_end_type:
		LogDebug("Entre en identifier_end_type");
        output("var_");
		output(arrow_node->identifier.text);
		output_LF(" = output.copy()");
		generateNewLineArrowNode(arrow_node->new_line_arrow_node, "output");
		break;
	default:
		break;
	}
}

static void generateNewLineArrowNode(const new_line_arrow_node* new_line_arrow_node, char* var){
	LogDebug("Entre a generateNewLineArrowNode");
	switch (new_line_arrow_node->new_line_arrow_node_type)
	{
	case input_new_line_type:
		LogDebug("Entre en input_new_line_type");
		generateArrowNode(new_line_arrow_node->arrow_node, "input");
		break;
	case single_identifier_new_line_type:
		LogDebug("Entre en single_identifier_new_line_type");
        unsigned int len = strlen(new_line_arrow_node->identifier.text);
        char*  aux = Calloc(len+6,sizeof(char));
        if(aux == NULL) {
            LogError("Cannot generate SingleIdentifierNewLine Node");
            Free_All();
            exit(1);
        }
        snprintf(aux,len+5,"var_%s", new_line_arrow_node->identifier.text);
		generateArrowNode(new_line_arrow_node->arrow_node, aux);
		break;
	case group_identifier_new_line_type:
		LogDebug("Entre en group_identifier_new_line_type");
		output("output = ");
		generateGroupVarNode(new_line_arrow_node->group_var_node);
		generateArrowNode(new_line_arrow_node->arrow_node, "output");
		break;
	default:
		break;
	}
}

static void generateGroupVarNode(const group_var_node* group_var_node){
    LogDebug("Entre en generateGroupVarNode");
    output("union( ");
    output("var_");
    output(group_var_node->identifier.text);
    output(", ");
    generateGroupAuxVarNode(group_var_node->group_aux_node);
}

static void generateGroupAuxVarNode(const group_aux_var_node * group_aux_var_node){
    switch(group_aux_var_node->group_aux_node_type){
        case last_group_aux_type:
            LogDebug("Entre en last_group_aux_type");
            output("var_");
            output(group_aux_var_node->identifier.text);
            output_LF(" )");
            break;
        case common_group_aux_type:
            LogDebug("Entre en common_group_aux_type");
            output("var_");
            output(group_aux_var_node->identifier.text);
            output_LF(" , ");
            generateGroupAuxVarNode(group_aux_var_node->group_aux_node);
            break;
        default:
            printError("Invalid argument_node");
            break;
    }
}

static void generateGroupNode(const group_node* group_node, char* var){
	LogDebug("Entre en generateGroupNode");
	output("union(");
	output("proc_");
	output(group_node->identifier.text);
	output("(");	
	output(var);
	output("), ");
	generateGroupAuxNode(group_node->group_aux_node,var);
}

static void generateGroupAuxNode(const group_aux_node* group_aux_node, char* var){
	switch(group_aux_node->group_aux_node_type){
		case last_group_aux_type:
			LogDebug("Entre en last_group_aux_type");
			output("proc_");
			output(group_aux_node->identifier.text);
			output("(");	
			output(var);
			output(")");
			output_LF(").copy()");
			break;
		case common_group_aux_type:
			LogDebug("Entre en common_group_aux_type");
			output("proc_");
			output(group_aux_node->identifier.text);
			output("(");	
			output(var);
			output("), ");
			generateGroupAuxNode(group_aux_node->group_aux_node, var);
			break;
		default:
			printError("Invalid argument_node");
			break;	
	}
}

// FUNCIONES AUXILIARES
void output(char * str){
    write_output(output_info, str);
}

void output_LF(char * str) {
    write_output(output_info, str);
    new_line(output_info);
}

void output_ATAB_LF(char * str) {
    write_output(output_info, str);
    add_tab(output_info);
    new_line(output_info);
}

void output_RTAB_LF(char * str) {
    write_output(output_info, str);
    reduce_tab(output_info);
    new_line(output_info);
}

void printError(char * err) {
    LogError("Invalid AST: '%s'", err);
}

char* strToupper(char* str) {
    for(char *p=str; *p; p++){
        *p=toupper(*p);
    }
    return str;
}

