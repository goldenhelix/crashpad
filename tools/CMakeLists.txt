set(CRASHPAD_TOOLS_LIBRARY_FILES
    ./tool_support.cc
    ./tool_support.h
)

add_library(crashpad_tools OBJECT ${CRASHPAD_TOOLS_LIBRARY_FILES})
target_compile_features(crashpad_tools PRIVATE cxx_std_14)

target_include_directories(crashpad_tools PUBLIC ..)
target_link_libraries(crashpad_tools PUBLIC mini_chromium backtrace_common crashpad_util crashpad_compat crashpad_snapshot)
