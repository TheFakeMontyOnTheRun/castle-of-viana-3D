#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <conio.h>
#include <iterator>
#include <dpmi.h>
#include <go32.h>
#include <sys/movedata.h>
#include <pc.h>
#include <sys/farptr.h>
#include <bios.h>
#include <sys/nearptr.h>

#include <cstdlib>
#include <cstdio>
#include <functional>
#include <cmath>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <memory>
#include <iostream>
#include <map>
#include <iostream>
#include <string>
#include <sg14/fixed_point>
#include <EASTL/vector.h>
#include <EASTL/array.h>

using eastl::vector;
using eastl::array;

using sg14::fixed_point;

#include "RaycastCommon.h"
#include "Vec2i.h"
#include "IMapElement.h"
#include "CTeam.h"
#include "CItem.h"
#include "CActor.h"
#include "CGameDelegate.h"
#include "CMap.h"
#include "IRenderer.h"
#include "IFileLoaderDelegate.h"
#include "CGame.h"
#include "NativeBitmap.h"
#include "CRenderer.h"

namespace odb {

  std::array<int, 320 * 128> mBuffer;

  int offset = 0;
  std::array< unsigned char, 320 * 128 > buffer;
  int origin = 0;
  int lastOrigin = -1;
  unsigned char shade;
  long frame = 0;



    void CRenderer::put( int x, int y, const std::array<uint8_t ,4>& colour ) {
        int pixel = colour[ 1 ] & 0xFF;
        pixel += (colour[ 2 ] & 0xFF) << 8;
        pixel += (colour[ 3 ] & 0xFF) << 16;

        if ( x < 0 || x >= 320 || y < 0 || y >= 128 ) {
	        return;
        }

        mBuffer[ (320 * y ) + x ] = pixel;
    }

  void CRenderer::fill( int x1, int y1, int w, int h, const std::array<uint8_t,4>& colour ) {
      int _x0 = std::min( 319, std::max( 0, x1) );
      int _x1 = std::min( 319, std::max( 0,  (x1 + w) ) );
      int _y0 = std::min( 127, std::max( 0,  y1 ) );
      int _y1 = std::min( 127, std::max( 0,  (y1 + h) ) );

      int pixel = colour[ 1 ] & 0xFF;
      pixel += (colour[ 2 ] & 0xFF) << 8;
      pixel += (colour[ 3 ] & 0xFF) << 16;

      auto data = std::begin( mBuffer );


      for ( int y = _y0; y < _y1; ++y ) {
        auto line = data + (320 * y );
          std::fill( line + _x0, line + _x1, pixel );
      }
    }
    
  unsigned char getPaletteEntry( int origin ) {
    unsigned char shade = 0;
    shade += (((((origin & 0x0000FF)      ) << 2  ) >> 8 ) ) << 4;
    shade += (((((origin & 0x00FF00) >> 8 ) << 2  ) >> 8 ) ) << 2;
    shade += (((((origin & 0xFF0000) >> 16) << 2  ) >> 8 ) ) << 0;
    return shade;
  }
  
  CRenderer::CRenderer() {
      for ( int c = 0; c < 360; ++c ) {
          auto sin_a = fixed_point<int32_t , -16>{(std::sin((c * 3.14159f) / 180.0f)) / 16.0f};
          auto cos_a = fixed_point<int32_t , -16>{(std::cos((c * 3.14159f) / 180.0f)) / 16.0f};
          sines[ c ] = sin_a;
          cossines[ c ] = cos_a;
      }

      __dpmi_regs reg;
      
      reg.x.ax = 0x13;
      __dpmi_int(0x10, &reg);
      
      outp(0x03c8, 0);
      
      for ( int r = 0; r < 4; ++r ) {
	for ( int g = 0; g < 4; ++g ) {
	  for ( int b = 0; b < 4; ++b ) {
	    outp(0x03c9, (r * (85) ) );
	    outp(0x03c9, (g * (85) ) );
	    outp(0x03c9, (b * (85) ) );
	  }
	}
      }
    }

  void CRenderer::sleep( long ms ) {
  }
  
  void CRenderer::handleSystemEvents() {
    while ( kbhit() ) {
      switch ( getch() ) {
      case 'a':
          mBufferedCommand = Knights::kTurnPlayerLeftCommand;
	break;
      case 'd':
          mBufferedCommand = Knights::kTurnPlayerRightCommand;
	break;
      case 'w':
          mBufferedCommand = Knights::kMovePlayerForwardCommand;
	break;
      case 's':
          mBufferedCommand = Knights::kMovePlayerBackwardCommand;
	break;
      case 'z':
          mBufferedCommand = Knights::kUseCurrentItemInInventoryCommand;
	break;
      default:
	break;
      }
    }
  }

  void CRenderer::flip() {
    for( int offset = 0; offset < 320 * 128; ++offset ) {
      origin = mBuffer[offset];
      
      if ( origin != lastOrigin ) {
	shade = getPaletteEntry( origin );
      }
      
      lastOrigin = origin;
      
      buffer[ offset ] = shade;
    }
    
    dosmemput(&buffer[0], 320 * 128, 0xa0000);

      gotoxy(1,1);
      printf( "%d", ++frame);


  }
}
