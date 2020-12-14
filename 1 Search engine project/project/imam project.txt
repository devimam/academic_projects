# include "iGraphics.h"
# include "math.h"
/* 
	function iDraw() is called again and again by the system.
*/

int flag=1,i,j,k,l,m,n,o,p,q,count,pos=575,cx,cy,flagup,slope,dx,dy,press=0;
char ch;

void iDraw()
{
	iClear();
	iSetcolor(0,0,1);
	iFilledRectangle(0,0,1224,700);
	iSetcolor(0,1,0);
	for(i=595;i<=700;i+=21){
		for(j=0;j<=1200;j+=51){
			iFilledRectangle(j,i,50,20);
		}
	}
	if(flag==1){
		iSetcolor(1,1,0);
		iFilledRectangle(575,10,100,15);
		iSetcolor(1,0,0);
		iFilledCircle(625,35,10,100);
		cx=612;
		cy=35;
		dx=3;
		dy=3;
		if(press==1)
			flag=5;
		else if(press==2)
			flag=2;
	}
	else if(flag==2){
		iSetcolor(1,1,0);
		iFilledRectangle(pos,10,100,15);
		if(cx+10<=1224 && cy+10<=595){
			iSetcolor(1,0,0);
			iFilledCircle(cx+=dx,cy+=dy,10,100);
			flag=2;
		}		
		else if(cx+10>1224) flag=5;
		else if(cy+10>595) flag=3;
	}
	else if(flag==3){
		iSetcolor(1,1,0);
		iFilledRectangle(pos,10,100,15);
		if(cx+10<=1224 && cy-10>=35){
			iSetcolor(1,0,0);
			iFilledCircle(cx+=dx,cy-=dy,10,100);
			flag=3;
		}
		else if(cx+10>1224) flag=4;
		else if(cx+10>=pos+50 && cx+10<=pos+100){
			flag=2;
			if(cx-10-pos-50==0){
				dx=0;
				dy=3;
			}
			else if(cx+10-pos-50<=10){
				dx=1;
				dy=3;
			}
			else if(cx+10-pos-50>=40){
				dx=3;
				dy=1;
			}
			else{
				dx=3;
				dy=3;
			}
		}
		else if(cx+10>=pos && cx+10<pos+50){
			flag=5;
			if(cx+10-pos<=10){
				dx=3;
				dy=1;
			}
			else if(cx+10-pos==50){
				dx=0;
				dy=3;
			}
			else if(cx+10-pos>=40){
				dx=1;
				dy=3;
			}
			else{
				dx=3;
				dy=3;
			}
		}
		else{
			flag=1;
			press=0;
		}
	}
	
	else if(flag==4){
		iSetcolor(1,1,0);
		iFilledRectangle(pos,10,100,15);
		if(cx-10>=0 && cy-10>=35){
			iSetcolor(1,0,0);
			iFilledCircle(cx-=dx,cy-=dy,10,100);
			flag=4;
		}

		else if(cx-10<0) flag=3;
		else if(cx-10>pos && cx-10<=pos+50){
			flag=5;
			if(cx-10-pos<=10){
				dx=3;
				dy=1;
			}
			else if(cx-10-pos==50){
				dx=0;
				dy=3;
			}
			else if(cx-10-pos>=40){
				dx=1;
				dy=3;
			}
			else{
				dx=3;
				dy=3;
			}
		}
		else if(cx-10>pos+50 && cx-10<pos+100){
			flag=2;
			if(cx-10-pos-50==0){
				dx=0;
				dy=3;
			}
			else if(cx-10-pos-50<=10){
				dx=1;
				dy=3;
			}
			else if(cx-10-pos-50>=40){
				dx=3;
				dy=1;
			}
			else{
				dx=3;
				dy=3;
			}
		}
		else{
			flag=1;
			press=0;
		}
	}

	else if(flag==5){
		iSetcolor(1,1,0);
		iFilledRectangle(pos,10,100,15);
		if(cx-10>=0 && cy+10<=595){
			iSetcolor(1,0,0);
			iFilledCircle(cx-=dx,cy+=dy,10,100);
			flag=5;
		}

		else if(cx-10<0) flag=2;
		else if(cy+10>595) flag=4;
	}

	//place your drawing codes here	
}

/* 
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
	if(mx+100>1224)
		pos=1150;
	else if(mx<0)
		pos=0;
	else pos=mx;
	//place your codes here
}

/* 
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(mx>625)
			press=1;
		else if(mx<=625)
			press=2;
		//place your codes here	
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
	iInitialize(1224, 700, "Dx Ball");
	return 0;
}	