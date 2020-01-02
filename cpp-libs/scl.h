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
#include "error.h"
#include "math.h"
#include "safe.h"

#include "static_array.h"
#include "dynamic_array.h"
#include "static_dyanamic_array.h"

#include "array_view.h"

#include "array_vector.h"

#include "fixed_vector.h"

#include "linked_vector_list.h"

#include "io.h"
#include "buffer.h"
#include "string.h"
#include "memory.h"
#include "pxld.h"
#include "machine.h"
