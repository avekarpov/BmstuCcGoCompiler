#pragma once

#include <llvm/IR/Value.h>

#include "tools.hpp"

PARENT_CLASS IrStingBuilder {
public:
    ValueWrapper getConstGlobalStringPtr(const std::string string) {
        std::string escaped_string;

        for (size_t i = 0; i < string.size(); ++i) {
            if (string[i] == '\\') {
                ++i;

                if (i == string.size()) {
                    throw std::runtime_error {
                        "expected one more char after '\\'"
                    };
                }

                switch (string[i]) {
                    case 'n': escaped_string += '\n'; break;
                    case 't': escaped_string += '\t'; break;
                    default: throw std::runtime_error { "unknow escaped char" };
                }
            }
            else {
                escaped_string += string[i];
            }
        }

        auto array_pointer_tw = FROM_DERIVED.getArrayType(
            FROM_DERIVED.getInt8Type(),
            escaped_string.size()
        ).getPointerTo();

        return ValueWrapper {
            FROM_DERIVED._builder.CreateGlobalStringPtr(escaped_string),
            array_pointer_tw
        };
    }
};
