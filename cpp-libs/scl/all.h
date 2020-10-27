#pragma once

#ifndef SCL_NOT_INC_GLOBAL_HEADERS

#include <safeint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

#include <random>
#include <cinttypes>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <variant>

#endif

#ifndef SCL_LOCKED_FEATURES
#define SCL_LOCKED_FEATURES 1
#endif


#include "macros.h"
#include "exit_codes.h"
#include "builtins.h"
#include "math.h"					// <safeint.h
#include "rand.h"

#include "features/all_features.h"	// <cinttypes> <cstdio> cstdarg>

#include "types/all_types.h"		// <string> "features/all_features.h"
									// "math.h"

#include "io.h"						// <sys/types.h> <sys/stat.h> <io.h>
									// "features/all_features.h"




