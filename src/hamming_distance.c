#include "postgres.h"
#include "fmgr.h"

#include <stdint.h>
#include <stdlib.h>

#include "hamming_distance.h"


#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


static int64_t bit_distance_from_hex(char *a, char* b, uint32_t len);
static int64_t bit_similarity_impl(VarChar *a, VarChar *b);
static int64_t bit_distance_impl(VarChar *a, VarChar *b);
static double bit_similarity_normalized_impl(VarChar *a, VarChar *b);
static double bit_distance_normalized_impl(VarChar *a, VarChar *b);


static const int hextable[103] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,
    3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,  0,  10, 11, 12,
    13, 14, 15, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  10, 11, 12, 13, 14, 15
};


PG_FUNCTION_INFO_V1(hamming_distance);
Datum
hamming_distance(PG_FUNCTION_ARGS) {
    VarChar *a = (VarChar *)PG_GETARG_VARCHAR_P(0);
    VarChar *b = (VarChar *)PG_GETARG_VARCHAR_P(1);
    int64_t res;

    if(VARSIZE(a)  != VARSIZE(b)) RAISE_HAMMINGDISTANCE_PAIR_LEN_ERROR;
    if((res = bit_distance_impl(a, b)) == -1) RAISE_HAMMINGDISTANCE_WRONG_TYPE;
    PG_RETURN_INT64(res);
}


PG_FUNCTION_INFO_V1(hamming_similarity);
Datum
hamming_similarity(PG_FUNCTION_ARGS) {
    VarChar *a = (VarChar *)PG_GETARG_VARCHAR_P(0);
    VarChar *b = (VarChar *)PG_GETARG_VARCHAR_P(1);
    int64_t res;

    if(VARSIZE(a)  != VARSIZE(b)) RAISE_HAMMINGDISTANCE_PAIR_LEN_ERROR; 
    if((res = bit_similarity_impl(a, b)) == -1) RAISE_HAMMINGDISTANCE_WRONG_TYPE;
    PG_RETURN_INT64(res);
}


PG_FUNCTION_INFO_V1(hamming_distance_normalized);
Datum
hamming_distance_normalized(PG_FUNCTION_ARGS) {
    VarChar *a = (VarChar *)PG_GETARG_VARCHAR_P(0);
    VarChar *b = (VarChar *)PG_GETARG_VARCHAR_P(1);
    double res;

    if(VARSIZE(a)  != VARSIZE(b)) RAISE_HAMMINGDISTANCE_PAIR_LEN_ERROR;
    if((res = bit_distance_normalized_impl(a, b)) < 0.0) RAISE_HAMMINGDISTANCE_WRONG_TYPE;
    PG_RETURN_FLOAT8(res);
}


PG_FUNCTION_INFO_V1(hamming_similarity_normalized);
Datum
hamming_similarity_normalized(PG_FUNCTION_ARGS) {
    VarChar *a = (VarChar *)PG_GETARG_VARCHAR_P(0);
    VarChar *b = (VarChar *)PG_GETARG_VARCHAR_P(1);
    double res;

    if(VARSIZE(a)  != VARSIZE(b)) RAISE_HAMMINGDISTANCE_PAIR_LEN_ERROR; 
    if((res = bit_similarity_normalized_impl(a, b)) < 0.0) RAISE_HAMMINGDISTANCE_WRONG_TYPE;
    PG_RETURN_FLOAT8(res);
}

// use int64_t couse in case of error we returning -1
static int64_t
bit_distance_from_hex(char *a, char* b, uint32_t len) {
    int64_t acc = 0;
    unsigned int aswap = 0;
    unsigned int bswap = 0;
    uint32_t i;
    int ta;
    int tb;

    for(i = 0; i < len; i++) {
        ta = (int)a[i];
        tb = (int)b[i];
        if(ta > 102 || tb > 102) return -1;
        
        /*
        assuming that it is faster to call popcount on 8 bit (N / 2) times 
        with even check than calling it (N) times without

                      name | rank |  runs |      mean |        sd | timesBaseline
        -------------------|------|-------|-----------|-----------|--------------
                  distance |    1 | 1e+06 | 1.967e-05 | 1.833e-05 |           1.0
        distance candidate |    2 | 1e+06 |  1.97e-05 |  2.77e-05 | 1.00128189431

        Each of the above 2000000 runs were run in random, non-consecutive order by
        `benchmark` v0.1.5 (http://jspi.es/benchmark) with Python 2.7.10
        Darwin-15.3.0-x86_64 on 2016-07-20 06:03:34.
        
        So this version is slightly faster
        */
        if(~i & 1) {
            aswap |= hextable[ta] << HEX_GROUP_HALF_SIZE;
            bswap |= hextable[tb] << HEX_GROUP_HALF_SIZE;
            continue;
        }

        aswap |= hextable[ta];
        bswap |= hextable[tb];
        acc += __builtin_popcount(aswap ^ bswap);
        aswap = 0;
        bswap = 0;
    }
    return acc;
}


static int64_t
bit_similarity_impl(VarChar *a, VarChar *b) {
    uint32_t len = VARSIZE(a) - VARHDRSZ;
    return (len * HEX_GROUP_HALF_SIZE) - bit_distance_from_hex(VARDATA(a), VARDATA(b), len);
}


static int64_t
bit_distance_impl(VarChar *a, VarChar *b) {
    uint32_t len = VARSIZE(a) - VARHDRSZ;
    return bit_distance_from_hex(VARDATA(a), VARDATA(b), len);
}


static double
bit_distance_normalized_impl(VarChar *a, VarChar *b) {
    uint32_t len = VARSIZE(a) - VARHDRSZ; 
    uint32_t glen = len * HEX_GROUP_HALF_SIZE;
    if (glen == 0) {
        return 0.0;
    }
    return (double)bit_distance_from_hex(VARDATA(a), VARDATA(b), len) / (double)(glen);
}


static double
bit_similarity_normalized_impl(VarChar *a, VarChar *b) {
    return 1.0 - bit_distance_normalized_impl(a, b);
}

