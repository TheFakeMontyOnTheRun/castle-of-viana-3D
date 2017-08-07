#ifndef NOUDAR_CORE_CGAME_H
#define NOUDAR_CORE_CGAME_H

namespace Knights {
    const static CommandType kMovePlayerNorthCommand = 'w';
    const static CommandType kMovePlayerEastCommand = 's';
    const static CommandType kMovePlayerSouthCommand = 'z';
    const static CommandType kMovePlayerWestCommand = 'a';
    const static CommandType kMovePlayerForwardCommand = 'o';
	const static CommandType kMovePlayerBackwardCommand = 'k';
	const static CommandType kTurnPlayerLeftCommand = 'i';
    const static CommandType kTurnPlayerRightCommand = 'p';
    const static CommandType kTurnPlayerNorthCommand = 'r';
    const static CommandType kTurnPlayerEastCommand = 'f';
    const static CommandType kTurnPlayerSouthCommand = 'c';
    const static CommandType kTurnPlayerWestCommand = 'd';
    const static CommandType kStrafeLeftCommand = 'j';
    const static CommandType kStrafeRightCommand = 'l';
    const static CommandType kGoToFirstLevelCommand = '1';
    const static CommandType kGoToTitleLevelCommand = '0';
    const static CommandType kEndTurnCommand = 't';
    const static CommandType kQuitGameCommand = 'q';
    const static CommandType kCycleLeftInventoryCommand = '-';
	const static CommandType kCycleRightInventoryCommand = '=';
	const static CommandType kUseCurrentItemInInventoryCommand = '\t';
    const static CommandType kPickItemCommand = '[';
	const static CommandType kDropItemCommand = ']';


	class CGame : public std::enable_shared_from_this<CGame> {
      std::shared_ptr<CMap> mMap;
	  std::shared_ptr<CActor> mPlayerActor;
      std::shared_ptr<IRenderer> mRenderer;
      std::shared_ptr<CGameDelegate> mGameDelegate;
      std::shared_ptr<IFileLoaderDelegate> mFileLoaderDelegate;

      bool mIsPlaying;
      int mTurn = 0;
      int mLevel = 0;
  public:
      CGame( std::shared_ptr<IFileLoaderDelegate> fileLoaderDelegate, std::shared_ptr<IRenderer> aRenderer, std::shared_ptr<CGameDelegate> aGameDelegate );
      void endOfTurn(std::shared_ptr<CMap> map);
      std::shared_ptr<CMap> getMap();
      void tick();
      int getTurn();
      void proceedToNextLevel();
      void playLevel( int level );
      bool isPlaying();
      Vec2i getCursorPosition();
      ActorId getCurrentActorId();
      int getLevelNumber();
      void setIsPlaying( bool isPlaying );
  };
}
#endif
