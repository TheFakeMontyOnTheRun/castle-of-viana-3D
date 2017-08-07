#include <string>
#include <memory>
#include <utility>
#include <iostream>

#include "Vec2i.h"
#include "IMapElement.h"
#include "CDoorway.h"

namespace Knights {

    CDoorway::CDoorway() : IMapElement() {
        mView = 'E';
    }
}