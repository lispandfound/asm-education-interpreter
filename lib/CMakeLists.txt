project(asminterpreter)
include_directories(include)
add_subdirectory(src)
set_property(TARGET asminterpreter PROPERTY C_STANDARD 11)
