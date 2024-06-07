#pragma once

#include "tools.hpp"
#include "value.hpp"

PARENT_CLASS IrFunctionBuilder {
public:
    FunctionPtr getFunctionDecl(FunctionTypePtr function_type_ptr, const std::string_view name) {
        auto function_ptr = Function::Create(
            function_type_ptr,
            Function::ExternalLinkage,
            name,
            FROM_DERIVED._module
        );

        if (function_ptr->getName().data() != name) {
            function_ptr->eraseFromParent();
            
            function_ptr = FROM_DERIVED._module.getFunction(name);
            
            if (not function_ptr->empty()) {
                throw std::runtime_error { "Redefinition of function" };
            }

            if (function_ptr->arg_size() != function_type_ptr->params().size()) {
                throw std::runtime_error { "Invalid args count" };
            }
        }

        return function_ptr;
    }
};
