#pragma once

#ifndef SCL_NOT_INC_GLOBAL_HEADERS

#include <safeint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <Windows.h>

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
#include "math.h"					// <safeint.h
#include "fs.h"
#include "rand.h"

#include "types/builtins.h"

#include "features/err.h"
#include "features/mem.h"	// "features/err.h"
#include "features/cfmt.h"	// "types/builtins.h" "feautres/err.h"		

#include "types/sorted_vector.h"
#include "types/bitset.h"
#include "types/buffer.h"
#include "types/string.h"			// <string> "mem.h" "math.h"
#include "types/linked_array.h"



#include "io.h"						// <sys/types.h> <sys/stat.h> <io.h>
									// "features/all_features.h"

#include "winapi/all_winapi.h"




