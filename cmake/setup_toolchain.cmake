
if (CMAKE_CXX_COMPILER MATCHES "MSVC")
    add_definitions(/W4 /WX)
else()
    message(FATAL_ERROR "Unsupported compiler, please enable highest warning level and enable warning as error")
endif()