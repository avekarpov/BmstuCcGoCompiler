execute_process(
    COMMAND bash -c "./gen.sh ${CMAKE_SOURCE_DIR}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    ECHO_OUTPUT_VARIABLE
)
