cmake_minimum_required(VERSION 3.27)
project(proto-cfg-file)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Protobuf REQUIRED)

# create an interface library to link against protobuf
add_library(google_protobuf INTERFACE)
target_include_directories(google_protobuf INTERFACE ${Protobuf_INCLUDE_DIRS})
target_link_libraries(google_protobuf INTERFACE ${Protobuf_LIBRARIES})
add_library(google::protobuf ALIAS google_protobuf)

add_library(config_proto OBJECT config.proto)
target_link_libraries(config_proto google::protobuf)

set(proto_generated_dir ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(config_proto PUBLIC $<BUILD_INTERFACE:${proto_generated_dir}>)

protobuf_generate(
    TARGET config_proto
    IMPORT_DIRS ${CMAKE_SOURCE_DIR}
    PROTOC_OUT_DIR ${proto_generated_dir}
)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE google::protobuf config_proto)