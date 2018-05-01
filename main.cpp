#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "LTexture.h"


 int SCREEN_WIDTH = 640;
 int SCREEN_HEIGHT = 480;

bool init();

bool loadMedia();

void close();

SDL_Texture* loadTexture( std::string path );

SDL_Window* gWindow = NULL;


SDL_Renderer* gRenderer = NULL;


SDL_Texture* gTexture = NULL;

SDL_Rect gSpriteClips[ 10 ];
LTexture gSpriteSheetTexture;

class playerController
{
    public:
        float posx;
        float posy;

};

class buildingProp
{
    public:
    float posx=150;
    float posy;

    int numberOfFloors=6;
    int widex=1;
    int widey=1;

};

buildingProp buildings[100];



playerController player;

bool init()
{

	bool success = true;


	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{

		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}


		gWindow = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{

			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );


				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void LTexture::render( float x, float y, SDL_Rect* clip, float scalex, float scaley )
{

    SDL_Rect renderQuad = { x, y, mWidth, mHeight };


    if( clip != NULL )
    {
        renderQuad.w = clip->w*scalex;
        renderQuad.h = clip->h*scaley;
    }


    SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

bool LTexture::loadFromFile( std::string path )
{

	free();


	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{

		//SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{

			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}


		SDL_FreeSurface( loadedSurface );
	}


	mTexture = newTexture;
	return mTexture != NULL;
}

const int WORLD_WIDTH=30;
const int WORLD_HEIGHT=30;

    int worldMap[WORLD_HEIGHT][WORLD_WIDTH]
    {
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
        {7,7,7,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
        {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
        {7,8,7,6,6,6,6,6,6,6,6,7,8,7,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
        {7,8,7,6,5,5,5,5,5,5,6,7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,6,7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,6,7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,6,7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,6,7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,6,6,6,6,6,6,6,7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,6,6,6,6,6,6,6,7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,6,6,6,7,7,7,6,6,6,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,6,7,7,7,7,7,7,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,6,7,7,7,7,7,7,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,6,7,7,7,7,7,7,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,6,8,8,8,8,8,8,8,6,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,6,6,6,6,6,6,6,6,6,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
        {7,8,7,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
    };


bool loadMedia()
{
	///Loading success flag
    bool success = true;





    if( !gSpriteSheetTexture.loadFromFile( "Images/Sprites.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {

        gSpriteClips[ 0 ].x = 0;
        gSpriteClips[ 0 ].y = 0;
        gSpriteClips[ 0 ].w = 8;
        gSpriteClips[ 0 ].h = 8;


        gSpriteClips[ 1 ].x = 24;
        gSpriteClips[ 1 ].y = 16;
        gSpriteClips[ 1 ].w = 8;
        gSpriteClips[ 1 ].h = 8;

        gSpriteClips[ 2 ].x = 32;
        gSpriteClips[ 2 ].y = 32;
        gSpriteClips[ 2 ].w = 8;
        gSpriteClips[ 2 ].h = 8;


        gSpriteClips[ 3 ].x = 16;
        gSpriteClips[ 3 ].y = 40;
        gSpriteClips[ 3 ].w = 8;
        gSpriteClips[ 3 ].h = 8;


        gSpriteClips[ 4 ].x = 24;
        gSpriteClips[ 4 ].y = 16;
        gSpriteClips[ 4 ].w = 2;
        gSpriteClips[ 4 ].h = 2;

        gSpriteClips[ 5 ].x = 16;
        gSpriteClips[ 5 ].y = 40;
        gSpriteClips[ 5 ].w = 8;
        gSpriteClips[ 5 ].h = 8;


        gSpriteClips[ 6 ].x = 8;
        gSpriteClips[ 6 ].y = 40;
        gSpriteClips[ 6 ].w = 8;
        gSpriteClips[ 6 ].h = 8;


        gSpriteClips[ 7 ].x = 0;
        gSpriteClips[ 7 ].y = 32;
        gSpriteClips[ 7 ].w = 8;
        gSpriteClips[ 7 ].h = 8;


        gSpriteClips[ 8 ].x = 0;
        gSpriteClips[ 8 ].y = 40;
        gSpriteClips[ 8 ].w = 8;
        gSpriteClips[ 8 ].h = 8;

        gSpriteClips[ 9 ].x = 8;
        gSpriteClips[ 9 ].y = 32;
        gSpriteClips[ 9 ].w = 8;
        gSpriteClips[ 9 ].h = 8;


    }


	return success;
}

void close()
{

	SDL_DestroyTexture( gTexture );
	gTexture = NULL;


	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;


	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{

	SDL_Texture* newTexture = NULL;


	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{

        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}


		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{

		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{

			bool quit = false;


			SDL_Event e;

            buildings[0].posy=-5*8;
            buildings[0].posx=0;
            buildings[0].widex=30;
            buildings[0].widey=5;
            buildings[0].numberOfFloors=25;

            buildings[1].posy=-4*27;
            buildings[1].posx=8*8;
            buildings[1].widex=8;
            buildings[1].widey=30;
            buildings[1].numberOfFloors=25;

            buildings[2].posy=8*6;
            buildings[2].posx=-8*5;
            buildings[2].widex=4;
            buildings[2].widey=4;
            buildings[2].numberOfFloors=7;

            buildings[3].posy=8*12;
            buildings[3].posx=-8*5;
            buildings[3].widex=3;
            buildings[3].widey=4;
            buildings[3].numberOfFloors=7;


            buildings[4].posy=8*12;
            buildings[4].posx=-8*17;
            buildings[4].widex=4;
            buildings[4].widey=4;
            buildings[4].numberOfFloors=9;



            SDL_RenderSetScale( gRenderer, 4, 4);
            SCREEN_HEIGHT/=4;
            SCREEN_WIDTH/=4;


			while( !quit )
			{

				while( SDL_PollEvent( &e ) != 0 )
				{
				    //std::cout<<"b";

					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
					    std::cout<<player.posx<<" "<<player.posy<<"\n";

						switch( e.key.keysym.sym )
						{
							case SDLK_UP:
							player.posy-=1,5;
							break;

							case SDLK_DOWN:
							player.posy+=1,5;
							break;

							case SDLK_LEFT:
							player.posx-=1,5;
							break;

							case SDLK_RIGHT:
							player.posx+=1,5;
							break;

							default:
							std::cout<<"l";
							break;
						}
					}
				}


				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
                //player.posy=(int)(player.posy)-(int)(player.posy)%10;



                /*for(int i=-20;i<=20;i++)
                    for(int j=-20;j<20;j++)
                {
                {
                    gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx+8*i, SCREEN_HEIGHT/2-player.posy+8*j, &gSpriteClips[ 3 ],1,1 );
                }*/



                //std::cout<<player.posx<<"\n";

                for(int i=0;i<WORLD_HEIGHT;i++)
                {
                    for(int j=0;j<WORLD_WIDTH;j++)
                    {
                        gSpriteSheetTexture.render(SCREEN_WIDTH/2-player.posx+j*8,SCREEN_HEIGHT/2-player.posy+i*8,&gSpriteClips[ worldMap[i][j] ],1,1);
                    }
                }

                gSpriteSheetTexture.render( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, &gSpriteClips[ 0 ],1,1 );

				for(int i=0;i<8;i++)
                {
                   gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx, SCREEN_HEIGHT/2-player.posy+buildings[i].posy, &gSpriteClips[ 2 ],buildings[i].widex ,buildings[i].widey);
                   float playerCY=player.posy+4;//center
                   float buildingCY=buildings[i].posy+buildings[i].widey*4;

                   if(playerCY>buildingCY)
                        {
                            //std::cout<<player.posy+4<<">"<<buildings[i].posy+4<<"\n";
                            float distanceHeight=(playerCY-buildingCY)/10*buildings[i].numberOfFloors;
                            //std::cout<<player.posy<<"\n";
                            //std::cout<<-(player.posy-buildings[i].posy-player.posy-buildings[i].posy)/10*buildings[i].numberOfFloors<<"\n";

                            float sizeOfWindows=distanceHeight/buildings[i].numberOfFloors;
                            gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+buildings[i].widey*8-buildings[i].numberOfFloors*sizeOfWindows, &gSpriteClips[ 4 ],buildings[i].widex*4,buildings[i].numberOfFloors*sizeOfWindows/2 );
                            gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx, SCREEN_HEIGHT/2-player.posy+buildings[i].posy- distanceHeight, &gSpriteClips[ 2 ],buildings[i].widex,buildings[i].widey );
                            //std::cout<<1/(player.posy-buildings[i].posy)*10; //<------------ old value for y scale

                            //std::cout<<sizeOfWindows<<"\n";

                            //std::cout<<  (player.posy-buildings[i].posy-buildings[i].posy +8) /buildings[i].numberOfFloors<<"\n";
                            for(int k=0;k<buildings[i].widex;k++)
                            for(int j=0;j<buildings[i].numberOfFloors;j++)

                            {
                                gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx+8*k, SCREEN_HEIGHT/2-player.posy+buildings[i].posy-sizeOfWindows-j*(sizeOfWindows)+8*buildings[i].widey, &gSpriteClips[ 1 ],1,sizeOfWindows/8 );
                                //std::cout<<j<<": "<<SCREEN_HEIGHT/2-player.posy-buildings[i].posy-j*sizeOfWindows<<"\n";
                                //std::cout<<j<<": "<<sizeOfWindows<<"\n";
                            }
                        }
                   else if(playerCY<buildingCY)

                        {

                        float distanceHeight=-(playerCY-buildingCY)/10*buildings[i].numberOfFloors;
                        float sizeOfWindows=distanceHeight/buildings[i].numberOfFloors;
                        gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+ distanceHeight-sizeOfWindows, &gSpriteClips[ 2 ],buildings[i].widex,buildings[i].widey );
                        gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx, SCREEN_HEIGHT/2-player.posy+buildings[i].posy, &gSpriteClips[ 4 ],buildings[i].widex*4,buildings[i].numberOfFloors*sizeOfWindows/2 );

                        for(int k=0;k<buildings[i].widex;k++)
                        for(int j=0;j<buildings[i].numberOfFloors;j++)
                            {
                                gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx+8*k, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+(j)*(sizeOfWindows), &gSpriteClips[ 1 ],1,sizeOfWindows/8 );
                                //std::cout<<j<<": "<<SCREEN_HEIGHT/2-player.posy-buildings[i].posy-j*sizeOfWindows<<"\n";
                                //std::cout<<j<<": "<<sizeOfWindows<<"\n";
                            }

                        }
                }


				SDL_RenderPresent( gRenderer );
			}
		}
	}


	close();

	return 0;
}
