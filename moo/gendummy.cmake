FILE(WRITE ${OUTPUT} "")
FILE(APPEND ${OUTPUT} "/* dummy file to please cmake which doesn't know that files can be generated, for ${LIBNAME} */\n")
FILE(APPEND ${OUTPUT} "#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)\n")
FILE(APPEND ${OUTPUT} "__attribute__((__unused__))\n")
FILE(APPEND ${OUTPUT} "#endif\n")
FILE(APPEND ${OUTPUT} "static void _moo_dummy_cmake_func_${LIBNAME} (void) {}\n")
