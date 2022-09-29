set(CRASHPAD_HANDLER_LIBRARY_FILES
    crash_report_upload_thread.cc
    crash_report_upload_thread.h
    handler_main.cc
    minidump_to_upload_parameters.cc
    minidump_to_upload_parameters.h
    prune_crash_reports_thread.cc
    prune_crash_reports_thread.h
    user_stream_data_source.cc
    user_stream_data_source.h
)

if (APPLE)
    list(APPEND CRASHPAD_HANDLER_LIBRARY_FILES
        mac/crash_report_exception_handler.cc
        mac/crash_report_exception_handler.h
        mac/exception_handler_server.cc
        mac/exception_handler_server.h
        mac/file_limit_annotation.cc
        mac/file_limit_annotation.h
    )
endif (APPLE)

if (LINUX)
    list(APPEND CRASHPAD_HANDLER_LIBRARY_FILES
        linux/capture_snapshot.cc
        linux/capture_snapshot.h
        linux/crash_report_exception_handler.cc
        linux/exception_handler_server.cc
        linux/exception_handler_server.h
    )
endif (LINUX)

if (WIN32)
    list(APPEND CRASHPAD_HANDLER_LIBRARY_FILES
        win/crash_report_exception_handler.cc
        win/crash_report_exception_handler.h
    )
endif (WIN32)

if (ANDROID)
    list(APPEND CRASHPAD_HANDLER_LIBRARY_FILES
        crashpad_handler_main.cc
    )
endif (ANDROID)

add_executable(crashpad_handler main.cc)
target_sources(crashpad_handler PRIVATE ${CRASHPAD_HANDLER_LIBRARY_FILES})
target_compile_features(crashpad_handler PRIVATE cxx_std_17)
target_include_directories(crashpad_handler PRIVATE ..)
target_link_libraries(crashpad_handler PRIVATE crashpad_client crashpad_tools crashpad_compat crashpad_snapshot crashpad_minidump)

if (APPLE)
    target_link_libraries(crashpad_handler PUBLIC -lbsm)
endif (APPLE)

if (WIN32)
    target_link_libraries(crashpad_handler PUBLIC crashpad_getopt)
endif (WIN32)

if (ANDROID)
    if (CUSTOM_CRASHPAD_HANDLER_DESTINATION)
        add_custom_command(TARGET handler POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:handler>
            ${CUSTOM_CRASHPAD_HANDLER_DESTINATION}
        )
    endif (CUSTOM_CRASHPAD_HANDLER_DESTINATION)
endif (ANDROID)
