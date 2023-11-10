#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include<string.h>
#include <SDL2/SDL.h>

//SDL Container object
typedef  struct {
 SDL_Window *window;
 SDL_Renderer * renderer ; 
 } sdl_t;

//EMULATOR configuration object
typedef struct {
    uint32_t window_width ; 
    uint32_t window_height; 
    uint32_t fg_color ; //Foreground color
    uint32_t bg_color  ; // Background color
    uint32_t scale_factor ; 
} config_t;

//Emulator state
typedef enum{
  QUIT , RUNNING, PAUSED,
}emulator_state_t ;

//CHIP8 machine object 
typedef struct {
  emulator_state_t state ;
}chip8_t ;

bool set_config_from_argcs(config_t * config , const int argc ,   char ** argv ) {
           config->window_height = 32;
           config->window_width= 64 ; 
           config->fg_color = 0xFFFFFFFF; // WHITE color
           config->bg_color = 0xFFFF00FF; // YELLOW color
           config->scale_factor = 20 ; 
           for( int i = 1 ; i <argc; i++ ) {
             (void)argv[i] ;  //prevent compiler error from unuse argv argc  
           }
           return true;
        }


bool init_sdl(sdl_t * sdl,const config_t config) {
  if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 ) {
    SDL_Log("Unable to initialize SDL %s\n ", SDL_GetError()) ; 
    return  false ; 
  }
  sdl->window = SDL_CreateWindow( "CHIP8_EMULATOR", SDL_WINDOWPOS_CENTERED,
  SDL_WINDOWPOS_CENTERED,config.window_width * config.scale_factor,config.window_height* config.scale_factor,0 ) ; 

  if( !sdl->window) {
       SDL_Log("Couldn't open SDL window %s\n ", SDL_GetError()) ; 
       return false;  
  }

  sdl->renderer = SDL_CreateRenderer( sdl->window, -1, SDL_RENDERER_ACCELERATED); 
   if( !sdl->renderer) {
       SDL_Log("Couldn't open SDL renderer %s\n ", SDL_GetError()) ; 
       return false;  
  } 
  return true ; 
}


void final_cleanup( const sdl_t  sdl) {
  SDL_DestroyRenderer(sdl.renderer) ; 
  SDL_DestroyWindow(sdl.window) ; 
  SDL_Quit() ; 
}
// Da main squeeze

void clear_screen( const config_t config , sdl_t sdl ) {
  const uint8_t r= (config.bg_color >> 24 ) & 0xFF ; 
  const uint8_t g= (config.bg_color >> 16 ) & 0xFF ;  
  const uint8_t b= (config.bg_color >> 8 ) & 0xFF ;  
  const uint8_t a= (config.bg_color >> 0 ) & 0xFF ;   
  SDL_SetRenderDrawColor( sdl.renderer,r,g,b,a ) ; 
  SDL_RenderClear(sdl.renderer) ; 
}

void update_screen( const sdl_t sdl ) {
  SDL_RenderPresent( sdl.renderer) ; 
}

void handle_input( chip8_t *  chip8) {
  SDL_Event event ; 
  while( SDL_PollEvent(&event) ) {
    switch(event.type) {
      case SDL_QUIT :
        chip8->state = QUIT ; 
        return ;

      case SDL_KEYDOWN:
        switch( event.key.keysym.sym ) {
          case SDLK_ESCAPE:
            chip8->state = QUIT ; 
            return ; 

          default:
            break ;   
        }
          break;

      case SDL_KEYUP :
          break ; 

      default : 
          break ;    
    }

  }
}


//Initialize chip8 machine 
bool init_chip8 ( chip8_t  * chip8) {
  chip8->state = RUNNING ; 
  return true ; 

}
int main(int argc, char **argv) {

  sdl_t sdl = {0} ; 
    
  //INI emulator configuration/option 
  config_t config= {0} ;
  if(!set_config_from_argcs(&config, argc,argv))
    exit(EXIT_FAILURE) ;

  if( !init_sdl(&sdl ,config )) 
    exit(EXIT_FAILURE) ; 
  
  // Initiale chip8 machine 
  chip8_t chip8 = {0} ; 
  if(!init_chip8(&chip8)) exit(EXIT_FAILURE) ; 
  // Initial screen clear
   clear_screen( config ,sdl ) ; 

  while(chip8.state != QUIT) {
    // Handel user input
    handle_input(&chip8) ; 
    //if(chip8.state == PAUSED ) continue ; 
  
    // Get_time() ; 
    //Emulate CHIP8 Instructions
    //Get_time()
    //Delay for approximately 60hz/60fps (16.67ms)
    SDL_Delay(16.67) ; 

    //Update window with changes
    update_screen(sdl) ; 
  }

  final_cleanup(sdl) ; 
  exit(EXIT_SUCCESS) ; 
}

