add_executable(
	c3sharingDataBetweenThreads 
	c3sharingDataBetweenThreads.cpp 
	3_1.cpp 
	3_1.h 
	3_2.h 
	3_2.cpp 
	3_3.cpp 
	threadsafe_stack.h 
	threadsafe_stack.cpp 
	3_6.cpp 
	3_7.cpp 
	3_8_hierarchical_mutex.h
	3_8_hierarchical_mutex.cpp 
	3_9.cpp 
	3_10.cpp 
	3_11.cpp 
	3_12.cpp
	3_13.cpp)
	
target_link_libraries(c3sharingDataBetweenThreads Threads::Threads)
