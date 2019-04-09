#include "tools.h"

inline char const *fh_tools_toStringOrError(struct fh_program *prog, struct fh_value *args, int index)
{
    if (!fh_is_string(&args[index])){
        clove_running = false;
        char buffer[512];
        fh_get_src_loc(prog, buffer, 512);
        clove_error("Error at %s expected string value at position %d\n", buffer, index);
        return "";
    }
    return GET_VAL_STRING_DATA(&args[index]);
}

inline double fh_tools_toNumberOrError(struct fh_program *prog, struct fh_value *args, int index)
{
    if (!fh_is_number(&args[index])){
        clove_running = false;
        char buffer[512];
        fh_get_src_loc(prog, buffer, 512);
        clove_error("Error at %s expected number value at position %d\n", buffer, index);
        return -1;
    }
    return args[index].data.num;
}
