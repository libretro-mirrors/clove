#include "tools.h"

inline char const *fh_tools_toStringOrError(struct fh_program *prog, struct fh_value *args, int index)
{
    if (!fh_is_string(&args[index])){
        clove_running = false;
        fh_print_error(prog, "");
        printf("Expected string value at position %d\n", index);
        return "";
    }
    return GET_VAL_STRING_DATA(&args[index]);
}

inline double fh_tools_toNumberOrError(struct fh_program *prog, struct fh_value *args, int index)
{
    if (!fh_is_number(&args[index])){
        clove_running = false;
        fh_print_error(prog, "");
        printf("Expected number value at position %d\n", index);
        return -1;
    }
    return args[index].data.num;
}
