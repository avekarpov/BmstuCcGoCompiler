#include "llvm_visitor.hpp"

// TODO:
// 1) all expressionList must have only one expression

#define ANY_AS_VALUE_PTR(__A__) std::any_cast<ValuePtr>((__A__))
#define ANY_AS_TYPE_PTR(__A__) std::any_cast<TypePtr>((__A__))
#define ANY_AS_VW(__A__) std::any_cast<ValueWrapper>((__A__))
#define ANY_AS_TW(__A__) std::any_cast<TypeWrapper>((__A__))

#define NOT_SUPPORTED_CTX(__OBJ__, __CTX__) \
if (__CTX__->__OBJ__() != nullptr) \
    throw std::runtime_error { "\"" #__OBJ__ "\" is not supported" }

#define NOT_SUPPORTED(__OBJ__) NOT_SUPPORTED_CTX(__OBJ__, ctx)

#define REQUIRED_CTX(__OBJ__, __CTX__) \
if (__CTX__->__OBJ__() == nullptr) \
    throw std::runtime_error { "\"" #__OBJ__ "\" is required" } \

#define REQUIRED(__OBJ__) REQUIRED_CTX(__OBJ__, ctx) 

#define OUT_OF_OPTIONS std::runtime_error { "Out of options" }

#define IF_HAS_CTX(__OBJ__, __CTX__) \
if (__CTX__->__OBJ__() != nullptr)

#define ELIF_HAS_CTX(__OBJ__, __CTX__) \
else if (__CTX__->__OBJ__() != nullptr)

#define IF_HAS(__OBJ__) IF_HAS_CTX(__OBJ__, ctx)
#define ELIF_HAS(__OBJ__) ELIF_HAS_CTX(__OBJ__, ctx)

#define ACCEPT_CTX(__OBJ__, __CTX__) __CTX__->__OBJ__()->accept(this)
#define ACCEPT(__OBJ__) ACCEPT_CTX(__OBJ__, ctx)

/*
functionDecl
    : FUNC IDENTIFIER typeParameters? signature block?
    ;
*/
std::any LlvmVisitor::visitFunctionDecl(GoParser::FunctionDeclContext *ctx) {
    REQUIRED(IDENTIFIER);

    auto name = ctx->IDENTIFIER()->getText();
    
    NOT_SUPPORTED(typeParameters);

    REQUIRED(signature);
    auto [function_type_ptr, return_tw, args_tws, args_names] = std::any_cast<
        std::tuple<
            FunctionTypePtr,
            TypeWrapper, // Return type
            std::vector<TypeWrapper>, // Args types
            std::vector<std::string>
        >
    >(ACCEPT(signature));

    auto function_ptr = _ir_builder.getFunctionDecl(
        function_type_ptr,
        name
    );

    for (std::size_t i = 0; auto &arg : function_ptr->args()) {
        arg.setName(args_names[i++]);
    }

    IF_HAS(block) {
        _ir_builder.initFunction(function_ptr);

        for (std::size_t i = 0; auto &arg : function_ptr->args()) {
            _ir_builder.addVar(args_tws[i], args_names[i]);
            _ir_builder.assignVar(args_names[i], static_cast<ValuePtr>(&arg));
            ++i;
        }

        ACCEPT(block);

        _ir_builder.finalizeFunction();
    }
    

    return {};
}

/*
signature
    : parameters result?
    ;

result
    : parameters
    | type_
    ;
*/
std::any LlvmVisitor::visitSignature(GoParser::SignatureContext *ctx) {
    TypeWrapper result_tw;
    IF_HAS(result) {
        auto result_ctx = ctx->result();

        NOT_SUPPORTED_CTX(parameters, result_ctx);
        REQUIRED_CTX(type_, result_ctx);

        result_tw = ANY_AS_TW(ACCEPT_CTX(type_, result_ctx));
    }
    else {
        result_tw = _ir_builder.getVoidType();
    }

    REQUIRED(parameters); 
    auto [args_tws, args_names] = std::any_cast<
        std::pair<
            std::vector<TypeWrapper>,
            std::vector<std::string>
        >
    >(ACCEPT(parameters));

    return std::make_tuple(
        // TODO: make as TypeWrapper
        _ir_builder.getFunctionType(
            result_tw,
            args_tws
        ),
        std::move(result_tw),
        std::move(args_tws),
        std::move(args_names)
    );

    throw OUT_OF_OPTIONS;
}

/*
parameters
    : L_PAREN (parameterDecl (COMMA parameterDecl)* COMMA?)? R_PAREN
    ;
*/
std::any LlvmVisitor::visitParameters(GoParser::ParametersContext *ctx) {
    std::vector<TypeWrapper> types;
    std::vector<std::string> names;

    for (auto param_decl_ctx : ctx->parameterDecl()) {
        REQUIRED_CTX(type_, param_decl_ctx);

        types.emplace_back(ANY_AS_TW(ACCEPT_CTX(type_, param_decl_ctx)));

        REQUIRED_CTX(identifierList, param_decl_ctx);

        auto ident_list_ctx = param_decl_ctx->identifierList();

        if (ident_list_ctx->IDENTIFIER().empty()) {
            throw std::runtime_error { "Empty declaration is not supported" };
        }
        if (ident_list_ctx->IDENTIFIER().size() != 1) {
            throw std::runtime_error { "Multi declaration is not supported" };
        }

        names.emplace_back(ident_list_ctx->IDENTIFIER().front()->getText());
    }

    return std::make_pair(std::move(types), std::move(names));
}

/*
statement
    : declaration
    | labeledStmt
    | simpleStmt
    | goStmt
    | returnStmt
    | breakStmt
    | continueStmt
    | gotoStmt
    | fallthroughStmt
    | block
    | ifStmt
    | switchStmt
    | selectStmt
    | forStmt
    | deferStmt
    ;
*/
std::any LlvmVisitor::visitStatement(GoParser::StatementContext *ctx) {
    NOT_SUPPORTED(labeledStmt);
    NOT_SUPPORTED(goStmt);
    NOT_SUPPORTED(breakStmt);
    NOT_SUPPORTED(continueStmt);
    NOT_SUPPORTED(gotoStmt);
    NOT_SUPPORTED(fallthroughStmt);
    NOT_SUPPORTED(switchStmt);
    NOT_SUPPORTED(selectStmt);
    NOT_SUPPORTED(deferStmt);

    IF_HAS(declaration) {
        return ACCEPT(declaration);
    }
    ELIF_HAS(returnStmt) {
        auto return_ctx = ctx->returnStmt();

        IF_HAS_CTX(expressionList, return_ctx) {
            auto return_vw = ANY_AS_VW(return_ctx->expressionList()->expression().front()->accept(this));

            if (return_vw.tw.type_ptr->isPointerTy()) {
                _ir_builder.addFunctionReturn(_ir_builder.loadPtr(return_vw));
            }
            else {
                _ir_builder.addFunctionReturn(return_vw);
            }
        }
        else {
            _ir_builder.addFunctionReturn(ValueWrapper { nullptr });
        }

        return {};
    }
    ELIF_HAS(block) {
        ACCEPT(block);

        return {};
    }
    ELIF_HAS(simpleStmt) {
        return ACCEPT(simpleStmt);
    }
    ELIF_HAS(forStmt) {
        auto for_ctx = ctx->forStmt();

        NOT_SUPPORTED_CTX(expression, for_ctx);
        NOT_SUPPORTED_CTX(rangeClause, for_ctx);
        REQUIRED_CTX(forClause, for_ctx);

        auto for_clause_ctx = for_ctx->forClause();

        _ir_builder.prepareLoop();

        if (for_clause_ctx->initStmt != nullptr) {
            for_clause_ctx->initStmt->accept(this);
        }

        REQUIRED_CTX(expression, for_clause_ctx);
        auto expression_vw = ANY_AS_VW(ACCEPT_CTX(expression, for_clause_ctx));
        _ir_builder.startLoop(expression_vw.value_ptr);

        ACCEPT_CTX(block, for_ctx);

        if (for_clause_ctx->postStmt != nullptr) {
            for_clause_ctx->postStmt->accept(this);
        }

        expression_vw = ANY_AS_VW(ACCEPT_CTX(expression, for_clause_ctx));
        _ir_builder.finalizeLoop(expression_vw.value_ptr);

        return {};
    }
    ELIF_HAS(ifStmt) {
        ACCEPT(ifStmt);

        return {};
    }

    throw OUT_OF_OPTIONS;
}

std::any LlvmVisitor::visitIfStmt(GoParser::IfStmtContext *ctx) {
    NOT_SUPPORTED(simpleStmt);

    REQUIRED(expression);
    auto expression_vw = ANY_AS_VW(ACCEPT(expression));

    if (expression_vw.tw.type_ptr->isPointerTy()) {
        expression_vw = _ir_builder.loadPtr(expression_vw);
    }

    _ir_builder.initIf(expression_vw.value_ptr);

    ctx->block().front()->accept(this);

    _ir_builder.finalizeTrueIf();

    IF_HAS(ifStmt) {
        ACCEPT(ifStmt);
    }
    ELIF_HAS(ELSE) {
        ctx->block().back()->accept(this);
    }

    _ir_builder.finalizeIf();

    return {};
}

/*
constDecl
    : CONST (constSpec | L_PAREN (constSpec eos)* R_PAREN)
    ;

constSpec
    : identifierList (type_? ASSIGN expressionList)?
    ;
*/
std::any LlvmVisitor::visitConstDecl(GoParser::ConstDeclContext *ctx) {
    if (ctx->constSpec().empty()) {
        throw std::runtime_error { "Empty declaration is not supported" };
    }
    if (ctx->constSpec().size() != 1) {
        throw std::runtime_error { "Multi declaration is not supported" };
    }

    auto const_spec_ctx = ctx->constSpec().front();

    REQUIRED_CTX(identifierList, const_spec_ctx);

    auto ident_list_ctx = const_spec_ctx->identifierList();

    if (ident_list_ctx->IDENTIFIER().empty()) {
        throw std::runtime_error { "Empty declaration is not supported" };
    }
    if (ident_list_ctx->IDENTIFIER().size() != 1) {
        throw std::runtime_error { "Multi declaration is not supported" };
    }

    auto name = ident_list_ctx->IDENTIFIER().front()->getText();

    REQUIRED_CTX(type_, const_spec_ctx);

    // auto tw = ANY_AS_TW(ACCEPT_CTX(type_, const_spec_ctx));

    REQUIRED_CTX(expressionList, const_spec_ctx);

    auto expression_list_ctx = const_spec_ctx->expressionList();

    if (expression_list_ctx->expression().empty()) {
        throw std::runtime_error { "Empty expression is not supported" };
    }
    if (expression_list_ctx->expression().size() != 1) {
        throw std::runtime_error { "Multi expression is not supported" };
    }

    auto expression_vw = ANY_AS_VW(expression_list_ctx->expression().front()->accept(this));

    _ir_builder.addNamedValue(
        std::move(name),
        expression_vw.value_ptr
    );

    return {};
}

/*
varDecl
    : VAR (varSpec | L_PAREN (varSpec eos)* R_PAREN)
    ;

varSpec
    : identifierList (type_ (ASSIGN expressionList)? | ASSIGN expressionList)
    ;
*/
std::any LlvmVisitor::visitVarDecl(GoParser::VarDeclContext *ctx) {
    if (ctx->varSpec().empty()) {
        throw std::runtime_error { "Empty declaration is not supported" };
    }
    if (ctx->varSpec().size() != 1) {
        throw std::runtime_error { "Multi declaration is not supported" };
    }

    auto var_spec_ctx = ctx->varSpec().front();

    REQUIRED_CTX(identifierList, var_spec_ctx);

    auto ident_list_ctx = var_spec_ctx->identifierList();

    if (ident_list_ctx->IDENTIFIER().empty()) {
        throw std::runtime_error { "Empty declaration is not supported" };
    }
    if (ident_list_ctx->IDENTIFIER().size() != 1) {
        throw std::runtime_error { "Multi declaration is not supported" };
    }

    auto name = ident_list_ctx->IDENTIFIER().front()->getText();

    REQUIRED_CTX(type_, var_spec_ctx);

    auto tw = ANY_AS_TW(ACCEPT_CTX(type_, var_spec_ctx));

    _ir_builder.addVar(tw, name);

    IF_HAS_CTX(expressionList, var_spec_ctx) {
        _ir_builder.assignVar(
            name, 
            ANY_AS_VW(var_spec_ctx->expressionList()->expression().front()->accept(this)).value_ptr
        );
    }

    return {};
}

/*
expressionList
    : expression (COMMA expression)*
    ;
*/
// TODO: remove this method
std::any LlvmVisitor::visitExpressionList(GoParser::ExpressionListContext *ctx) {
    throw std::runtime_error { "not supported" };
}

/*
type_
    : typeName typeArgs?
    | typeLit
    | L_PAREN type_ R_PAREN
    ;
*/
std::any LlvmVisitor::visitType_(GoParser::Type_Context *ctx) {
    NOT_SUPPORTED(type_);

    IF_HAS(typeName) {
        NOT_SUPPORTED(typeArgs);
        REQUIRED(typeName);
        auto type_name_ctx = ctx->typeName();

        NOT_SUPPORTED_CTX(qualifiedIdent, type_name_ctx);
        REQUIRED_CTX(IDENTIFIER, type_name_ctx);
        return TypeWrapper { _ir_builder.getType(type_name_ctx->IDENTIFIER()->getText()) };
    }
    ELIF_HAS(typeLit) {
        return ACCEPT(typeLit);
    }

    throw OUT_OF_OPTIONS;
}

/*
expression
    : primaryExpr
    | unary_op = (PLUS | MINUS | EXCLAMATION | CARET | STAR | AMPERSAND | RECEIVE) expression
    | expression mul_op = (STAR | DIV | MOD | LSHIFT | RSHIFT | AMPERSAND | BIT_CLEAR) expression
    | expression add_op = (PLUS | MINUS | OR | CARET) expression
    | expression rel_op = (
        EQUALS
        | NOT_EQUALS
        | LESS
        | LESS_OR_EQUALS
        | GREATER
        | GREATER_OR_EQUALS
    ) expression
    | expression LOGICAL_AND expression
    | expression LOGICAL_OR expression
    ;
*/
std::any LlvmVisitor::visitExpression(GoParser::ExpressionContext *ctx) {
    IF_HAS(primaryExpr) {
        return ACCEPT(primaryExpr);
    }

    if (ctx->expression().size() == 1) {
        NOT_SUPPORTED(PLUS);
        NOT_SUPPORTED(EXCLAMATION);
        NOT_SUPPORTED(CARET);
        NOT_SUPPORTED(RECEIVE);

        auto vw = ANY_AS_VW(ctx->expression().front()->accept(this));
        
        IF_HAS(AMPERSAND) {
            vw.pass_as_pointer = true;
            return vw;
        }
        ELIF_HAS(STAR) {
           return _ir_builder.loadPtr(vw);
        }
        ELIF_HAS(MINUS) {
            if (vw.tw.type_ptr->isPointerTy()) {
                vw = _ir_builder.loadPtr(vw);
            }
            
            return _ir_builder.getMul(vw, _ir_builder.getConstInt32(-1));
        }
    }

    if (ctx->expression().size() == 2) {
        NOT_SUPPORTED(MOD);
        NOT_SUPPORTED(CARET);
        NOT_SUPPORTED(OR);
        NOT_SUPPORTED(LSHIFT);
        NOT_SUPPORTED(RSHIFT);
        NOT_SUPPORTED(AMPERSAND);
        NOT_SUPPORTED(BIT_CLEAR);
        NOT_SUPPORTED(LOGICAL_AND);
        NOT_SUPPORTED(LOGICAL_OR);

        auto lhs = ANY_AS_VW(ctx->expression().front()->accept(this));
        if (lhs.tw.type_ptr->isPointerTy()) {
            lhs = _ir_builder.loadPtr(lhs);
        }
        auto rhs = ANY_AS_VW(ctx->expression().back()->accept(this));
        if (rhs.tw.type_ptr->isPointerTy()) {
            rhs = _ir_builder.loadPtr(rhs);
        }

        IF_HAS(PLUS) {
            return _ir_builder.getBiPlus(lhs, rhs);
        }
        ELIF_HAS(MINUS) {
            return _ir_builder.getBiMinus(lhs, rhs);
        }
        ELIF_HAS(STAR) {
            return _ir_builder.getMul(lhs, rhs);
        }
        ELIF_HAS(DIV) {
            return _ir_builder.getMul(lhs, rhs);
        }
        ELIF_HAS(LESS) {
            return _ir_builder.getLess(lhs, rhs);
        }
        ELIF_HAS(LESS_OR_EQUALS) {
            return _ir_builder.getLessEqual(lhs, rhs);
        }
        ELIF_HAS(GREATER) {
            // swapped
            return _ir_builder.getLess(rhs, lhs);
        }
        ELIF_HAS(GREATER_OR_EQUALS) {
            // swapped
            return _ir_builder.getLessEqual(rhs, lhs);
        }
        ELIF_HAS(EQUALS) {
            return _ir_builder.getEqual(rhs, lhs);
        }
        ELIF_HAS(NOT_EQUALS) {
            return _ir_builder.getNotEqual(rhs, lhs);
        }
    }

    throw OUT_OF_OPTIONS;
}

/*
primaryExpr
    : operand
    | conversion
    | methodExpr
    | primaryExpr ( DOT IDENTIFIER | index | slice_ | typeAssertion | arguments)
    ;
*/
std::any LlvmVisitor::visitPrimaryExpr(GoParser::PrimaryExprContext *ctx) {
    NOT_SUPPORTED(conversion);
    NOT_SUPPORTED(methodExpr);
    
    IF_HAS(operand) {
        return ACCEPT(operand);
    }
    ELIF_HAS(primaryExpr) {
        NOT_SUPPORTED(slice_);
        NOT_SUPPORTED(typeAssertion);

        IF_HAS(arguments) {
            auto primary_ctx = ctx->primaryExpr();

            REQUIRED_CTX(operand, primary_ctx);

            auto operand_ctx = primary_ctx->operand();

            REQUIRED_CTX(operandName, operand_ctx);

            auto function_name = operand_ctx->operandName()->IDENTIFIER()->getText();

            auto arguments_ctx = ctx->arguments();

            std::vector<ValueWrapper> args_vws;
            IF_HAS_CTX(expressionList, arguments_ctx) {
                for (auto expression_ctx : arguments_ctx->expressionList()->expression()) {
                    auto vw = ANY_AS_VW(expression_ctx->accept(this));
                    if (vw.tw.type_ptr->isPointerTy() && not vw.pass_as_pointer) {
                        vw = _ir_builder.loadPtr(vw);
                    }
                    args_vws.emplace_back(vw);
                }
            }
            
            return _ir_builder.callFunction(function_name, args_vws);
        }

        auto primary_vw = ANY_AS_VW(ACCEPT(primaryExpr));

        IF_HAS(index) {
            auto index_vw = ANY_AS_VW(ACCEPT(index));

            return _ir_builder.getArrayElementPtr(
                primary_vw,
                index_vw
            );
        }
        ELIF_HAS(IDENTIFIER) {
            auto name = ctx->IDENTIFIER()->getText();

            return _ir_builder.getStructElementPtr(
                primary_vw,
                name
            );
        }
    }

    throw OUT_OF_OPTIONS;
}

/*
operand
    : literal
    | operandName typeArgs?
    | L_PAREN expression R_PAREN
    ;
*/
std::any LlvmVisitor::visitOperand(GoParser::OperandContext *ctx) {
    IF_HAS(operandName) {
        auto operand_name_ctx = ctx->operandName();

        REQUIRED_CTX(IDENTIFIER, operand_name_ctx);
        return _ir_builder.find(operand_name_ctx->IDENTIFIER()->getText());
    }
    ELIF_HAS(expression) {
        return ACCEPT(expression);
    }
    ELIF_HAS(literal) {
        return ACCEPT(literal);
    }

    throw OUT_OF_OPTIONS;
}

/*
integer
    : DECIMAL_LIT
    | BINARY_LIT
    | OCTAL_LIT
    | HEX_LIT
    | IMAGINARY_LIT
    | RUNE_LIT
    ;
*/
std::any LlvmVisitor::visitInteger(GoParser::IntegerContext *ctx) {
    NOT_SUPPORTED(BINARY_LIT);
    NOT_SUPPORTED(OCTAL_LIT);
    NOT_SUPPORTED(HEX_LIT);
    NOT_SUPPORTED(IMAGINARY_LIT);
    NOT_SUPPORTED(RUNE_LIT);

    IF_HAS(DECIMAL_LIT) {
        return _ir_builder.getConstInt32(
            std::stoi(ctx->DECIMAL_LIT()->getText())
        );
    }

    throw OUT_OF_OPTIONS;
}

/*
basicLit
    : NIL_LIT
    | integer
    | string_
    | FLOAT_LIT
    ;
*/
std::any LlvmVisitor::visitBasicLit(GoParser::BasicLitContext *ctx) {
    NOT_SUPPORTED(NIL_LIT);

    IF_HAS(FLOAT_LIT) {
        return _ir_builder.getConstFloat(
            std::stof(ctx->FLOAT_LIT()->getText())
        );
    }
    ELIF_HAS(integer) {
        return ACCEPT(integer);
    }
    IF_HAS(string_) {
        REQUIRED_CTX(INTERPRETED_STRING_LIT, ctx->string_());

        auto quoted_string = ctx->string_()->INTERPRETED_STRING_LIT()->getText();
        quoted_string.erase(quoted_string.begin());
        quoted_string.pop_back();

        auto vw = _ir_builder.getConstGlobalStringPtr(quoted_string);
        vw.pass_as_pointer = true;
        return vw;
    }

    throw OUT_OF_OPTIONS;
}

/*
literal
    : basicLit
    | compositeLit
    | functionLit
    ;
*/
std::any LlvmVisitor::visitLiteral(GoParser::LiteralContext *ctx) {
    NOT_SUPPORTED(compositeLit);
    NOT_SUPPORTED(functionLit);

    IF_HAS(basicLit) {
        return ACCEPT(basicLit);
    }

    throw OUT_OF_OPTIONS;
}

/*
typeLit
    : arrayType
    | structType
    | pointerType
    | functionType
    | interfaceType
    | sliceType
    | mapType
    | channelType
    ;
*/
std::any LlvmVisitor::visitTypeLit(GoParser::TypeLitContext *ctx) {
    NOT_SUPPORTED(functionType);
    NOT_SUPPORTED(interfaceType);
    NOT_SUPPORTED(sliceType);
    NOT_SUPPORTED(mapType);
    NOT_SUPPORTED(channelType);

    IF_HAS(arrayType) {
        auto array_ctx = ctx->arrayType();

        REQUIRED_CTX(elementType, array_ctx);
        REQUIRED_CTX(type_, array_ctx->elementType());
        REQUIRED_CTX(arrayLength, array_ctx);
        auto tw = ANY_AS_TW(ACCEPT_CTX(type_, array_ctx->elementType()));
        // auto length = std::stoll(array_ctx->arrayLength()->getText())
        auto length_vw = ANY_AS_VW(ACCEPT_CTX(expression, array_ctx->arrayLength()));
        
        assert(not length_vw.tw.type_ptr->isPointerTy());
        auto length = llvm::dyn_cast<llvm::ConstantInt>(length_vw.value_ptr)->getSExtValue();

        return TypeWrapper { _ir_builder.getArrayType(tw, length) };
    }
    ELIF_HAS(pointerType) {
        auto pointer_ctx = ctx->pointerType();

        REQUIRED_CTX(type_, pointer_ctx);
        auto tw = ANY_AS_TW(ACCEPT_CTX(type_, pointer_ctx));
        
        return TypeWrapper { tw.type_ptr->getPointerTo(), std::make_shared<TypeWrapper>(tw) };
    }
    ELIF_HAS(structType) {
        auto struct_ctx = ctx->structType();
        std::vector<TypePtr> fields_types_ptrs;
        std::vector<std::string> fields_names;

        for (auto field_ctx : struct_ctx->fieldDecl()) {
            NOT_SUPPORTED_CTX(embeddedField, field_ctx);
            REQUIRED_CTX(identifierList, field_ctx);

            auto ident_list_ctx = field_ctx->identifierList();

            if (ident_list_ctx->IDENTIFIER().empty()) {
                throw std::runtime_error { "Empty declaration is not supported" };
            }
            if (ident_list_ctx->IDENTIFIER().size() != 1) {
                throw std::runtime_error { "Multi declaration is not supported" };
            }

            fields_names.emplace_back(ident_list_ctx->IDENTIFIER().front()->getText());

            REQUIRED_CTX(type_, field_ctx);

            fields_types_ptrs.emplace_back(ANY_AS_TW(ACCEPT_CTX(type_, field_ctx)).type_ptr);
        }

        // TODO: return as type wrapper
        return _ir_builder.getStructType(fields_types_ptrs, fields_names);
    }

    throw OUT_OF_OPTIONS;
}

/*
typeDecl
    : TYPE (typeSpec | L_PAREN (typeSpec eos)* R_PAREN)
    ;
*/
std::any LlvmVisitor::visitTypeDecl(GoParser::TypeDeclContext *ctx) {
    if (ctx->typeSpec().empty()) {
        throw std::runtime_error { "Empty declaration is not supported" };
    }
    if (ctx->typeSpec().size() != 1) {
        throw std::runtime_error { "Multi declaration is not supported" };
    }

    auto type_spec_ctx = ctx->typeSpec().front();

    NOT_SUPPORTED_CTX(aliasDecl, type_spec_ctx);
    REQUIRED_CTX(typeDef, type_spec_ctx);

    auto type_def_ctx = type_spec_ctx->typeDef();

    NOT_SUPPORTED_CTX(typeParameters, type_def_ctx);
    REQUIRED_CTX(IDENTIFIER, type_def_ctx);
    REQUIRED_CTX(type_, type_def_ctx);

    auto name = type_def_ctx->IDENTIFIER()->getText();

    auto struct_type_ptr = std::any_cast<StructTypePtr>(ACCEPT_CTX(type_, type_def_ctx));
    struct_type_ptr->setName(name);

    return static_cast<TypePtr>(struct_type_ptr);
}

/*
simpleStmt
    : sendStmt
    | incDecStmt
    | assignment
    | expressionStmt
    | shortVarDecl
    ;
*/
std::any LlvmVisitor::visitSimpleStmt(GoParser::SimpleStmtContext *ctx) {
    NOT_SUPPORTED(sendStmt);
    NOT_SUPPORTED(incDecStmt);

    IF_HAS(assignment) {
        auto assigment_ctx = ctx->assignment();

        if (assigment_ctx->expressionList().size() != 2) {
            throw std::runtime_error { "Invalid assignment" };
        }

        auto lhs_vw = ANY_AS_VW(assigment_ctx->expressionList().front()->expression().front()->accept(this));
        auto rhs_vw = ANY_AS_VW(assigment_ctx->expressionList().back()->expression().front()->accept(this));

        if (rhs_vw.tw.type_ptr->isPointerTy()) {
            rhs_vw = _ir_builder.loadPtr(rhs_vw);
        }

        _ir_builder.assign(lhs_vw.value_ptr, rhs_vw.value_ptr);

        return {};
    }
    ELIF_HAS(shortVarDecl) {
        auto short_var_decl_ctx = ctx->shortVarDecl();

        REQUIRED_CTX(identifierList, short_var_decl_ctx);

        auto ident_list_ctx = short_var_decl_ctx->identifierList();

        if (ident_list_ctx->IDENTIFIER().empty()) {
            throw std::runtime_error { "Empty declaration is not supported" };
        }
        if (ident_list_ctx->IDENTIFIER().size() != 1) {
            throw std::runtime_error { "Multi declaration is not supported" };
        }

        auto name = ident_list_ctx->IDENTIFIER().front()->getText();

        auto init_vw = ANY_AS_VW(short_var_decl_ctx->expressionList()->expression().front()->accept(this));

        if (init_vw.tw.type_ptr->isPointerTy()) {
            init_vw = _ir_builder.loadPtr(init_vw);
        }

        _ir_builder.addVar(init_vw.tw, name);
        _ir_builder.assignVar(name, init_vw.value_ptr);

        return {};
    }
    ELIF_HAS(expressionStmt) {
        ACCEPT(expressionStmt);

        return {};
    }

    throw OUT_OF_OPTIONS;
}
