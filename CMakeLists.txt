cmake_minimum_required(VERSION 3.14)

# 프로젝트 루트 디렉토리를 설정
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(DESIGN_PATTERN)

# C++14 표준을 사용하도록 설정
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# # 모듈 패스 설정
# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/module" ${CMAKE_MODULE_PATH})
# message(STATUS "${CMAKE_MODULE_PATH}")
# include(V2XClangTidyFlags)


# # clang-tidy를 통해 정적 분석 수행
# option(CLANG_TIDY_FIX "Perform fixes for Clang-Tidy" ON)
# find_program(
#     CLANG_TIDY_EXE
#     NAMES "clang-tidy"
#     DOC "Path to clang-tidy executable"
# )
# find_program( # clang이 사용되는 것은 아니나, clang-tidy의 올바른 동작을 위해 필요함
#     CLANG_EXE
#     NAMES "clang"
#     DOC "Path to clang executable"
# )

# # 정적 분석 준비
# if((NOT CLANG_TIDY_EXE) OR (NOT CLANG_EXE))
#     message(FATAL_ERROR "clang-tidy tool not found")
# endif()

# include 디렉터리 추가
include_directories(${PROJECT_ROOT_DIR}/include)

# 소스 파일 목록을 가져옴
file(GLOB SOURCES "${PROJECT_ROOT_DIR}/src/*.cpp")

# 실행 파일이 생성될 디렉터리를 설정
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/build/bin)

# 빌드 대상 프로그램 생성
add_executable(main ${SOURCES} ${PROJECT_ROOT_DIR}/main.cpp)

find_package(spdlog REQUIRED)
target_link_libraries(main
    PRIVATE
        spdlog::spdlog
)
