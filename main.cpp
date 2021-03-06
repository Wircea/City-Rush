#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include "LTexture.h"
#include "LTimer.h"


 int SCREEN_WIDTH = 800;
 int SCREEN_HEIGHT = 600;

bool init();

bool loadMedia();

void close();

SDL_Texture* loadTexture( std::string path );

SDL_Window* gWindow = NULL;


SDL_Renderer* gRenderer = NULL;


SDL_Texture* gTexture = NULL;

SDL_Rect gSpriteClips[ 64 ];
LTexture gSpriteSheetTexture;


bool pressedKeys[4];


LTimer stepTimer;

class playerController
{
    public:
        float posx;
        float posy;

        double rotation=180;
        float velocity=0;
        float rotationvelocity=0;
        void move(double deltaTime);
        void rotate(double deltaTime);

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

const float CAP_LIMIT_CAR=0.05;
const float CAP_LIMIT_CAR_ROTATION=0.2;

void playerController::move(double deltaTime)
{
if(velocity>CAP_LIMIT_CAR)
    velocity=CAP_LIMIT_CAR;
if(velocity<-CAP_LIMIT_CAR)
    velocity=-CAP_LIMIT_CAR;

    posx = posx - float(velocity*cos((double)rotation*0.0174532925));
    posy = posy - float(velocity*sin((double)rotation*0.0174532925));



    if(velocity>0)
    velocity-=0.5*deltaTime;
    if(velocity<0)
    velocity+=0.5*deltaTime;

    if(velocity<0.0005&&velocity>0)
        velocity=0;
    if(velocity>-0.0005&&velocity<0)
        velocity=0;
    //std::cout<<velocity<<"\n";
}

void playerController::rotate(double deltaTime)
{

if(rotationvelocity>CAP_LIMIT_CAR_ROTATION)
    rotationvelocity=CAP_LIMIT_CAR_ROTATION;
if(rotationvelocity<-CAP_LIMIT_CAR_ROTATION)
    rotationvelocity=-CAP_LIMIT_CAR_ROTATION;

    if(velocity>0)
    rotation+= rotationvelocity;

if(!pressedKeys[2]&&!pressedKeys[3])
rotationvelocity=0;
if(rotationvelocity>0)
    rotationvelocity-=0.9*deltaTime;
    if(rotationvelocity<0)
    rotationvelocity+=0.9*deltaTime;

if(velocity==0)
    {
    //rotationvelocity=0;
    }
}

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

void LTexture::render( float x, float y, SDL_Rect* clip, float scalex, float scaley, double angle,SDL_Point* center, SDL_RendererFlip flip  )
{

    SDL_Rect renderQuad = { x, y, mWidth, mHeight };


    if( clip != NULL )
    {
        renderQuad.w = clip->w*scalex;
        renderQuad.h = clip->h*scaley;
    }


    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
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

        gSpriteClips[ 10 ].x = 32;
        gSpriteClips[ 10 ].y = 0;
        gSpriteClips[ 10 ].w = 16;
        gSpriteClips[ 10 ].h = 8;


        gSpriteClips[ 11 ].x = 16;
        gSpriteClips[ 11 ].y = 16;
        gSpriteClips[ 11 ].w = 8;
        gSpriteClips[ 11 ].h = 8;


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



Uint8 *SDL_GetKeyState(int *numkeys);

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

            /*buildings[0].posy=-5*8;
            buildings[0].posx=0;
            buildings[0].widex=30;
            buildings[0].widey=5;
            buildings[0].numberOfFloors=25;

            buildings[1].posy=-4*27;
            buildings[1].posx=8*8;
            buildings[1].widex=8;
            buildings[1].widey=30;
            buildings[1].numberOfFloors=25;*/

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

            /*buildings[5].posy=8*20;
            buildings[5].posx=-8*17;
            buildings[5].widex=7;
            buildings[5].widey=8;
            buildings[5].numberOfFloors=4;*/




            SDL_RenderSetScale( gRenderer, 4, 4);
            SCREEN_HEIGHT/=4;
            SCREEN_WIDTH/=4; //scale


			while( !quit )
			{
                double timeStep = stepTimer.getTicks() / 1000.f;
				while( SDL_PollEvent( &e ) != 0 )
				{
				    //std::cout<<"b";

					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN )
					{
					    //std::cout<<player.posx<<" "<<player.posy<<"\n";

                    //Uint8 *keystate = SDL_GetKeyState(NULL);


						switch( e.key.keysym.sym )
						{
							case SDLK_UP:

                            pressedKeys[0]=true;

							break;

							case SDLK_DOWN:

							pressedKeys[1]=true;

							break;

							case SDLK_LEFT:

							pressedKeys[2]=true;

							break;

							case SDLK_RIGHT:

							pressedKeys[3]=true;
							break;

							default:
							std::cout<<"l";
							break;
						}
					}
					else if( e.type == SDL_KEYUP )
                    {
                        switch( e.key.keysym.sym )
						{
							case SDLK_UP:

                            pressedKeys[0]=false;

							break;

							case SDLK_DOWN:

							pressedKeys[1]=false;

							break;

							case SDLK_LEFT:


							pressedKeys[2]=false;

							break;

							case SDLK_RIGHT:

							pressedKeys[3]=false;
							break;

							default:
							std::cout<<"l";
							break;
						}
                    }
				}

                if(pressedKeys[0]&&pressedKeys[2])
                    {
                    player.velocity+=1.1*timeStep;
                    player.rotationvelocity-=10000*timeStep;
                    player.move(timeStep);
                    player.rotate(timeStep);
                    }
                 if(pressedKeys[0]&&pressedKeys[3])
                    {
                    player.velocity+=1.1*timeStep;
                    player.rotationvelocity+=10000*timeStep;
                    player.move(timeStep);
                    player.rotate(timeStep);
                    }
                if(pressedKeys[0]&&!pressedKeys[2]&&!pressedKeys[3])
                    {
                    player.velocity+=1.5*timeStep;
                    player.move(timeStep);
                    }

                if(pressedKeys[1]&&pressedKeys[2])
                    {
                    player.velocity-=100*timeStep;
                    player.rotationvelocity-=10000*timeStep;
                    player.move(timeStep);
                    player.rotate(timeStep);
                    }
                 if(pressedKeys[1]&&pressedKeys[3])
                    {
                    player.velocity-=100*timeStep;
                    player.rotationvelocity+=10000*timeStep;
                    player.move(timeStep);
                    player.rotate(timeStep);
                    }
                if(pressedKeys[1]&&!pressedKeys[2]&&!pressedKeys[3])
                    {
                        player.velocity-=100*timeStep;
                        player.move(timeStep);
                    }

                    player.move(timeStep);
                    player.rotate(timeStep);

                std::cout<<pressedKeys[0]<<" "<<pressedKeys[1]<<" "<<pressedKeys[2]<<" "<<pressedKeys[3]<<"\n";

				stepTimer.start();


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

                gSpriteSheetTexture.render( SCREEN_WIDTH/2-8, SCREEN_HEIGHT/2-4, &gSpriteClips[ 10 ],1,1 ,player.rotation);

				for(int i=0;i<8;i++)
                {
                   gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx, SCREEN_HEIGHT/2-player.posy+buildings[i].posy, &gSpriteClips[ 2 ],buildings[i].widex ,buildings[i].widey);
                   float playerCY=player.posy;//center
                   float playerCX=player.posx;

                   float buildingCY=buildings[i].posy+buildings[i].widey*4;
                   float buildingCX=buildings[i].posx-buildings[i].widex*2;


                   //std::cout<<playerCX<<"\n";

                   if(playerCY>buildingCY)
                        {
                            //std::cout<<player.posy+4<<">"<<buildings[i].posy+4<<"\n";
                            float distanceHeight=(playerCY-buildingCY)/10*buildings[i].numberOfFloors;
                            float distanceWidth=(playerCX+buildingCX)/10*buildings[i].numberOfFloors;
                            //std::cout<<player.posy<<"\n";
                            //std::cout<<-(player.posy-buildings[i].posy-player.posy-buildings[i].posy)/10*buildings[i].numberOfFloors<<"\n";

                            float sizeOfWindows=distanceHeight/buildings[i].numberOfFloors;
                            float offsetOfWindows=distanceWidth/buildings[i].numberOfFloors;

                            //gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+buildings[i].widey*8-buildings[i].numberOfFloors*sizeOfWindows, &gSpriteClips[ 4 ],buildings[i].widex*4,buildings[i].numberOfFloors*sizeOfWindows/2 ); //old handler for building gaps (vertical only)
                            gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx - distanceWidth, SCREEN_HEIGHT/2-player.posy+buildings[i].posy- distanceHeight, &gSpriteClips[ 2 ],buildings[i].widex,buildings[i].widey );
                            //std::cout<<1/(player.posy-buildings[i].posy)*10; //<------------ old value for y scale

                            //std::cout<<sizeOfWindows<<"\n";

                            //std::cout<<  (player.posy-buildings[i].posy-buildings[i].posy +8) /buildings[i].numberOfFloors<<"\n";

                            if(playerCX>buildingCX)
                            {
                            for(int k=0;k<buildings[i].widey;k++)
                                for(int j=0;j<buildings[i].numberOfFloors;j++)
                                {
                                    gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx-j*(offsetOfWindows), SCREEN_HEIGHT/2-player.posy+buildings[i].posy+buildings[i].widey*4+8-sizeOfWindows-k*8-j*sizeOfWindows, &gSpriteClips[ 1 ],-floor(offsetOfWindows)/8,1 ,0);
                                }
                            }

                            if(playerCX>buildingCX&& playerCX>0)
                            {

                            for(int k=0;k<buildings[i].widey;k++)
                                for(int j=0;j<buildings[i].numberOfFloors;j++)
                                {
                                    gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx-(j+1)*(offsetOfWindows)+buildings[i].widex*8-1, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+buildings[i].widey*4+8-sizeOfWindows-k*8-j*sizeOfWindows, &gSpriteClips[ 1 ],floor(offsetOfWindows+1)/8,1 ,0);
                                }
                            }

                            for(int k=0;k<buildings[i].widex;k++)
                                for(int j=0;j<buildings[i].numberOfFloors;j++)
                                {
                                    gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx+8*k-offsetOfWindows*j, SCREEN_HEIGHT/2-player.posy+buildings[i].posy-(j+1)*(sizeOfWindows)+8*buildings[i].widey, &gSpriteClips[ 11 ],1,floor(sizeOfWindows+1)/8 );
                                }


                        }
                   else if(playerCY<buildingCY)

                        {

                        float distanceHeight=-(playerCY-buildingCY)/10*buildings[i].numberOfFloors;
                        float distanceWidth=(playerCX+buildingCX)/10*buildings[i].numberOfFloors;

                        float sizeOfWindows=distanceHeight/buildings[i].numberOfFloors;
                        float offsetOfWindows=distanceWidth/buildings[i].numberOfFloors;
                        gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx- distanceWidth, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+ distanceHeight-sizeOfWindows, &gSpriteClips[ 2 ],buildings[i].widex,buildings[i].widey );
                        //gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx, SCREEN_HEIGHT/2-player.posy+buildings[i].posy, &gSpriteClips[ 4 ],buildings[i].widex*4,buildings[i].numberOfFloors*sizeOfWindows/2 );

                         if(playerCX>buildingCX)
                            {
                            for(int k=0;k<buildings[i].widey;k++)
                                for(int j=0;j<buildings[i].numberOfFloors;j++)
                                {
                                    gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx+(j+1)*(offsetOfWindows)- distanceWidth, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+k*8+(buildings[i].numberOfFloors- j)*sizeOfWindows, &gSpriteClips[ 1 ],-floor(offsetOfWindows)/8,1 ,0);
                                }
                            }

                            if(playerCX>buildingCX&& playerCX>0)
                            {

                            for(int k=0;k<buildings[i].widey;k++)
                                for(int j=0;j<buildings[i].numberOfFloors;j++)
                                {
                                    gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx-(j+1)*(offsetOfWindows)+buildings[i].widex*8-1, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+buildings[i].widey*4+8-sizeOfWindows-k*8+(j+1)*sizeOfWindows, &gSpriteClips[ 1 ],floor(offsetOfWindows+1)/8,1 ,0);
                                }
                            }

                        for(int k=0;k<buildings[i].widex;k++)
                        for(int j=0;j<buildings[i].numberOfFloors;j++)
                            {
                                gSpriteSheetTexture.render( SCREEN_WIDTH/2-player.posx-buildings[i].posx+8*k-offsetOfWindows*j, SCREEN_HEIGHT/2-player.posy+buildings[i].posy+(j)*(sizeOfWindows), &gSpriteClips[ 11 ],1,floor(sizeOfWindows+1)/8,180 );
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
