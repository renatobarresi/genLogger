set(app_sources
    app/src/main.cpp
    platform/src/posixThreads.cpp
    platform/src/rtosFactory.cpp
)

set(app_includes
    platform/inc/
)

add_executable(app 
    ${app_sources}
)

target_include_directories(app PRIVATE
    ${app_includes}
)

target_link_libraries(app PRIVATE
    -pthread
)

add_custom_command(TARGET app POST_BUILD 
    COMMAND ${SIZE} app
)