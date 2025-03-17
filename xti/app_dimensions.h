#ifndef APP_DIMENSIONS_H
#define APP_DIMENSIONS_H

// 1. Qt framework headers
// 2. System/OS headers
// 3. C++ standard library headers
#include <cstdint>
// 4. Project classes
// 5. Forward decl

struct app_dimensions {
    int32_t dimensionsAvailableScreenWidth;
    int32_t dimensionsAboveYEnd;
    int32_t dimensionsBelowYStart;
    int32_t dimensionsBelowYEnd;
};

#endif // APP_DIMENSIONS_H
