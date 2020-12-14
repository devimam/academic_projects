# include "iGraphics.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>

using namespace std;

#define RAD 12

/*
	function iDraw() is called again and again by the system.
*/
int tiles_grabbed;
int SPEEDX;
int SPEEDY;
int start;
int end;
int pause;
int restart;
int is_killed;
int newround;
int ballx;
int bally;
int up;
int rightt;

int barx;
int bary;

vector<pair<int,int> > v;

void initialize_screen(){
	start=0;
	end=0;
	is_killed=0;
	restart=0;
	pause=0;
	newround=0;

	SPEEDX=2;
	SPEEDY=2;

	barx=550;
	bary=11;

	ballx=600;
	bally=21+RAD;

	up=1;

	rightt=1;

	tiles_grabbed=0;

	v.clear();
	///creating the board
	for(int i=0;i<3;i++){
		for(int j=0;j<10;j++){
			int x=301+j*50;
			int y=475+i*25;

			pair<int,int> p;
			p.first=x;
			p.second=y;

			v.push_back(p);
		}
	}
}


void iDraw()
{
	//cout<<ballx<<" "<<bally<<" "<<barx<<" "<<bary<<endl;
	//place your drawing codes here
	iClear();

	newround+=3;
	newround=newround%10;

	iSetcolor(1,1,1);
	iLine(300,0,300,600);
	iLine(300,550,800,550);
	iLine(300,10,800,10);

	for(int i=0;i<10;i++){
		int x=325+i*50;
		iSetcolor(1,0,0);
		if(i==newround) iFilledCircle(x,575,25,100);
		else iCircle(x,575,25);
	}



	iSetcolor(0,0,1);
	iFilledRectangle(25,25,150,50);
	iFilledRectangle(25,100,150,50);
	iFilledRectangle(25,175,150,50);
	char* str="START";
	iSetcolor(1,0,0);
	iText(70,50,str);
	iText(70,125,"Pause");
	iText(70,200,"Restart");
	iText(70,300,"Score: ");
	int score=tiles_grabbed*5;
	stringstream ss;
	ss << score;
	string s = ss.str();
	char *cstr = new char[s.length() + 1];
	strcpy(cstr, s.c_str());
	iText(150,300,cstr);

	///drawing the upper tiles
	for(int itt=0;itt<v.size();itt++){
		pair<int,int> p;
		p=v[itt];

		int posx=p.first;
		int posy=p.second;

		//cout<<posx<<" "<<posy<<endl;

		if(posx!=-1 && posy!=-1){
			iSetcolor(0,1,0);
			iFilledRectangle(posx,posy,48,24);
		}
	}

	///drawing the bar

	iSetcolor(0,0,1);
	iFilledRectangle(barx,bary,100,10);

	//drawing the ball
	iSetcolor(0,0,1);
	iFilledCircle(ballx,bally,RAD,100);


	if(end==1 && restart==0){
		iSetcolor(1,0,0);
		iFilledRectangle(450,250,100,100);

		iSetcolor(0,0,0);
		iText(480,280,"The END");
	}
	if(is_killed==1 && restart==0){
		iSetcolor(1,0,0);
		iFilledRectangle(450,250,100,100);

		iSetcolor(0,0,0);
		iText(450,280,"Game Over");
	}
	//if(pause==1 && restart==0){
	//	start=1-pause;
	//}
	if(restart==1){
		initialize_screen();
	}


	if(start==1){
		///changing the ball position
		//if(end!=1 && is_killed!=1 && pause!=1){
			if(up==1 && rightt==1){
				ballx+=SPEEDX;
				bally+=SPEEDY;

				if(ballx>800) rightt=0;
				if(bally>550) up=0;

			}
			else if(up==0 && rightt==1){
				ballx+=SPEEDX;
				bally-=SPEEDY;

				if(ballx>800) rightt=0;
				if(bally<10) up=1;

			}
			else if(up==1 && rightt==0){
				ballx-=SPEEDX;
				bally+=SPEEDY;

				if(ballx<300) rightt=1;
				if(bally>550) up=0;
			}
			else if(up==0 && rightt==0){
				ballx-=SPEEDX;
				bally-=SPEEDY;

				if(ballx<300) rightt=1;
				if(bally<10) up=1;
			}

		//}

		///checking whether or not a tiles is touched or not

		for(int it=0;it<v.size();it++){
			pair<int,int> p;
			p=v[it];

			int posx=p.first;
			int posy=p.second;

			if((ballx>posx && ballx<posx+48) && (bally>posy && bally<posy+24)){
				up=1-up;

				p.first=-1;
				p.second=-1;
				v[it]=p;

				tiles_grabbed++;
			}
		}



		///checking whether or not touched the bar

		if(ballx>barx-10 && ballx<barx+10+100 && bally>bary-5 && bally<bary+5+10){
			up=1-up;

			int lower_half;
			if(ballx<(barx+40)) lower_half=1;
			else if(ballx>(barx+70)) lower_half=2;
			else lower_half=3;

			if(lower_half==1) SPEEDY=2.5;
			if(lower_half==2) SPEEDY=1.5;
			if(lower_half==3) SPEEDY=2;

		}
		else if((ballx<barx-10 || ballx>barx+10+100)&&(bally>bary-5 && bally<bary+5+10)){
			is_killed=1;
			start=0;
		}

		if(tiles_grabbed==30){
			end=1;
			start=0;
		}

	}

}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
	//place your codes here
	if(mx>300 && mx<700 && end!=1 && is_killed!=1 && pause!=1){
		barx=mx;
	}

}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//place your codes here

		if(mx>25 && mx<175 && my>25 && my<75){
			initialize_screen();
			start=1;
			restart=0;
		}
		else if(mx>25 && mx<175 && my>100 && my<150){
			pause=1-pause;
			if(pause==1) start=0;
			else start=1;
		}
		else if(mx>25 && mx<175 && my>175 && my<225){
			restart=1;
		}

	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//place your codes here
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
	if(key == 'q')
	{
		exit(0);
	}
	//place your codes for other keys here
}

int main()
{
	//place your own initialization codes here.
	initialize_screen();
	iInitialize(800, 600, "Dx Ball");
	return 0;
}
