cmake_minimum_required(VERSION 3.25)
project(SW_project C)

set(CMAKE_C_STANDARD 11)

add_executable(SW_project
        final_proj/symnmf.c
        final_proj/symnmfmodule.c final_proj/symnmf.h final_proj/general.c final_proj/general.h)
