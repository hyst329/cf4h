#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fstream>
#include <iostream>
#include "version.h"
#include "F4Helen/AST.h"
#include "F4Helen/CGenTreeWalker.h"
#include "F4Helen/LLVMTreeWalker.h"
#include "F4Helen/LLVMExecutionEngine.h"

using namespace std;
using namespace F4Helen;
using namespace AutoVersion;

extern int yyparse(AST *&);

extern void yyerror(AST *, const char *);

extern FILE *yyin;

extern int yylex();

extern char *yytext;

enum {
    MODE_GENERATE_C,
    MODE_LLVM
};
static int mode;

static option opts[] =
        {
                {"generate",      no_argument, &mode, MODE_GENERATE_C},
                {"generate-llvm", no_argument, &mode, MODE_LLVM}
        };

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("CF4Helen v%s (%s-%s-%s)\n", FULLVERSION_STRING, YEAR, MONTH, DATE);
        printf("Usage: cf4helen [options] input-file\n");
        printf("\twhere [options] are:\n");
        printf("\t\t-g, --generate\t\tgenerate C file\n");
        printf("\t\t-l, --generate-llvm\t\tgenerate LLVM\n");
        exit(0);
    }
    printf("Running for file: %s\n", argv[argc - 1]);
    yyin = fopen(argv[argc - 1], "r");
    if (!yyin) {
        yyerror(0, "Cannot open file for reading");
        exit(1);
    }
    AST *res = 0;
    yyparse(res);
    int option_index = 0;
    while (1) {
        int c = getopt_long(argc, argv, "gl", opts, &option_index);
        switch (c) {
            case 'g': mode = MODE_GENERATE_C; break;
            case 'l': mode = MODE_LLVM; break;
        }
        if (c == -1) break;
    }
    CGenTreeWalker *cgtw;
    LLVMTreeWalker *llvmtw;
    LLVMExecutionEngine *llvmee;
    ofstream f;
    switch (mode) {
        case MODE_GENERATE_C:
            printf("Generating code...\n");
            cgtw = new CGenTreeWalker;
            f.open(argv[optind] + std::string(".cpp"));
            cgtw->codegen(res, f);
            f.close();
            delete cgtw;
            break;
        case MODE_LLVM:
            // Don't try to write into stderr!
            // FIXME Temporary solution
            freopen((argv[optind] + std::string(".ll")).c_str(), "w", stderr);
            printf("Generating JIT...\n");
            llvmtw = new LLVMTreeWalker(argv[optind]);
            llvmtw->codegen(res, cout);
            printf("Executing...\n");
            llvmee = new LLVMExecutionEngine(llvmtw->getModule());
            printf("Successfully executed returning status %d!\n", llvmee->executeCode());
            delete llvmtw;
            delete llvmee;
            break;
    }
}
