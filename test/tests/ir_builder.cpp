#include <catch2/catch_all.hpp>

#include "llvm_visitor.hpp"

void check(
    const std::string &string,
    const std::vector<std::string> &patterns
) {
    std::size_t pos = string.find(patterns.front());
    for (const auto &pattern : patterns) {
        REQUIRE(string.substr(pos, pattern.size()) == pattern);
        pos += pattern.size();
        pos = string.find_first_of("\n", pos) + 1;
        while (string[pos] == ' ' | string[pos] == '\n') { ++pos; }
    }
}

std::string getIr(const std::string_view code) {
    std::string ir;

    antlr4::ANTLRInputStream input_stream { code };
    GoLexer lexer { &input_stream };
    
    antlr4::CommonTokenStream tokens { &lexer };
    tokens.fill();

    GoParser parser { &tokens };

    LlvmVisitor visitor;
    visitor.visit(parser.sourceFile());

    auto os = llvm::raw_string_ostream(ir);
    visitor.print(os, nullptr);

    return ir;
}

TEST_CASE("Function generation") {
    const std::string code = R"(
        package main

        func main() int {
            return 0
        }
    )";
    
    auto ir = getIr(code);

    check(
        ir,
        {
            "define i32 @main() {",
            "entry:",
                "%0 = alloca i32",
                "store i32 0, ptr %0",
                "br label %exit",
            "exit:",
                "%1 = load i32, ptr %0",
                "ret i32 %1",
            "}"
        }
    );
}

TEST_CASE("Var declaration and assignment generation") {
    const std::string code = R"(
        package main

        func main() int {
            var i int = 100
            return 0
        }
    )";
    
    auto ir = getIr(code);

    check(
        ir,
        {
            "define i32 @main() {",
            "entry:",
                "%0 = alloca i32",
                "%i = alloca i32",
                "store i32 100, ptr %i",
                "store i32 0, ptr %0",
                "br label %exit",
            "exit:",
                "%1 = load i32, ptr %0",
                "ret i32 %1",
            "}"
        }
    );
}

TEST_CASE("Struct generation") {
    const std::string code = R"(
        package main

        type S struct {
            val int
        }

        func main() int {
            var s S
            s.val = 100
            return 0
        }
    )";
    
    auto ir = getIr(code);

    check(
        ir,
        {
            "define i32 @main() {",
            "entry:",
                "%0 = alloca i32",
                "%s = alloca { i32 }",
                "%1 = getelementptr inbounds { i32 }, ptr %s, i32 0, i32 0",
                "store i32 100, ptr %1",
                "store i32 0, ptr %0",
                "br label %exit",
            "exit:",
                "%2 = load i32, ptr %0",
                "ret i32 %2",
            "}"
        }
    );
}

TEST_CASE("Function call generation") {
    const std::string code = R"(
        package main

        func main() int {
            printf("Hello world")
            return 0
        }
    )";
    
    auto ir = getIr(code);

    check(
        ir,
        {
            "define i32 @main() {",
            "entry:",
                "%0 = alloca i32",
                "%1 = call i32 (ptr, ...) @printf(ptr @0)",
                "store i32 0, ptr %0",
                "br label %exit",
            "exit:",
                "%2 = load i32, ptr %0",
                "ret i32 %2",
            "}"
        }
    );
}

TEST_CASE("If generation") {
    const std::string code = R"(
        package main

        func main() int {
            var i int
            if (i == 0) {
                i = 1
            } else {
                i = 0
            }
            return 0
        }
    )";
    
    auto ir = getIr(code);

    check(
        ir,
        {
            "define i32 @main() {",
            "entry:",
                "%0 = alloca i32",
                "%i = alloca i32",
                "%1 = load i32, ptr %i",
                "%2 = icmp eq i32 0, %1",
                "br i1 %2, label %true_cond, label %false_cond",
                
            "exit:",
                "%3 = load i32, ptr %0",
                "ret i32 %3",
            
            "true_cond:",
                "store i32 1, ptr %i",
                "br label %after_if",

            "false_cond:",
                "store i32 0, ptr %i",
                "br label %after_if",

            "after_if:",
                "store i32 0, ptr %0",
                "br label %exit",
            "}"
        }
    );
}

TEST_CASE("Loop generation") {
    const std::string code = R"(
        package main

        func main() int {
            for i := 0; i < 10; i = i + 1 {
            }
            return 0
        }
    )";
    
    auto ir = getIr(code);

    check(
        ir,
        {
            "define i32 @main() {",
            "entry:",
                "%0 = alloca i32",
                "%i = alloca i32",
                "store i32 0, ptr %i",
                "%1 = load i32, ptr %i",
                "%2 = icmp ult i32 %1",
                "br i1 %2, label %loop, label %after",

            "exit:",
                "%3 = load i32, ptr %0",
                "ret i32 %3",
            
            "loop:",
                "%4 = load i32, ptr %i",
                "%5 = add i32 %4, 1",
                "store i32 %5, ptr %i",
                "%6 = load i32, ptr %i",
                "%7 = icmp ult i32 %6, 10",
                "br i1 %7, label %loop, label %after",
            "after:",
                "store i32 0, ptr %0",
                "br label %exit",
            "}"
        }
    );
}
