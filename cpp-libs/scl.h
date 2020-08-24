#pragma once


/* usefull macros:
 *		SCL_THROW_EXCEPTIONS
*/

// #define SCL_USE_ERROR

#include "exit_codes.h"
#include "types.h"

#ifdef SCL_USE_ERROR
#include "error.h"
#else
#include "exceptions.h"
#endif

#include "math.h"
#include "safe.h"

#include "memory.h"
#include "io.h"
#include "pxld.h"
#include "machine.h"

#include "arrays.h"
#include "vectors.h"
#include "buffers.h"
