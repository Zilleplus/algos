add_executable(
	chapter4
	main.cpp 
	4_1.cpp 
	4_5_thread_safe_queue.h 
	4_5_thread_safe_queue.cpp 
	4_5_thread_safe_demo.h
	4_6.h
	4_6.cpp
	4_7.h
	4_7.cpp
	4_8.h)
target_link_libraries(chapter4 Threads::Threads)
