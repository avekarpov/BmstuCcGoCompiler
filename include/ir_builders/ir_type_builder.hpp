#pragma once

#include <format>

#include "tools.hpp"
#include "value.hpp"

PARENT_CLASS IrTypeBuilder {
public:
    TypeWrapper getType(const std::string_view name) {
        if (name == "int") {
            return getInt32Type();
        }

        if (name == "float32") {
            return getFloatType();
        }

        if (name == "bool") {
            return getBoolType();
        }

        if (
            auto struct_type_ptr = StructType::getTypeByName(
                FROM_DERIVED._context,
                name
            );
            struct_type_ptr != nullptr
        ) {
            return TypeWrapper { struct_type_ptr };
        }
        
        throw std::runtime_error {
            std::format(R"(Undefined type "{}")", name)
        };
    }

    TypeWrapper getVoidType() {
        return TypeWrapper { Type::getVoidTy(FROM_DERIVED._context) };
    }

    TypeWrapper getInt32Type() {
        return TypeWrapper { Type::getInt32Ty(FROM_DERIVED._context) };
    }

    TypeWrapper getInt8Type() {
        return TypeWrapper { Type::getInt8Ty(FROM_DERIVED._context) };
    }

    TypeWrapper getFloatType() {
        return TypeWrapper { Type::getFloatTy(FROM_DERIVED._context) };
    }

    TypeWrapper getBoolType() {
        return TypeWrapper { Type::getInt1Ty(FROM_DERIVED._context) };
    }

    TypeWrapper getArrayType(TypeWrapper tw, std::size_t length) {
        return TypeWrapper { ArrayType::get(tw.type_ptr, length) };
    }

    // TODO: return TypeWrapper
    FunctionTypePtr getFunctionType(
        TypeWrapper return_tw, 
        const std::vector<TypeWrapper> &args_tws,
        bool va_arg = false
    ) {
        TypePtr return_type_ptr = return_tw.type_ptr;
        std::vector<TypePtr> args_types_ptrs;
        for (const auto tw : args_tws) {
            args_types_ptrs.emplace_back(tw.type_ptr);
        }

        return FunctionType::get(return_type_ptr, args_types_ptrs, va_arg);
    }

    StructTypePtr getStructType(
        const std::vector<TypePtr> &fields_types_ptrs,
        const std::vector<std::string> &fields_names
    ) {
        if (fields_types_ptrs.size() != fields_names.size()) {
            throw std::runtime_error { "Not all fields have name" };
        }

        auto struct_type_ptr = StructType::get(
            FROM_DERIVED._context,
            fields_types_ptrs,
            false
        );

        auto &fields = _structs_elements_poses[struct_type_ptr];
        for (std::size_t i = 0; i < fields_types_ptrs.size(); ++i) {
            if (not fields.try_emplace(fields_names[i], i).second) {
                throw std::runtime_error { "Duplicate struct field name" };
            }
        }

        return struct_type_ptr;
    }

    std::size_t getStructElementPos(TypeWrapper tw, const std::string &name) {
        assert(not tw.type_ptr->isPointerTy());
        
        if (
            auto it = _structs_elements_poses.find(tw.type_ptr);
            it != _structs_elements_poses.end()
        ) {
            auto &struct_elements_poses = it->second;

            if (auto element_pos_it = struct_elements_poses.find(name); 
                element_pos_it != struct_elements_poses.end()
            ) {
                return element_pos_it->second;
            }

            throw std::runtime_error { "Not existed field" };
        }

        throw std::runtime_error { "Not existed struct" };
    }

    ValueWrapper getStructElementPtr(ValueWrapper vw, const std::string &name) {
        assert(vw.tw.type_ptr->isPointerTy());

        const auto pos = getStructElementPos(
            *vw.tw.deref_tw_ptr,
            name
        );

        return ValueWrapper {
            FROM_DERIVED._builder.CreateInBoundsGEP(
                vw.tw.deref_tw_ptr->type_ptr,
                vw.value_ptr,
                { 
                    FROM_DERIVED.getConstInt32(0).value_ptr, 
                    FROM_DERIVED.getConstInt32(pos).value_ptr
                }
            ),
            TypeWrapper { 
                vw.tw.deref_tw_ptr->type_ptr->getStructElementType(pos)->getPointerTo(),
                vw.tw.deref_tw_ptr->type_ptr->getStructElementType(pos)
            }
        };
    }

    ValueWrapper getArrayElementPtr(ValueWrapper vw, ValueWrapper index_wv) {
        if (not vw.tw.type_ptr->isPointerTy()) {
            throw std::runtime_error { "Expected array pointer" };
        }

        ValuePtr index_ptr = nullptr;
        if (index_wv.tw.type_ptr->isPointerTy()) {
            index_ptr = FROM_DERIVED.loadPtr(index_wv).value_ptr;
        }
        else {
            index_ptr = index_wv.value_ptr;
        }

        return ValueWrapper {
            FROM_DERIVED._builder.CreateInBoundsGEP(
                vw.tw.deref_tw_ptr->type_ptr,
                vw.value_ptr,
                { 
                    FROM_DERIVED.getConstInt32(0).value_ptr, 
                    index_ptr
                }
            ),
            TypeWrapper { 
                vw.tw.deref_tw_ptr->type_ptr->getArrayElementType()->getPointerTo(),
                vw.tw.deref_tw_ptr->type_ptr->getArrayElementType()
            }
        };
    }

private:
    std::unordered_map<
        TypePtr,
        std::unordered_map<
            std::string,
            std::size_t
        >
    > _structs_elements_poses;
};
