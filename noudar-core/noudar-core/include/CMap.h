#ifndef NOUDAR_CORE_CMAP_H
#define NOUDAR_CORE_CMAP_H

namespace Knights {

	const static int kMapSize = 40;

    class CBlockProperties {
    public:
        explicit CBlockProperties(bool aBlockMovement, bool aBlockProjectile, bool aBlockView):
                mBlockMovement(aBlockMovement), mBlockProjectile(aBlockProjectile), mBlockView(aBlockView){
        }

        explicit CBlockProperties():
                mBlockMovement(false), mBlockProjectile(false), mBlockView(false){
        }


        bool mBlockMovement = false;
        bool mBlockProjectile = false;
        bool mBlockView = false;
    };

    class CMap {
    public:
		void floodFill( Vec2i position, std::map<ElementView, std::pair<ElementView, CBlockProperties> > transformations );

	private:

        std::shared_ptr<IMapElement> map[ kMapSize ][ kMapSize ];
        std::shared_ptr<CActor> mActors[ kMapSize ][ kMapSize ];
        std::shared_ptr<CGameDelegate> mGameDelegate;
        std::array< std::array<CBlockProperties, kMapSize>, kMapSize> mTileBlockProperties;
        ElementView mElement[ kMapSize ][ kMapSize ];
	    std::array< std::array< std::shared_ptr<CItem>, kMapSize >, kMapSize > mItems;
        std::vector<std::shared_ptr<CActor>> actors;
        std::shared_ptr<CActor> mAvatar;
		ActorId mCurrentId = 1;

	public:

        bool isValid( const Vec2i& p ) const;
        bool isBlockMovementAt(const Vec2i &p) const;
        bool isBlockProjectilesAt( const Vec2i &p ) const;
        bool isBlockViewAt( const Vec2i &p ) const;
        CMap( const std::string& data, std::shared_ptr<CGameDelegate> aGameDelegate );
        void move( EDirection d, std::shared_ptr<CActor> a );
        void endOfTurn();
	    bool isLevelFinished();
        Vec2i getActorTargetPosition( std::shared_ptr<CActor> a );
		Vec2i getTargetProjection( std::shared_ptr<CActor> a );
        ElementView getElementAt( const Vec2i& p ) const;
		ItemView getItemViewAt( const Vec2i& p ) const;
		std::shared_ptr<CActor> getAvatar();
        std::vector<std::shared_ptr<CActor>> getActors();
        std::shared_ptr<CActor> getActorAt( Vec2i position );
        void setActorAt( Vec2i position, std::shared_ptr<CActor> actor );
		void removeActorFrom( Vec2i position );
        void moveActor( Vec2i from, Vec2i to, std::shared_ptr<CActor> actor );
        std::shared_ptr<CActor> attack( std::shared_ptr<CActor> a, Vec2i position , bool mutual );
	    Vec2i projectHitscanPosition(Vec2i position, EDirection dir);
	    void giveItemAt( Vec2i from, std::shared_ptr<CActor> to );
		std::shared_ptr<Knights::CItem> getItemAt( Vec2i from );
		void putItemAt( std::shared_ptr<CItem>, Vec2i destination );
		void addActorAt( std::shared_ptr<CActor> actor, const Vec2i& position );
		ActorId getLastestId();
		std::shared_ptr<CGameDelegate> getGameDelegate();
    };
}
#endif
