#ifndef NOUDAR_CORE_IMAPELEMENT_H
#define NOUDAR_CORE_IMAPELEMENT_H
namespace Knights {
    using ElementView = char;
    const ElementView kEmptySpace = '.';
    class IMapElement {
    public:
        ElementView mView = kEmptySpace;
    public:
        ElementView getView();
    };
}
#endif
