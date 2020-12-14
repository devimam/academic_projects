# include "iGraphics.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int start=0,click=0,check=0,isall=1;

int flag=0,count=0,i,j,k,l,press=0,cx=625,cy=35,dx=8,dy=8,pos,istop,isleft,isright,lifecount=1,imam=0;

int arr[250][3];

/*
	function iDraw() is called again and again by the system.
*/
void iDraw()
{
	//place your drawing codes here
	char temp1[5]="";
	char temp2[5]="";
	char slife[10]="LIFE : ";
	char sscore[30]="SCORE : ";
	if(start==0){
		iSetcolor(0,0,1);
		iFilledRectangle(0,0,1224,700);
		iShowBMP(250,100,"opening page.bmp");
	}
	else if(start==1){
		exit(0);
	}
	else if(start==2){
		iSetcolor(0,0,1);
		iFilledRectangle(0,0,1224,700);
		iShowBMP(300,100,"instruction.bmp");
	}
	else if(start==3){
		iClear();
		istop=0;
		iShowBMP(100,0,"starting image.bmp");
		itoa(4-lifecount,temp1,10);
		strcat(slife,temp1);
		iSetcolor(0,0,1);
		itoa(imam,temp2,10);
		strcat(sscore,temp2);
		iText(480,250,sscore,GLUT_BITMAP_HELVETICA_18);
		iText(620,250,slife,GLUT_BITMAP_HELVETICA_18);
		start=3;
		if(flag==0){
			iSetcolor(0,1,0);
			for(i=595;i<=680;i+=21){
				for(j=0;j<=1180;j+=51){
					iFilledRectangle(j,i,50,20);
					arr[count][0]=j;
					arr[count][1]=i;
					arr[count][2]=0;
					count++;
					//printf("%d %d %d\n",arr[count-1][0],arr[count-1][1],arr[count-1][2]);
				}
			}
			iSetcolor(1,1,0);
			iFilledRectangle(575,10,100,15);
			iSetcolor(1,0,0);
			iFilledCircle(625,35,10,100);
			if(press==1) flag=5;
			else if(press==2) flag=2;
			else flag=0;
			cx=625;
			cy=35;
			dx=6;dy=6;
			check=1;
		}

		else {
			isall=1;
			istop=0;
			//isleft=0;
			//isright=0;

			//iSetcolor(1,1,0);
			//iFilledRectangle(pos,10,100,15);
			for(l=0;l<count;l++){
					if(arr[l][2]==0){
						isall=0;
						break;
					}
				}
			if(isall==1) {
				iShowBMP(400,450,"congratulation.bmp");
				iSetcolor(0,0,1);
				iFilledRectangle(475,220,230,20);
				iSetcolor(1,0,0);
				iText(500,225,"Press Space Bar",GLUT_BITMAP_HELVETICA_18);
				
			}
			else{
				if(cy>=595){
					//printf("YAAAAAAAAA ");
					for(k=0;k<count;k++){
						if(cx>=arr[k][0] && cx<=arr[k][0]+50 && cy>=arr[k][1] && cy<=arr[k][1]+20 && arr[k][2]==0){
							arr[k][2]=1;
							istop=1;
							imam=imam+5;
							break;
						}
					}
					//printf(" %d %d\n",istop,isright);
					//printf("%d %d\n",arr[k][0],arr[k][1]);
				}

				iSetcolor(0,1,0);
				for(l=0;l<count;l++){
					if(arr[l][2]==0){
						iFilledRectangle(arr[l][0],arr[l][1],50,20);
					}
				}

				if(flag==-1){
					iSetcolor(1,1,0);
					iFilledRectangle(pos,10,100,15);
					iSetcolor(1,0,0);
					iFilledCircle(625,35,10,100);
					if(press==1) flag=5;
					else if(press==2) flag=2;
					cx=625;
					cy=35;
					dx=8;
					dy=8;
					check=1;
				}

				else if(flag==2){
					iSetcolor(1,1,0);
					iFilledRectangle(pos,10,100,15);
					if(istop==0 && cx+10>1224) flag=5;
					else if(istop==1 && cx+10>=1224) flag=4;
					else if(istop==1 || cy+10>=700) flag=3;
					//else if(isright==1) flag=5;
					//else if(istop==1 && isright==1) flag=4;
					else if(cy+10>=700) flag=3;
					else{
						iSetcolor(1,0,0);
						iFilledCircle(cx+=dx,cy+=dy,10,200);
						flag=2;
					}
				}

				else if(flag==5){
					iSetcolor(1,1,0);
					iFilledRectangle(pos,10,100,15);
					if(istop==0 && cx-10<0) flag=2;
					else if(istop==1 && cx-10<0) flag=3;
					else if(istop==1 || cy+10>=700) flag=4;
					else if(isleft==1) flag=2;
					else{
						iSetcolor(1,0,0);
						iFilledCircle(cx-=dx,cy+=dy,10,200);
						flag=5;
					}
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
						if(cx-pos-50==0){
							dx=0;
							dy=8;
						}
						else if(cx-pos-50<=10){
							dx=3;
							dy=8;
						}
						else if(cx-pos-50<=40){
							dx=8;
							dy=8;
						}
						else{
							dx=8;
							dy=4;
						}
					}
					else if(cx+10>=pos && cx+10<pos+50){
						flag=5;
						if(cx+10-pos<=10){
							dx=8;
							dy=3;
						}
						else if(cx+10-pos==50){
							dx=0;
							dy=8;
						}
						else if(cx+10-pos>=40){
							dx=4;
							dy=8;
						}
						else{
							dx=8;
							dy=8;
						}
					}
					else if(cy>595 && istop==1) flag=4;
					else{
						if(lifecount++<=3){
							flag=-1;
							press=0;
							pos=575;
						}
						else{
							flag=0;
							press=0;
							imam=0;
							lifecount=1;
						}
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
							dx=8;
							dy=3;
						}
						else if(cx-10-pos==50){
							dx=0;
							dy=8;
						}
						else if(cx-10-pos>=40){
							dx=4;
							dy=8;
						}
						else{
							dx=8;
							dy=8;
						}
					}
					else if(cx-10>pos+50 && cx-10<pos+100){
						flag=2;
						if(cx-10-pos-50==0){
							dx=0;
							dy=8;
						}
						else if(cx-10-pos-50<=10){
							dx=3;
							dy=8;
						}
						else if(cx-10-pos-50>=40){
							dx=8;
							dy=4;
						}
						else{
							dx=8;
							dy=8;
						}
					}
					else if(cy>595 && istop==1) flag=3;
					else{
						if(lifecount++<=3){
							flag=-1;
							press=0;
								pos=575;
						}
						else{
							flag=0;
							press=0;
							imam=0;
							lifecount=1;
						}
					}
				}
			}
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
	if(mx+100>1224) pos=1124;
	else if(mx<0) pos=0;
	else pos=mx;
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
		//printf("%d %d\n",mx,my);
		//if(flag==0 || flag==-1){
		//	if(mx>625) press=1;
		//	else press=2;
		//	start=3;
		//}
		if(start==0){
			if((mx>433 && mx<869)&&(my>195 && my<258)) start=1;
			else if((mx>439 && mx<859)&&(my>274 && my<332)) start=2;
			else if((mx>446 && mx<848)&&(my>348 && my<400)) start=3;
			else start=0;
		}
		else if(start==1) start=1;

		if((flag==0 || flag==-1)&&check==1){
			if(mx>625) press=1;
			else press=2;
			start=3;
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
	if(key == (char) 27) start=1;
	else if(key=='i' && start!=1) start=2;
	else if(key=='n' && start!=1) {start=3;flag=0;check=0;press=0;cx=625;cy=35;dx=8;dy=8;count=0;click=0;lifecount=1;}
	else if(key=='p' && start!=1 && start!=4) start=4;
	else if(key=='p' && start!=1 && start==4) start=3;
	else if(key==(char) 32 && start!=1) start=0;
	//place your codes for other keys here
}

int main()
{
	//place your own initialization codes here.
	iInitialize(1224, 700, "DX BALL");
	return 0;
}