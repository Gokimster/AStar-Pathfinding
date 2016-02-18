// AStarPathFinding.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"

#include "glut.h"
#include "glaux.h"
#include "Path.h"
#include <stdio.h>
#include <sstream>

using namespace std;

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"GLaux.lib")

int win_width=640;
int win_height=810;

int g_map_width=20;
int g_map_height=20;

int g_tile_wide=10;
int g_tile_high=10;

int g_XPos=0;
int g_YPos=0;

int tile_size=64;
int y_offset=650;
//holds the tiles of the map
GLuint map_tiles[20][20];
GLuint border,ground,tree,bush,stones;
GLuint start,goal,track;
GLuint LoadTexture(char *FileName ) 
{ 
	FILE *File=NULL;          
	GLuint ID;
	File=fopen(FileName,"r");     

	if (File)            
	{ 
		fclose(File);    
	} 

	AUX_RGBImageRec *TextureImage=new(AUX_RGBImageRec);       
	memset(TextureImage,0,sizeof(void *)*1);             
	TextureImage = auxDIBImageLoad(FileName); 

	glGenTextures(1, &ID);       
	glBindTexture(GL_TEXTURE_2D,ID); 

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data); 

	free(TextureImage->data);        
	free(TextureImage);   
	
	return ID;
}
void SetupMap()
{
		//clear all the matrix with the ground tile
	for(int i=0;i<g_map_height;i++)
	{
		for(int j=0;j<g_map_width;j++)
		{
			map_tiles[i][j]=ground;
		}
	}

	//sparse ground trees every where
	int rnd_i=rand()% 20;
	int rnd_j=rand()% 20;

	map_tiles[rnd_i][rnd_j]=tree;
	

	rnd_i=rand()% 20;
	rnd_j=rand()% 20;
	map_tiles[rnd_i][rnd_j]=bush;

	rnd_i=rand()% 20;
	rnd_j=rand()% 20;
	map_tiles[rnd_i][rnd_j]=stones;

	rnd_i=rand()% 20;
	rnd_j=rand()% 20;
	map_tiles[rnd_i][rnd_j]=tree;

	rnd_i=rand()% 20;
	rnd_j=rand()% 20;
	map_tiles[rnd_i][rnd_j]=bush;

	rnd_i=rand()% 20;
	rnd_j=rand()% 20;
	map_tiles[rnd_i][rnd_j]=stones;
}
void LoadAllSlices()
{
	
	//load all slides
	border=LoadTexture("border.bmp");
	ground=LoadTexture("map_slices\\ground.bmp");
	tree=LoadTexture("map_slices\\tree.bmp");
	bush=LoadTexture("map_slices\\bush.bmp");
	stones=LoadTexture("map_slices\\stones.bmp");


	start=LoadTexture("map_slices\\start.bmp");
	goal=LoadTexture("map_slices\\goal.bmp");
	track=LoadTexture("map_slices\\track.bmp");

	SetupMap();
}


void RenderBorder()
{
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	//top border
	glBindTexture(GL_TEXTURE_2D,  border);
	glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(0,0);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(win_width,0);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(win_width,20);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(0,20);
	glEnd();
	
	glBegin(GL_QUADS);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(0,y_offset-10);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(win_width,y_offset-10);
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(win_width,y_offset+10);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(0,y_offset+10);
	glEnd();
}

void RenderMapSlices()
{
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	for(int y=0;y<g_tile_high;y++)
	{
		for(int x=0;x<g_tile_wide;x++)
		{

			glBindTexture(GL_TEXTURE_2D,  map_tiles[x+g_XPos][y+g_YPos]);

			int pixel_x=x*tile_size;
			int pixel_y=y*tile_size;

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,1.0f);
				glVertex2f(pixel_x,pixel_y);
				glTexCoord2f(1.0f,1.0f);
				glVertex2f(pixel_x+tile_size,pixel_y);
				glTexCoord2f(1.0f,0.0f);
				glVertex2f(pixel_x+tile_size,pixel_y+tile_size);
				glTexCoord2f(0.0f,0.0f);
				glVertex2f(pixel_x,pixel_y+tile_size);
			glEnd();

		}

	}
}
void RenderMiniMap()
{
	
	int dot_size=8;

	for(int y=y_offset;y<y_offset+g_map_height;y++)
	{
		for(int x=0;x<g_map_width;x++)
		{
			int pixel_x=x*dot_size;
			int pixel_y= y_offset + (y-y_offset)*dot_size;
			glBindTexture(GL_TEXTURE_2D,  map_tiles[x][y-y_offset]);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,1.0f);	
				glVertex2f(pixel_x,pixel_y);
				glTexCoord2f(1.0f,1.0f);
				glVertex2f(pixel_x+dot_size,pixel_y);
				glTexCoord2f(1.0f,0.0f);
				glVertex2f(pixel_x+dot_size,pixel_y+dot_size);
				glTexCoord2f(0.0f,0.0f);
				glVertex2f(pixel_x,pixel_y+dot_size);
			glEnd();

					
		}
	}

	glColor3f(1.0f,1.0f,1.0f);
		//render the small rectangle to show your pos on the map
	int rect_pixel_x=g_XPos*dot_size;
	int rect_pixel_y=y_offset+g_YPos*dot_size;

	int rect_pixel_width=g_tile_wide*dot_size;
	int rect_pixel_height=g_tile_high*dot_size;
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	
	glBegin(GL_QUADS);
				
				glVertex2f(rect_pixel_x,rect_pixel_y);
				
				glVertex2f(rect_pixel_x+rect_pixel_width,rect_pixel_y);
				
				glVertex2f(rect_pixel_x+rect_pixel_width,rect_pixel_y+rect_pixel_height);
				
				glVertex2f(rect_pixel_x,rect_pixel_y+rect_pixel_height);
	glEnd();

}
void init()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glEnable(GL_TEXTURE_2D);

	LoadAllSlices();


}
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	RenderMapSlices();
	
	RenderMiniMap();

	RenderBorder();
	
	

	glutSwapBuffers();
}
void reshape(int w,int h) 
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void ClearTrack(bool clear_start)
{
	//clear the array from the start tile track and end tiles
	for(int i=0;i<g_map_height;i++)
	{
		for(int j=0;j<g_map_width;j++)
		{
			if(map_tiles[i][j]==start && clear_start)
				map_tiles[i][j]=ground;
			else if(map_tiles[i][j]==goal || map_tiles[i][j]==track)
				map_tiles[i][j]=ground;
		}
	}
}  
   
vector<NODE> pathL;
CPath pathCreator;
                                                                                                                                                                                                                                                      
void Mouse(int button, int state,int x,int y)
{
	int mouse_tile_XPos=x/tile_size+g_XPos;
	int mouse_tile_YPos=y/tile_size+g_YPos;

	switch (button) 
	{ 
		case GLUT_LEFT_BUTTON: 
			if (state == GLUT_DOWN) 
			{
				//left clicked
				ClearTrack(true);

				map_tiles[mouse_tile_XPos][mouse_tile_YPos]=start;
						
			}
			break;
		case GLUT_RIGHT_BUTTON: 
			if (state == GLUT_DOWN ) 
			{
				//right clicked
				ClearTrack(false);

				//set the goal position
				map_tiles[mouse_tile_XPos][mouse_tile_YPos]=goal;

				//create the path
				pathCreator.SetObstructionMatrix(map_tiles,ground,start,goal);
				pathCreator.Create();
				pathL=pathCreator.BackTrack();
				//set the path to the map
				int i; 
				for(i=0;i<pathL.size()-1;i++)
				{
					map_tiles[pathL[i].X][pathL[i].Y]=track;
				}
				map_tiles[pathL[i].X][pathL[i].Y]=start;

				//clean the path list
				pathL.erase(pathL.begin(),pathL.begin()+pathL.size());


			}
			break; 
		default: 
			break; 
	} 
}

void MouseMotion(int x,int y)
{

	int mouse_tile_XPos=x/tile_size;
	int mouse_tile_YPos=y/tile_size;

	if(mouse_tile_XPos == g_tile_wide-1)
	{
		g_XPos++;
		if(g_XPos>=g_map_width-g_tile_wide)
			g_XPos=g_map_width-g_tile_wide;
	}
	else if(mouse_tile_XPos ==0)
	{
		g_XPos--;
		if(g_XPos<0)
			g_XPos=0;
	}
	else if(mouse_tile_YPos == g_tile_high-1)
	{
		g_YPos++;
		if(g_YPos >= g_map_height-g_tile_high)
			g_YPos=g_map_height-g_tile_high;
	}
	else if(mouse_tile_YPos == 0)
	{
		g_YPos--;
		if(g_YPos <0)
			g_YPos=0;
	}
}

int time_frame=10000;
void timer_func(int n)          
{
		glutPostRedisplay();
        glutTimerFunc(time_frame, timer_func, 0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (win_width, win_height); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("Path Finding");

	init ();

	glutDisplayFunc(render); 
	glutIdleFunc(render);

	glutReshapeFunc(reshape);
    glutPassiveMotionFunc(MouseMotion);
	glutMouseFunc(Mouse);

  
	glutTimerFunc(time_frame, timer_func, 0);

	glutMainLoop();
	return 0;
}
