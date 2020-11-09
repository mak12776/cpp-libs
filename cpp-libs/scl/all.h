#pragma once

#ifndef SCL_NOT_INC_GLOBAL_HEADERS

// windows headers
#include <safeint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <Windows.h>

// standard template library headers
#include <random>
#include <cinttypes>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <variant>

#endif

// scl headers

#include "macros.h"
#include "exit_codes.h"
#include "math.h"		// <safeint.h
#include "rand.h"

#include "types/builtins.h"

#include "features/err.h"		// "features/cfmt.h"
#include "features/cfmt.h"		// "types/builtins.h" "feautres/err.h"
#include "features/mem.h"		// "features/err.h"
#include "features/cleaner.h"	// <cinttypes>

#include "types/buffer.h"
#include "types/string.h"			// <string> "mem.h" "math.h"

#include "io.h"						// <sys/types.h> <sys/stat.h> <io.h>
									// "features/all_features.h"

#include "winapi/functions.h"
#include "winapi/tools.h"




