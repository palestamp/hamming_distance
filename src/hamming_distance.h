#ifndef HAMMINGDISTANCE_H
#define HAMMINGDISTANCE_H


#define HEX_GROUP_HALF_SIZE (4)

#define RAISE_HAMMINGDISTANCE_PAIR_LEN_ERROR                                        \
    do {                                                                            \
        ereport(ERROR,                                                              \
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),                          \
                 errmsg("Strings must be equal length")));                          \
    } while(0)                                                                      \

#define RAISE_HAMMINGDISTANCE_WRONG_TYPE                                            \
    do {                                                                            \
        ereport(ERROR,                                                              \
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),                          \
                 errmsg("Can't interpret one of parameters as hex number.")));      \
    } while(0)                                                                      \

#endif
