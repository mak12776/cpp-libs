#pragma once

/* usefull macros:
 *		SCL_CATCH_EXCEPTIONS
*/



#include <random>
#include <cinttypes>
#include <cstdio>
#include <cstdarg>

#include "exit_codes.h" 
#include "math.h" 

#include "features/err.h"		// <cinttypes>
#include "features/log.h"		// <cstdio> <cstdarg>
#include "features/mem.h"		// "features/log.h", "features/err.h"
#include "features/cleaner.h"	// "features/mem.h"

#include "io.h"					// <sys/types.h>, <sys/stat.h>, <io.h>
								// "features/err.h", "features/mem.h", "types/builtins.h"

