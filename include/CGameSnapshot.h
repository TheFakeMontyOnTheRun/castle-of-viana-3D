//
// Created by monty on 27/08/16.
//

#ifndef BLANKSLATE_CGAMESNAPSHOT_H
#define BLANKSLATE_CGAMESNAPSHOT_H

namespace odb {

  class CGameSnapshot {
    public:
    RayCollision mCurrentScan[ 90 ];
    Vec2f mCamera = {0, 0};
    float mAngle = 0;
    };
}

#endif //BLANKSLATE_CGAMESNAPSHOT_H
