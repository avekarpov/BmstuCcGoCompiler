#pragma once

#include <memory>

#include "antlr4/go.hpp"
#include "ir_builders/ir_builder.hpp"

class LlvmVisitor : public GoParserBaseVisitor
{
private:
    using Token = std::string;

public:
    // TODO: remove after
    std::any aggregateResult(std::any current, std::any next) override {
        if (current.has_value()) {
            return current;
        }

        return next;
    }

    std::any visitFunctionDecl(GoParser::FunctionDeclContext *ctx) override;

    std::any visitSignature(GoParser::SignatureContext *ctx) override;

    std::any visitParameters(GoParser::ParametersContext *ctx) override;

    std::any visitStatement(GoParser::StatementContext *ctx) override;

    std::any visitIfStmt(GoParser::IfStmtContext *ctx) override;

    std::any visitConstDecl(GoParser::ConstDeclContext *ctx) override;

    std::any visitVarDecl(GoParser::VarDeclContext *ctx) override;

    std::any visitExpressionList(GoParser::ExpressionListContext *ctx) override;

    std::any visitType_(GoParser::Type_Context *ctx) override;

    std::any visitExpression(GoParser::ExpressionContext *ctx) override;

    std::any visitPrimaryExpr(GoParser::PrimaryExprContext *ctx) override;

    std::any visitOperand(GoParser::OperandContext *ctx) override;

    std::any visitInteger(GoParser::IntegerContext *ctx) override;

    std::any visitBasicLit(GoParser::BasicLitContext *ctx) override;

    std::any visitLiteral(GoParser::LiteralContext *ctx) override;

    std::any visitTypeLit(GoParser::TypeLitContext *ctx) override;

    std::any visitTypeDecl(GoParser::TypeDeclContext *ctx) override;

    std::any visitSimpleStmt(GoParser::SimpleStmtContext *ctx) override;

    template <class ... Args>
    void print(Args &&...args) const {
        _ir_builder.print(std::forward<Args>(args)...);
    }

private:
    IrBuilder _ir_builder;
};
