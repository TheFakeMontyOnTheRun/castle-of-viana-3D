#ifndef NOUDAR_CORE_IRENDERER_H
#define NOUDAR_CORE_IRENDERER_H
namespace Knights {

    using CommandType = char;

  class IRenderer {
  public:
    virtual void drawMap( CMap &map, std::shared_ptr<CActor> current ) = 0;
    virtual CommandType getInput() = 0;
      virtual ~IRenderer(){
      };
  };
}
#endif
