//#link "chr_generic.s"
#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// include CC65 NES Header (PPU)
#include <nes.h>

///// METASPRITES

// define a 2x2 metasprite
#define DEF_METASPRITE_2x2(name,code,pal)\
const unsigned char name[]={\
        0,      0,      (code)+0,   pal, \
        0,      8,      (code)+1,   pal, \
        8,      0,      (code)+2,   pal, \
        8,      8,      (code)+3,   pal, \
        128};

DEF_METASPRITE_2x2(mySprite, 0, 0);

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x03,			// screen color

  0x11,0x30,0x27,0x0,	// background palette 0
  0x1c,0x20,0x2c,0x0,	// background palette 1
  0x00,0x10,0x20,0x0,	// background palette 2
  0x06,0x16,0x26,0x0,	// background palette 3

  0x16,0x35,0x24,0x0,	// sprite palette 0
  0x00,0x37,0x25,0x0,	// sprite palette 1
  0x0d,0x2d,0x3a,0x0,	// sprite palette 2
  0x0d,0x27,0x2a	// sprite palette 3
};

// setup PPU and tables
void setup_graphics() {
  // clear sprites
  oam_hide_rest(0);
  // set palette colors
  pal_all(PALETTE);
  // turn on PPU
  ppu_on_all();
}

// main program
void main() {
  byte oam_id;	// sprite ID
  char pad;	// controller flags
  byte spriteX = 128;  // initial X position of the sprite
  
  // print instructions
  vram_adr(NTADR_A(2,2));
  vram_write("TEST3 by DEGAMISU.", 18);
  
  // setup graphics
  setup_graphics();
  
  // loop forever
  while (1) {
    // start with OAMid/sprite 0
    oam_id = 0;
    
    // poll controller 0
    pad = pad_poll(0);
    
    // move sprite left/right based on controller input
    if (pad & PAD_LEFT && spriteX > 0) {
      spriteX--;
    } else if (pad & PAD_RIGHT && spriteX < 240) {
      spriteX++;
    }
    
    // draw the sprite at the new position
    oam_id = oam_meta_spr(spriteX, 100, oam_id, mySprite);
    
    // hide rest of sprites
    if (oam_id != 0) {
      oam_hide_rest(oam_id);
    }
    
    // wait for next frame
    ppu_wait_frame();
  }
}
