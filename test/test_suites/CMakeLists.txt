target_sources(
    ${TARGET}
    PRIVATE
        color.h

        04_nesting_container.cpp
        05_deserialize_booleans.cpp
        06_deserialize_integer.cpp
        07_deserialize_floats.cpp
        08_deserialize_strings.cpp
        09_deserialize_attributes.cpp
        10_deserialize_container_attributes.cpp
        11_serialize_and_deserialize_color.cpp
        13_get_wrong_value_type.cpp
        14_value_string_conversion.cpp
        16_addition_assignment.cpp
        17_deserializing_empty_container.cpp
        18_load_and_save_file.cpp

        attributes.cpp
        container.cpp
        container_creation.cpp
        container_lookup.cpp
        lists.cpp
        values.cpp
)
