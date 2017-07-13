//
// Created by monty on 04-07-2017.
//

#include <functional>
#include <array>
#include <memory>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

#include <Common.h>

namespace odb {

    float Q_rsqrt(float number) {
        long i;
        float x2, y;
        const float threehalfs = 1.5F;

        x2 = number * 0.5F;
        y = number;
        i = *(long *) &y;                       // evil floating point bit level hacking
        i = 0x5f3759df - (i >> 1);               // what the fuck?
        y = *(float *) &i;
        y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

        return y;
    }

    int wrap360(int i) {
        int angle = static_cast<int>( i ) % 360;

        while (angle < 0) {
            angle += 360;
        }

        return angle;
    }

    ActorAppearance::ActorAppearance( Vec2f aPosition, EActorType aType, float aAngle, float aSquaredDistance ) :
            mPosition( aPosition ), mType( aType ), mAngle( aAngle ), mSquaredDistance( aSquaredDistance ) {
    }
}

