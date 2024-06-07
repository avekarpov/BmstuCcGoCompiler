#pragma once

#include "value.hpp"
#include "tools.hpp"

PARENT_CLASS IrBiOperBuilder {
public:
    ValueWrapper getBiPlus(ValueWrapper lhs, ValueWrapper rhs) {
        check(lhs, rhs);

        if (lhs.tw.type_ptr->isIntegerTy(32)) {
            return ValueWrapper {
                    FROM_DERIVED._builder.CreateAdd(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        if (lhs.tw.type_ptr->isFloatTy()) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateFAdd(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        throw std::runtime_error {
            "Unsupported type for binry plus operation"
        };
    }

    ValueWrapper getBiMinus(ValueWrapper lhs, ValueWrapper rhs) {
        check(lhs, rhs);

        if (lhs.tw.type_ptr->isIntegerTy(32)) {
            return ValueWrapper {
                    FROM_DERIVED._builder.CreateSub(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        if (lhs.tw.type_ptr->isFloatTy()) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateFSub(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        throw std::runtime_error {
            "Unsupported type for binry plus operation"
        };
    }

    ValueWrapper getMul(ValueWrapper lhs, ValueWrapper rhs) {
        check(lhs, rhs);

        if (lhs.tw.type_ptr->isIntegerTy(32)) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateMul(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }
        
        if (lhs.tw.type_ptr->isFloatTy()) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateFMul(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        throw std::runtime_error {
            "Unsupported type for binry plus operation"
        };
    }

    ValueWrapper getDiv(ValueWrapper lhs, ValueWrapper rhs) {
        check(lhs, rhs);

        if (lhs.tw.type_ptr->isIntegerTy(32)) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateSDiv(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }
        
        if (lhs.tw.type_ptr->isFloatTy()) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateFDiv(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        throw std::runtime_error {
            "Unsupported type for binry plus operation"
        };
    }

    ValueWrapper getLess(ValueWrapper lhs, ValueWrapper rhs) {
        check(lhs, rhs);

        if (lhs.tw.type_ptr->isIntegerTy(32)) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateICmpULT(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }
        
        if (lhs.tw.type_ptr->isFloatTy()) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateFCmpOLT(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        throw std::runtime_error {
            "Unsupported type for binry plus operation"
        };
    }

    ValueWrapper getLessEqual(ValueWrapper lhs, ValueWrapper rhs) {
        check(lhs, rhs);

        if (lhs.tw.type_ptr->isIntegerTy(32)) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateICmpSLE(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }
        
        if (lhs.tw.type_ptr->isFloatTy()) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateFCmpOLE(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        throw std::runtime_error {
            "Unsupported type for binry plus operation"
        };
    }

    ValueWrapper getEqual(ValueWrapper lhs, ValueWrapper rhs) {
        check(lhs, rhs);

        if (lhs.tw.type_ptr->isIntegerTy(32)) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateICmpEQ(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }
        
        if (lhs.tw.type_ptr->isFloatTy()) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateFCmpOGE(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        throw std::runtime_error {
            "Unsupported type for binry plus operation"
        };
    }

    ValueWrapper getNotEqual(ValueWrapper lhs, ValueWrapper rhs) {
        check(lhs, rhs);

        if (lhs.tw.type_ptr->isIntegerTy(32)) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateICmpNE(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }
        
        if (lhs.tw.type_ptr->isFloatTy()) {
            return ValueWrapper {
                FROM_DERIVED._builder.CreateFCmpONE(
                    lhs.value_ptr,
                    rhs.value_ptr
                )
            };
        }

        throw std::runtime_error {
            "Unsupported type for binry plus operation"
        };
    }

private:
    void check(ValueWrapper lhs, ValueWrapper rhs) {
        if (lhs.tw.type_ptr->isPointerTy()) {
            throw std::runtime_error {
                "Required deref variable pointer"
            };
        }

        if (lhs.tw.type_ptr != rhs.tw.type_ptr) {
            throw std::runtime_error {
                "Types must be the same"
            };
        }
    }
};
