add_executable(test_ext_net)

target_include_directories(test_ext_net
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/net"
        "${PROJECT_SOURCE_DIR}/ceammc/extra"
        "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(test_ext_net PUBLIC
    tests_main_lib
    ceammc_net
    ceammc_core
    puredata-core)

add_test(NAME test_ext_net
    COMMAND test_ext_net
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_net_test src)
    target_sources(test_ext_net PRIVATE "test_net_${src}.cpp")
endfunction()


add_net_test(host)
