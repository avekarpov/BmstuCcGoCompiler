#pragma once

#include <llvm/IR/Value.h>

#include "tools.hpp"

PARENT_CLASS IrNumberBuilder {
public:
    ValueWrapper getConstInt32(int32_t number) {
        return ValueWrapper {
            llvm::ConstantInt::get(
                FROM_DERIVED._context,
                llvm::APInt(32, number)
            )
        };
    }

    ValueWrapper getConstFloat(float number) {
        return ValueWrapper {
            llvm::ConstantFP::get(
                FROM_DERIVED._context,
                llvm::APFloat(number)
            )
        };
    }

    ValueWrapper getConstBool(bool value) {
        return ValueWrapper {
            llvm::ConstantInt::get(
                FROM_DERIVED._context,
                llvm::APInt(1, value ? 1 : 0)
            )
        };
    }
};