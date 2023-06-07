#include <stddef.h>
#include "template.h"

#define IMPORTS "import csv\nimport sys"
#define AND_PY "def AND(a, b):\n\treturn a & b"
#define OR_PY "def OR(a, b):\n\treturn a | b"
#define NOT_PY "def NOT(a):\n\treturn int(not a)"
#define XOR_PY "def XOR(a, b):\n\treturn int(bool(a) ^ bool(b))"
#define NAND_PY "def NAND(a, b):\n\treturn int(not (AND(a, b)))"
#define NOR_PY "def NOR(a, b):\n\treturn int(not (OR(a, b)))"
#define XNOR_PY "def XNOR(a, b):\n\treturn int(not (XOR(a, b)))"
#define BUFF_PY "def BUFF(a):\n\treturn a"
#define BITARR_TO_STR_PY "def bitarr_to_str(bitarr):\n\taux = ''\n\tfor bit in bitarr:\n\t\taux += str(bit)\n\treturn aux"
#define UNION_PY "def union(* arrays):\n\tout = []\n\tfor array in arrays:\n\t\tfor elem in array:\n\t\t\tout.append(elem)\n\treturn out"

static void print_double_LF(outputADT out);
static char * get_imports();
static char * get_and();
static char * get_or();
static char * get_xor();
static char * get_not();
static char * get_nand();
static char * get_nor();
static char * get_xnor();
static char * get_buff();
static char * get_bitarr_to_str();
static char * get_union();

static char * (*template_funcs [])(void) = {
    get_imports,
    get_and,
    get_or,
    get_xor,
    get_not,
    get_nand,
    get_xnor,
    get_buff,
    get_bitarr_to_str,
    get_union,
    NULL
};

void exec_template(outputADT out) {
    for(int i = 0; template_funcs[i] != NULL; i++) {
        write_output(out, template_funcs[i]());
        print_double_LF(out);
    }
}

static void print_double_LF(outputADT out) {
    write_output(out, "\n\n");
}

static char * get_imports() {
    return IMPORTS;
}

static char * get_and() {
    return AND_PY;
}

static char * get_or() {
    return OR_PY;
}

static char * get_xor() {
    return XOR_PY;
}

static char * get_not() {
    return NOT_PY;
}

static char * get_nand() {
    return NAND_PY;
}

static char * get_nor() {
    return NOR_PY;
}

static char * get_xnor() {
    return XNOR_PY;
}

static char * get_buff() {
    return BUFF_PY;
}

static char * get_bitarr_to_str() {
    return BITARR_TO_STR_PY;
}

static char * get_union() {
    return UNION_PY;
}