set(tests
    mlock_test.cpp
    nanosleep_test.cpp
    rw_mutex_test.cpp
    rw_spinlock_test.cpp
    semaphore_test.cpp
    spin_lock_test.cpp
    thread_priority_test.cpp
)

foreach(test ${tests})
  string(REPLACE .cpp "" test_name ${test} )
  add_executable(${test_name} ${test})
  target_link_libraries(${test_name} pthread boost_thread-mt)
  add_test(${test_name}_run ${EXECUTABLE_OUTPUT_PATH}/${test_name})
endforeach(test)
