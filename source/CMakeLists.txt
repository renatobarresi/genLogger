add_executable(main)
target_sources(main PRIVATE
               app/src/main.cpp)
target_include_directories(main PRIVATE
                           app/inc/)