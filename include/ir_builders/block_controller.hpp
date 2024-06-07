#pragma once

#include <llvm/IR/Verifier.h>
#include <format>

#include "value.hpp"
#include "tools.hpp"

PARENT_CLASS BlockController {
private:
    // TODO: store ValueWrapper
    using ValueMap = std::unordered_map<std::string, ValuePtr>;
    using VarMap = std::unordered_map<std::string, ValueWrapper>;

    using BB = llvm::BasicBlock;
    using BBPtr = BB *;

    struct Block {
        ValueMap map;
        VarMap vars;

        BBPtr bb_ptr = nullptr;
        BBPtr future_bb_ptr = nullptr;

        std::unordered_set<BBPtr> bb_has_return;
    };

public:
    void setupExtendFunctions() {
        FROM_DERIVED._module.getOrInsertFunction(
            "printf",
            FROM_DERIVED.getFunctionType(
                TypeWrapper { FROM_DERIVED.getInt32Type() },
                {
                    FROM_DERIVED.getInt8Type().getPointerTo()
                },
                true
            )
        );
        FROM_DERIVED._module.getOrInsertFunction(
            "scanf",
            FROM_DERIVED.getFunctionType(
                TypeWrapper { FROM_DERIVED.getInt32Type() },
                {
                    FROM_DERIVED.getInt8Type().getPointerTo()   
                },
                true
            )
        );
    }

    void addNamedValue(std::string name, ValuePtr value_ptr) {
        if (isInFunction()) {
            auto &block = _blocks.back();

            if (not block.map.try_emplace(std::move(name), value_ptr).second) {
                throw std::runtime_error { "Name already exists" };
            }
        }
        else {
            throw std::runtime_error { "Value out of function" };
        }
    }

    ValueWrapper find(const std::string &name) {
        if (name == "true") {
            return FROM_DERIVED.getConstBool(true).value_ptr;
        }
        if (name == "false") {
            return FROM_DERIVED.getConstBool(false).value_ptr;
        }

        for (auto block_it = _blocks.rbegin(); block_it != _blocks.rend(); ++block_it) {
            auto &block = *block_it;

            if (auto it = block.map.find(name); it != block.map.end()) {
                return ValueWrapper { it->second };
            }

            if (auto it = block.vars.find(name); it != block.vars.end()) {
                return it->second;
            }
        }

        throw std::runtime_error {
            std::format(R"(Name "{}" not existed)", name)
        };
    }

    void initFunction(llvm::Function *function_ptr) {
        if (function_ptr == nullptr) {
            throw std::runtime_error { "Null function" };
        }

        if (not function_ptr->empty()) {
            throw std::runtime_error { "Redefinition of function" };
        }

        Block block;
        block.bb_ptr = llvm::BasicBlock::Create(
            FROM_DERIVED._context,
            "entry",
            function_ptr
        );

        FROM_DERIVED._builder.SetInsertPoint(
            block.bb_ptr
        );

        if (not function_ptr->getReturnType()->isVoidTy()) {
            _return_alloca_ptr = FROM_DERIVED._builder.CreateAlloca(function_ptr->getReturnType(), nullptr, "");
            block.future_bb_ptr = llvm::BasicBlock::Create(
                FROM_DERIVED._context,
                "exit",
                function_ptr
            );
        }
        else {
            _return_alloca_ptr = nullptr;
        }

        _blocks.emplace_back(block);
    }

    void prepareLoop() {
        Block block;

        _blocks.emplace_back(block);
    }

    void startLoop(ValuePtr value_ptr) {
        _blocks.back().bb_ptr = llvm::BasicBlock::Create(
            FROM_DERIVED._context,
            "loop",
            function()
        );

        _blocks.back().future_bb_ptr = llvm::BasicBlock::Create(
            FROM_DERIVED._context,
            "after",
            function()
        );

        FROM_DERIVED._builder.CreateCondBr(
            value_ptr, 
            _blocks.back().bb_ptr,
            _blocks.back().future_bb_ptr
        );
        FROM_DERIVED._builder.SetInsertPoint(_blocks.back().bb_ptr);
    }

    void finalizeLoop(ValuePtr value_ptr) {
        FROM_DERIVED._builder.CreateCondBr(
            value_ptr,
            _blocks.back().bb_ptr,
            _blocks.back().future_bb_ptr
        );
        FROM_DERIVED._builder.SetInsertPoint(_blocks.back().future_bb_ptr);

        _blocks.pop_back();
    }

    void initIf(ValuePtr cond_value_ptr) {
        Block block;

        block.bb_ptr = llvm::BasicBlock::Create(
            FROM_DERIVED._context,
            "true_cond",
            function()
        );

        block.future_bb_ptr = llvm::BasicBlock::Create(
            FROM_DERIVED._context,
            "false_cond",
            function()
        );

        FROM_DERIVED._builder.CreateCondBr(
            cond_value_ptr,
            block.bb_ptr,
            block.future_bb_ptr
        );
        
        // active block true_cond
        FROM_DERIVED._builder.SetInsertPoint(block.bb_ptr);

        _blocks.emplace_back(block);
    }

    void finalizeTrueIf() {
        // active block false_cond
        FROM_DERIVED._builder.SetInsertPoint(_blocks.back().future_bb_ptr);
    }

    void finalizeIf() {
        auto after_bb_ptr = llvm::BasicBlock::Create(
            FROM_DERIVED._context,
            "after_if",
            function()
        );
        
        if (not _blocks.back().bb_has_return.count(FROM_DERIVED._builder.GetInsertBlock())) {
            // exit from false_cond to after
            FROM_DERIVED._builder.CreateBr(
                after_bb_ptr
            );
        }

        FROM_DERIVED._builder.SetInsertPoint(_blocks.back().bb_ptr);

        if (not _blocks.back().bb_has_return.count(FROM_DERIVED._builder.GetInsertBlock())) {
            // exit from true_cond to after
            FROM_DERIVED._builder.CreateBr(
                after_bb_ptr
            );
        }

        // active block after
        FROM_DERIVED._builder.SetInsertPoint(after_bb_ptr);

        _blocks.pop_back();
    }

    void addFunctionReturn(ValueWrapper vw) {
        if (not isInFunction()) {
            throw std::runtime_error { "Return out of function" };
        }

        _blocks.back().bb_has_return.insert(
            FROM_DERIVED._builder.GetInsertBlock()
        );

        FROM_DERIVED._builder.CreateStore(vw.value_ptr, _return_alloca_ptr);
        FROM_DERIVED._builder.CreateBr(
            _blocks.front().future_bb_ptr
        );
    }

    void finalizeFunction() {
        assert(_blocks.size() == 1);

        if (_return_alloca_ptr != nullptr) {
            FROM_DERIVED._builder.SetInsertPoint(
                _blocks.back().future_bb_ptr
            );
            FROM_DERIVED._builder.CreateRet(
                    FROM_DERIVED._builder.CreateLoad(
                    _return_alloca_ptr->getAllocatedType(),
                    _return_alloca_ptr
                )
            );    
        }
        else {
            FROM_DERIVED._builder.CreateRet(nullptr);   
        }

        _blocks.clear();

        // llvm::verifyFunction(function());
    }

    ValueWrapper callFunction(const std::string &name, const std::vector<ValueWrapper> &args_vws) {
        std::vector<ValuePtr> args_ptrs;
        args_ptrs.reserve(args_vws.size());
        for (const auto vw : args_vws) {
            args_ptrs.emplace_back(vw.value_ptr);
        }

        auto function_ptr = FROM_DERIVED._module.getFunction(name);

        if (function_ptr == nullptr) {
            throw std::runtime_error {
                "Non existent function"
            };
        }

        return ValueWrapper {
            FROM_DERIVED._builder.CreateCall(
                function_ptr,
                args_ptrs
            )
        };
    }

    void addVar(TypeWrapper tw, std::string name) {
        if (isInFunction()) {
            auto &block = _blocks.back();

            if (
                ValuePtr value_ptr = FROM_DERIVED._builder.CreateAlloca(tw.type_ptr, nullptr, name);
                not block.vars.try_emplace(
                    std::move(name), 
                    ValueWrapper { 
                        value_ptr,
                        TypeWrapper {
                            value_ptr->getType(),
                            std::make_shared<TypeWrapper>(tw)
                        }
                    }
                ).second
            ) {
                throw std::runtime_error { "Var already exists" };
            }
        }
        else {
            throw std::runtime_error { "Variable out of function" };
        }
    }

    void assignVar(const std::string &name, ValuePtr value_ptr) {
        auto &block = _blocks.back();

        if (isInFunction()) {
            if (auto it = block.vars.find(name); it != block.vars.end()) {
                assert(
                    it->second.tw.type_ptr->isPointerTy() &&
                    it->second.tw.deref_tw_ptr->type_ptr == value_ptr->getType()
                );
                FROM_DERIVED._builder.CreateStore(value_ptr, it->second.value_ptr);
            }
            else {
                throw std::runtime_error { "Not existed name" };
            }
        }
        else {
            throw std::runtime_error { "Global variable is not supported" };
        }
    }

    void assign(ValuePtr to_ptr, ValuePtr from_ptr) {
        assert(not from_ptr->getType()->isPointerTy());

        FROM_DERIVED._builder.CreateStore(from_ptr, to_ptr);
    }

    ValueWrapper loadPtr(ValueWrapper vw) {
        return ValueWrapper {
            FROM_DERIVED._builder.CreateLoad(
                vw.tw.deref_tw_ptr->type_ptr, 
                vw.value_ptr
            ),
            *vw.tw.deref_tw_ptr
        };
    }

    bool isInFunction() const {
        return not _blocks.empty();
    }

    FunctionPtr function() {
        return FROM_DERIVED._builder.GetInsertBlock()->getParent();
    }

private:
    std::list<Block> _blocks;
    llvm::AllocaInst *_return_alloca_ptr;
};
