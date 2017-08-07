#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <conio.h>
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
#include <vector>
#include <cstring>
#include <memory>
#include <iostream>
#include <map>
#include <array>
#include <iostream>
#include <string>
#include <vector>

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

  std::array<int, 320 * 200> mBuffer;

  int offset = 0;
  std::array< unsigned char, 320 * 200 > buffer;
  int origin = 0;
  int lastOrigin = -1;
  unsigned char shade;
  long frame = 0;

  void CRenderer::fill( int x1, int y1, int w, int h, std::array<uint8_t,4> colour ) {
      int _x0 = x1;
      int _x1 = (x1 + w);
      int _y0 = y1;
      int _y1 = (y1 + h);

      int pixel = colour[ 1 ] & 0xFF;
      pixel += (colour[ 2 ] & 0xFF) << 8;
      pixel += (colour[ 3 ] & 0xFF) << 16;
      
      for ( int y = _y0; y < _y1; ++y ) {
	    for ( int x = _x0; x < _x1; ++x ) {

	        if ( x < 0 || x >= 320 || y < 0 || y >= 200 ) {
	            continue;
	        }
	  
	        mBuffer[ (320 * y ) + x ] = pixel;
	    }
      }
    };
  
  void drawSquare( int x1, int y1, int x2, int y2, std::array<int,4> colour ) {
      int _x0 = x1 / 2;
      int _x1 = x2 / 2;
      int _y0 = (y1 * 200) / 480;
      int _y1 = (y2 * 200 ) / 480;

      int pixel = colour[ 1 ];
      pixel += colour[ 2 ] << 8;
      pixel += colour[ 3 ] << 8;
      
      for ( int y = _y0; y < _y1; ++y ) {
	for ( int x = _x0; x < _x1; ++x ) {

	  if ( x < 0 || x >= 320 || y < 0 || y >= 200 ) {
	    continue;
	  }
	  
	  mBuffer[ (320 * y ) + x ] = pixel;
	}
      }
    };
    
  unsigned char getPaletteEntry( int origin ) {
    unsigned char shade = 0;
    shade += (((((origin & 0x0000FF)      ) << 2  ) >> 8 ) ) << 4;
    shade += (((((origin & 0x00FF00) >> 8 ) << 2  ) >> 8 ) ) << 2;
    shade += (((((origin & 0xFF0000) >> 16) << 2  ) >> 8 ) ) << 0;
    return shade;
  }
  
  CRenderer::CRenderer() {
      for ( int c = 0; c < 360; ++c ) {
          auto sin_a = (std::sin((c * 3.14159f) / 180.0f)) / 16.0f;
          auto cos_a = (std::cos((c * 3.14159f) / 180.0f)) / 16.0f;
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
    for( int offset = 0; offset < 320 * 200; ++offset ) {
      origin = mBuffer[offset];
      
      if ( origin != lastOrigin ) {
	shade = getPaletteEntry( origin );
      }
      
      lastOrigin = origin;
      
      buffer[ offset ] = shade;
    }
    
    dosmemput(&buffer[0], 64000, 0xa0000);
      gotoxy(1,1);
      std::cout << frame++ << std::endl;
  }
}
