#include "DataTypes.hpp"

#include <iostream>
#include <vector>
#include <string>

extern std::vector<DataType> DataTypes = { // the default variant is always the first one in the list
    DataType(
        DataType_Byte, {
            DataTypeVariant(DataTypeVariant_Byte, "byte"),
        },
        "byte",
        false
    ),

    DataType(
        DataType_Modifier, {
            DataTypeVariant(DataTypeVariant_Constant, "constant"),
        },
        "constant",
        true
    ),

    DataType(
        DataType_Modifier, {
            DataTypeVariant(DataTypeVariant_Constant, "unsigned"),
        },
        "unsigned",
        true
    )
};
