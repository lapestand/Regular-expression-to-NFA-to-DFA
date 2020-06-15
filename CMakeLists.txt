cmake_minimum_required(VERSION 3.16)
project(RE_FA)

set(CMAKE_CXX_STANDARD 20)

add_executable(RE_FA main.cpp src/NFA.cpp include/NFA.h src/RegexParser.cpp include/RegexParser.h src/Tester.cpp include/Tester.h src/DFA.cpp include/DFA.h)