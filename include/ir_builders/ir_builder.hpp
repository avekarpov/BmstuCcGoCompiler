#pragma once

#include <llvm/IR/IRBuilder.h>

#include "block_controller.hpp"
#include "ir_bi_oper_builder.hpp"
#include "ir_function_builder.hpp"
#include "ir_number_builder.hpp"
#include "ir_type_builder.hpp"
#include "ir_string_builder.hpp"

class IrBuilder
: public BlockController<IrBuilder>
, public IrNumberBuilder<IrBuilder>
, public IrFunctionBuilder<IrBuilder>
, public IrBiOperBuilder<IrBuilder>
, public IrTypeBuilder<IrBuilder>
, public IrStingBuilder<IrBuilder>
{
private:
    friend class BlockController<IrBuilder>;
    friend class IrBiOperBuilder<IrBuilder>;
    friend class IrNumberBuilder<IrBuilder>;
    friend class IrFunctionBuilder<IrBuilder>;
    friend class IrTypeBuilder<IrBuilder>;
    friend class IrStingBuilder<IrBuilder>;

public:
    IrBuilder(const std::string_view module_id = "Id")
    : _module { { module_id }, _context }
    , _builder { _context }
    {
        this->setupExtendFunctions();
    }

    template <class ...Args>
    void print(Args &&...args) const {
        _module.print(std::forward<Args>(args)...);
    }

private:
    llvm::LLVMContext _context;
    llvm::Module _module;
    llvm::IRBuilder<> _builder;
};

#undef FROM_DERIVED
