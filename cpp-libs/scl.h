#pragma once

#include <string>
#include <iostream>

/* usefull macros:
 *		SCL_THROW_EXCEPTIONS
*/

#define SCL_THROW_EXCEPTIONS

#if CHAR_MAX != INT8_MAX
#error invalid char size
#endif

#include "exit_codes.h"
#include "types.h"
#include "math.h"
#include "safe.h"

#include "memory.h"
#include "io.h"
#include "pxld.h"
#include "machine.h"

#include "arrays.h"
#include "vectors.h"
#include "buffers.h"
