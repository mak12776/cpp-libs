#pragma once

#ifndef SCL_NOT_INC_GLOBAL_HEADERS

#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

#include <random>
#include <cinttypes>
#include <cstdio>
#include <cstdarg>
#include <string>

#endif

#include "features/err.h"		// <cinttypes>
#include "features/log.h"		// <cstdio> <cstdarg>
#include "features/mem.h"		// "features/log.h", "features/err.h"
#include "features/cleaner.h"	// "features/mem.h"

#include "types/builtins.h"
#include "types/string.h"		// <string>, "features/mem.h"
#include "types/linked_array.h"

#include "exit_codes.h"
#include "io.h"	/*	<sys/types.h>, <sys/stat.h>, <io.h>
					"features/err.h", "features/mem.h", 
					"types/builtins.h" */
#include "math.h" 
#include "rand.h"
