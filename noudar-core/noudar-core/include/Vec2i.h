//
// Created by monty on 05/10/16.
//

#ifndef NOUDAR_CORE_VEC2I_H
#define NOUDAR_CORE_VEC2I_H
namespace Knights {

    class Vec2i {
    public:
        int x = 0;
        int y = 0;

        Vec2i( int aX, int aY );

        Vec2i();

        Vec2i& operator+=( const Vec2i &other );

        Vec2i& operator-=( const Vec2i &other );
    };

    enum class EDirection {
        kNorth,
        kEast,
        kSouth,
        kWest
    };

    Vec2i operator+( const Vec2i &lh, const Vec2i &rh );

    bool operator==( const Vec2i &lh, const Vec2i &rh );

    bool operator!=( const Vec2i &lh, const Vec2i &rh );

    std::ostream& operator<<(std::ostream& os, const EDirection& aDirection );
    std::string to_string( const EDirection& aDirection );

    std::ostream& operator<<(std::ostream& os, const Vec2i& aVec );
    std::string to_string( const Vec2i& aVec );

    EDirection wrapDirection( EDirection direction, int offset );
    Vec2i mapOffsetForDirection( EDirection direction );
    EDirection leftOf(Knights::EDirection d);
    EDirection rightOf(Knights::EDirection d);
    EDirection oppositeOf(Knights::EDirection d);
}
#endif //KNIGHTS_IN_PORTO_VEC2I_H
