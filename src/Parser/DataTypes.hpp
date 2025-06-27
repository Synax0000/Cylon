#include <iostream>
#include <vector>
#include <string>

#ifndef DATATYPES_HPP
#define DATATYPES_HPP

typedef enum VariableDataTypeVariant {
    DataTypeVariant_Byte,

    DataTypeVariant_Constant,
    DataTypeVariant_Unsigned,
    DataTypeVariant_Custom
} VariableDataTypeVariant;

typedef enum VariableDataType {
    DataType_Byte,
    DataType_Modifier,
    DataType_Custom
} VariableDataType;

typedef struct DataTypeVariant {
    VariableDataTypeVariant Type;
    std::string Value;

    DataTypeVariant() = default;
    DataTypeVariant(VariableDataTypeVariant TypeValue, std::string ValueValue) {
        Value = ValueValue;
        Type = TypeValue;
    }
} DataTypeVariant;

typedef struct DataType {
    VariableDataType Type;
    std::vector<DataTypeVariant> Variants;
    std::string Value;
    bool Modifier;

    DataType() = default;
    DataType(VariableDataType TypeValue, std::vector<DataTypeVariant> VariantsValue, std::string ValueValue, bool ModifierValue) {
        Value = ValueValue;
        Type = TypeValue;
        Variants = VariantsValue;
        Modifier = ModifierValue;
    }
} DataType;

extern std::vector<DataType> DataTypes;

#endif