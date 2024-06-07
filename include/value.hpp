#pragma once

#include <llvm/IR/Value.h>
#include <variant>

using llvm::Value;
using ValuePtr = Value *;

using llvm::Type;
using TypePtr = Type *;

using llvm::Value;
using ValuePtr = Value *;

using llvm::FunctionType;
using FunctionTypePtr = FunctionType *;

using llvm::Function;
using FunctionPtr = Function *;

using llvm::StructType;
using StructTypePtr = StructType *;

using llvm::AllocaInst;
using AllocaInstPtr = AllocaInst *;

using llvm::ArrayType;
using ArrayTypePtr = ArrayType *;

struct TypeWrapper {
    TypeWrapper() = default;

    // TypeWrapper(TypeWrapper &) = delete;

    explicit TypeWrapper(TypePtr type_ptr)
    : type_ptr { type_ptr }
    {
        if (type_ptr == nullptr) {
            return;
        }

        if (type_ptr->isPointerTy()) {
            throw std::runtime_error { "Invalid type wrapper" };
        }
    }

    TypeWrapper(TypePtr type_ptr, std::shared_ptr<TypeWrapper> deref_tw_ptr)
    : type_ptr { type_ptr }
    , deref_tw_ptr { deref_tw_ptr }
    {
        if (type_ptr == nullptr) {
            throw std::runtime_error { "Invalid type wrapper" };
        }

        if (not type_ptr->isPointerTy()) {
            throw std::runtime_error { "Invalid type wrapper" };
        }
    }

    TypeWrapper(TypePtr type_ptr, TypePtr deref_tw_ptr)
    : type_ptr { type_ptr }
    , deref_tw_ptr { std::make_shared<TypeWrapper>(deref_tw_ptr) }
    {
        if (type_ptr == nullptr) {
            throw std::runtime_error { "Invalid type wrapper" };
        }

        if (not type_ptr->isPointerTy()) {
            throw std::runtime_error { "Invalid type wrapper" };
        }

        if (deref_tw_ptr == nullptr) {
            throw std::runtime_error { "Invalid type wrapper" };
        }
    }

    TypeWrapper getPointerTo() {
        return TypeWrapper {
            type_ptr->getPointerTo(),
            std::make_shared<TypeWrapper>(*this)
        };
    }

    TypePtr type_ptr = nullptr;
    std::shared_ptr<TypeWrapper> deref_tw_ptr = nullptr;
};

class ValueWrapper {
public:
    ValueWrapper() = default;

    // ValueWrapper(ValueWrapper &) = delete;

    ValueWrapper(ValuePtr value_ptr)
    : value_ptr { value_ptr }
    {
        if (value_ptr == nullptr) {
            return;
        }

        if (value_ptr->getType()->isPointerTy()) {
            throw std::runtime_error { "Invalid value wrapper" };
        }

        tw.type_ptr = value_ptr->getType();
    }

    ValueWrapper(ValuePtr value_ptr, TypeWrapper type_wrapper)
    : value_ptr { value_ptr }
    , tw { type_wrapper }
    {
        if (value_ptr == nullptr) {
            throw std::runtime_error { "Invalid value wrapper" };
        }

        if (value_ptr->getType() != tw.type_ptr) {
            throw std::runtime_error { "Invalid value wrapper" };
        }
    }

    ValuePtr value_ptr = nullptr;
    TypeWrapper tw;

    bool pass_as_pointer = false;
};
