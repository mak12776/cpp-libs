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

#include "err.h"		// <cinttypes>
#include "log.h"		// <cstdio> <cstdarg>
#include "mem.h"		// "log.h" "err.h"
#include "cleaner.h"	// "mem.h"

#include "builtins.h"
#include "string.h"		// <string> "mem.h"
#include "linked_array.h"

#include "exit_codes.h"
#include "io.h"			/* <sys/types.h> <sys/stat.h> <io.h>
						// "err.h" "mem.h" "builtins.h" */
#include "math.h" 
#include "rand.h"
