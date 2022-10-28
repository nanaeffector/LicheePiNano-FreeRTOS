#include "fpn.h"

const FPN fpnMax = {{0x7FFFFFFF}};
const FPN fpnMin = {{0x80000000}};
const FPN fpnZero = VALUE2FRAC(0);
const FPN fpnOne = VALUE2FRAC(1);
const FPN fpnMOne = VALUE2FRAC(-1);
const FPN fpnTwo = VALUE2FRAC(2);
const FPN fpnMTwo = VALUE2FRAC(-2);
const FPN fpnPI = VALUE2FRAC(3.14159265F);