#include "pch.h"
#include "RandomHelper.h"

#include <time.h>

void Helper::RandomInit()
{
    srand((uint32_t)time(nullptr));
}
