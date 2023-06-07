#include "../support/logger.h"
#include "generator.h"
#include "output.h"
#include "template.h"
#include <string.h>
#include <ctype.h>

// #define CODE_TEMPLATE "import csv\nimport sys\n\ndef AND(a, b):\n\treturn a & b\n\ndef OR(a, b):\n\treturn a | b\n\ndef NOT(a):\n\treturn int(not a)\n\ndef XOR(a, b):\n\treturn int(bool(a) ^ bool(b))\n\ndef NAND(a, b):\n\treturn int(not (a & b))\n\ndef NOR(a, b):\n\treturn int(not (a | b))\n\ndef bitarr_to_str(bitarr):\n\taux = ''\n\tfor bit in bitarr:\n\t\taux += str(bit)\n\treturn aux\n\n"

/**
 * Implementación de "generator.h".
 */

void Generator(int result) {
	LogInfo("El resultado de la expresion computada es: '%d'.", result);
}

static outputADT output_info;
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
static void generateNewLineArrowNode(const new_line_arrow_node* new_line_arrow_noded);
static void generateGroupNode(const group_node* group_node);
static void generateGroupAuxNode(const group_aux_node* group_aux_node);

char* strToupper(char* str);

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

void generatorCode(const program_node * program_node){
	output_info = output_init("./salida.py", "w+");
	exec_template(output_info);
	new_line(output_info);
	generateProgramNode(program_node);
}

static void generateProgramNode(const program_node * program_node){
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
	//Definicion de la funcion para el procesador
	output("def proc_");
	output(processor_node->identifier.text);
	output("(input):");
	add_tab(output_info);
	new_line(output_info);
	output_LF("out = [1024]");
	generateBlockNode(processor_node->block_node);
	output("return out");
	reduce_tab(output_info);
}

static void generateBlockNode(const block_node* block_node){
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
	switch (line_node->line_node_type){
		case local_assigment_type:
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
	output("out[");
	output(exit_var_node->exit_var_index.text);
	output("]");
}

static void generateOperatorNode(const operator_node* operator_node){
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
	switch(argument_node->argument_node_type) {
		case identifier_argument_type:
			output(argument_node->identifier.text);
			break;
		case input_argument_type:
			output("input[");
			output(argument_node->input_variable.text);
			output("]");
			break;
		case constant_argument_type:
			output(argument_node->constant_value.text);
			break;
		default:
			printError("Invalid argument_node");
			break;
	}
}


static void generateInputNode(const input_node* input_node){
	reset_tab(output_info);
	new_line(output_info);
	output_ATAB_LF("if __name__ == '__main__':");
	output_LF("read_file = open(sys.argv[1], 'r');");
	output_LF("write_file = open(sys.argv[2], 'w');");
	output_LF("csvreader = csv.reader(read_file)");
	output_LF("csvwriter = csv.writer(write_file)");
	output_ATAB_LF("for row in csvreader:");
	output_LF("input = []");
	output_ATAB_LF("for bit in row[0]:");
	output_LF("bit = int(bit)");
	output_RTAB_LF("input.append(bit)");
	output("if(len(input) > ");
	output(input_node->integer.text);
	output_ATAB_LF("): ");
	output_RTAB_LF("exit(1)");
	output_LF("output = main_proc(input)");
	output_LF("input_str = ''");
	output_ATAB_LF("for bit in input:");
	output_RTAB_LF("input_str += str(bit)");
	output_LF("output_str = ''");
	output_ATAB_LF("for bit in output:");
	output_RTAB_LF("output_str += str(bit)");
	output_LF("csvwriter.writerow([input_str, output_str])");
	reduce_tab(output_info);

}

static void generateConnectionNode(const connection_node* connection_node){
	reset_tab(output_info);
	new_line(output_info);
	new_line(output_info);
	output_ATAB_LF("def main_proc(input):");
	output_LF("output = [1024]");
	generateArrowNode(connection_node->arrow_node,"input");
}

static void generateArrowNode(const arrow_node* arrow_node, char * var){
	switch (arrow_node->arrow_node_type)
	{
	case single_identifier_type:
		output("output = proc_");
		output(arrow_node->identifier.text);
		output("(");
		output(var);
		output_LF(")");
		generateArrowNode(arrow_node->arrow_node, "output");
		break;
	case group_identifier_type:
		output("output = ");
		generateGroupNode(arrow_node->group_node);
		generateArrowNode(arrow_node->arrow_node, "output");
		break;
	case output_identifier_type:
		output_LF("return output");
		break;
	case identifier_end_type:
		output("aux_");
		output(arrow_node->identifier.text);
		output(" = output");
		generateNewLineArrowNode(arrow_node->new_line_arrow_node);
		break;
	case group_identifier_end_type:
		//ESTADO INVALIDO?
		break;
	default:
		break;
	}
}

static void generateNewLineArrowNode(const new_line_arrow_node* new_line_arrow_node){
	switch (new_line_arrow_node->new_line_arrow_node_type)
	{
	case input_new_line_type:
		generateArrowNode(new_line_arrow_node->arrow_node, "input");
		break;
	case single_identifier_new_line_type:
		generateArrowNode(new_line_arrow_node->arrow_node, new_line_arrow_node->identifier.text);
		break;
	case group_identifier_new_line_type:
		output("output = ");
		generateGroupNode(new_line_arrow_node->group_node);
		generateArrowNode(new_line_arrow_node->arrow_node, "output");
		break;
	default:
		break;
	}
}

static void generateGroupNode(const group_node* group_node){
	output("Union(");
	generateGroupAuxNode(group_node->group_aux_node);
}

static void generateGroupAuxNode(const group_aux_node* group_aux_node){
	switch(group_aux_node->group_aux_node_type){
		case last_group_aux_type:
			output(group_aux_node->identifier.text);
			output("(output)");
			output(")");
			break;
		case common_group_aux_type:
			output(group_aux_node->identifier.text);
			output("(output), ");	
			generateGroupAuxNode(group_aux_node);
			break;
		default:
			printError("Invalid argument_node");
			break;	
	}
}


char* strToupper(char* str) {
  for(char *p=str; *p; p++){
	 *p=toupper(*p);
  }
  return str;
}
