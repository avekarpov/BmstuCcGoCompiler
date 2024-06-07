#include <fstream>

#include "llvm_visitor.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    // std::string input_path = "/Users/artem.karpov/University/Sem2/Cc/CourseWork/GoCompiler/fib.go";
    // std::string output_path = "/Users/artem.karpov/University/Sem2/Cc/CourseWork/GoCompiler/tmp/fib.ll";

    std::ifstream fin(input_path);
    
    if (fin.is_open()) {
        antlr4::ANTLRInputStream input_stream { fin };
        GoLexer lexer { &input_stream };
        antlr4::CommonTokenStream tokens { &lexer };

        tokens.fill();

        GoParser parser { &tokens };

        LlvmVisitor visitor;
        visitor.visit(parser.sourceFile());
        
        std::error_code err;
        auto os = llvm::raw_fd_ostream(output_path, err);
        visitor.print(os, nullptr);

        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}
