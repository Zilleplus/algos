add_executable(helloworld helloworld.cpp)

add_executable(helloworldConcurrent helloworldConcurrent.cpp)
target_link_libraries(helloworldConcurrent Threads::Threads)
