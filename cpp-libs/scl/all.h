#pragma once

#ifndef SCL_NOT_INC_GLOBAL_HEADERS

#if 0

// windows headers

#include <Windows.h>


#include <random>
#include <cinttypes>
#include <cstdarg>

#include <string>

#include <variant>

#endif // 0
// <sys/types.h> <sys/stat.h> <io.h>

// windows headers
#include <safeint.h>	// needed by "math.h"

#include <sys/types.h>	// needed by "io.h"
#include <sys/stat.h>	// needed by "io.h"
#include <io.h>			// needed by "io.h"

// standard template library headers
#include <cstdlib>		// needed by "features/err.h"
#include <cstdint>		// needed by "features/err.h"
#include <cstdio>		// needed by "features/err.h"
#include <cerrno>		// needed by "features/err.h"
#include <cstring>		// needed by "features/err.h"

#include <cmath>		// needed by "math.h"
#include <cfenv>		// needed by "math.h"
#include <iostream>		// needed by "math.h"

#include <random>		// needed by "rand.h"

#include <cinttypes>	// needed by "features/gc.h", "features/err.h"

#endif


// scl headers
#include "macros.h"
#include "exit_codes.h"
#include "math.h"					// needed by "features/mem.h"

#include "rand.h"

#include "types/builtins.h"			// needed by "cfmt.h"

#include "features/cfmt_header.h"	// needed by "features/err.h"

#include "features/err.h"			// needed by "cfmt.h", "mem.h"
#include "features/mem.h"			// needed by "cfmt.h"
#include "features/cfmt.h"
#include "features/gc.h"

#include "io.h"
#include "types/types.h"

#ifdef SCL_INCLUDE_WIN_API
#include "fs.h"

#include "winapi/functions.h"
#include "winapi/conv.h"
#endif