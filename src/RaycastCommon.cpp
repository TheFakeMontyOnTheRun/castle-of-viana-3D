//
// Created by monty on 04-07-2017.
//

#include <functional>
#include <memory>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sg14/fixed_point>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

using sg14::fixed_point;

#include "RaycastCommon.h"

namespace odb {

    int wrap360(int i) {
        int angle = static_cast<int>( i ) % 360;

        while (angle < 0) {
            angle += 360;
        }

        return angle;
    }
}

