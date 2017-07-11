//
// Created by monty on 27/08/16.
//

#ifndef BLANKSLATE_CGAMESNAPSHOT_H
#define BLANKSLATE_CGAMESNAPSHOT_H

namespace odb {

  class CGameSnapshot {
    public:
      std::vector<std::tuple<int, int, int>> mVisibleCharacters;
    RayCollision mCurrentScan[ 90 ];
    CActor mCamera;
    };
}

#endif //BLANKSLATE_CGAMESNAPSHOT_H
