cmake_minimum_required(VERSION 3.5)

project(os_lab)

set(CMAKE_C_STANDARD 11)

set(SOURCES

    src/fork_demo.c
    src/exec_demo.c
    src/pipe_demo.c
    src/pipline_demo.c
    src/zombie_demo.c
    )
add_executable(fork_demo src/fork_demo.c)
add_executable(exec_demo src/exec_demo.c)
add_executable(pipe_demo src/pipe_demo.c)
add_executable(pipeline_demo src/pipeline_demo.c)
add_executable(zombie_demo src/zombie_demo.c)
# Add challenge executables
add_executable(ch1_single_fork challenges/ch1_single_fork.c)
add_executable(ch2_multi_child challenges/ch2_multi_child.c)
add_executable(ch3_exec_ls challenges/ch3_exec_ls.c)
add_executable(worker challenges/worker.c)
add_executable(ch4_exec_worker challenges/ch4_exec_worker.c)
add_executable(ch5_exec_examples challenges/ch5_exec_examples.c)
add_executable(ch6_pipe_sum challenges/ch6_pipe_sum.c)
add_executable(ch7_pipeline challenges/ch7_pipeline.c)
add_executable(ch8_wait_nonblock challenges/ch8_wait_nonblock.c)
add_executable(ch9_zombie challenges/ch9_zombie.c)
add_executable(ch10_pool challenges/ch10_pool.c)
