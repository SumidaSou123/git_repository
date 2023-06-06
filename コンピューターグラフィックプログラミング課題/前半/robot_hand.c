/* List: p3-robot.c
 * Robot arm with two links and two joints.
 * Examination of world-local coordinates, modeling transfomation and 
 * operation of matrix stack.
 */
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
int 	timenow=0,samplingTime = 200;
time_t start_time;
int rps_type;
static int shoulder = 0, elbow = 0,hand=0, fin_4 = 0, fin_5 = 0, fin_6 = 0,fin_7 = 0, fin_8 = 0, fin_9 = 0 
, fin_10 = 0, fin_11 = 0 , fin_12 = 0 , fin_13 = 0, fin_14 = 0 , fin_15 = 0 , fin_16 = 0, fin_17 = 0,fin_4_y=0,fin_7_y=0
, fin_10_y = 0, fin_13_y = 0;


void myInit(char *progname) 
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1600, 900); 
	glutInitWindowPosition(0, 0);
	glutCreateWindow(progname);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
/* 1st link 肩*/
	glRotated((double)shoulder, 0.0, 0.0, 1.0);
	glTranslated(0.3, 0.0, 0.0);
	glPushMatrix();
	glScaled(2.0, 0.4, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
/* 2nd link 肘*/
	glTranslated(1.0, 0.0, 0.0);				//move to the end of 1st link
	glRotated((double)elbow, 0.0, 0.0, 1.0);
	glTranslated(1.0, 0.0, 0.0);
	glPushMatrix();
	glScaled(2.0, 0.4, 1.0);
	glutWireCube(1.0);
	glPopMatrix();
/* 3rd link 手*/
	glTranslated(1.0, 0.0, 0.0);				
	glRotated((double)hand, 0.0, 0.0, 1.0);
	glTranslated(0.5, 0.0, 0.0);
	glPushMatrix();
	glScaled(1.0, 0.6, 1.2);
	glutWireCube(1.0);
	glPopMatrix();
/* 4th link *人指し指第2関節まで*/			
	glPushMatrix();
	glPushMatrix();
	glTranslated(0.5, 0.0, -0.5);
	glRotated((double)fin_4, 0.0, 0.0, 1.0);
	glRotated((double)fin_4_y, 0.0, 1.0, 0.0);
	glTranslated(0.25, 0.0, -0.1);
	glScaled(0.45, 0.3, 0.3);
	glutWireCube(1.0);
		glPushMatrix();/* 5th link *人指し指第1関節まで*/
		glTranslated(0.5, 0.0, 0.5);
		glRotated((double)fin_5, 0.0, 0.0, 1.0);
		glTranslated(0.5, 0.0, -0.45);
		glutWireCube(1.0);
			glPushMatrix();/* 6th link *人指し指爪*/
			glTranslated(0.5, 0.0, 1.0);
			glRotated((double)fin_6, 0.0, 0.0, 1.0);
			glTranslated(0.5, 0.0, -1.0);
			glutWireCube(1.0);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
/* 7th link *中指第2関節まで*/
	glPushMatrix();
	glTranslated(0.5, 0.0, -0.5);
	glRotated((double)fin_7, 0.0, 0.0, 1.0);
	glRotated((double)fin_7_y, 0.0, 1.0, 0.0);
	glTranslated(0.25, 0.0, 0.3);
	glScaled(0.45, 0.3, 0.4);
	glutWireCube(1.0);
		glPushMatrix();/* 8th link *中指第1関節まで*/
		glTranslated(0.5, 0.0, 0.5);
		glRotated((double)fin_8, 0.0, 0.0, 1.0);
		glTranslated(0.5, 0.0, -0.45);
		glutWireCube(1.0);
			glPushMatrix();/* 9th link *中指爪*/
			glTranslated(0.5, 0.0, 1.0);
			glRotated((double)fin_9, 0.0, 0.0, 1.0);
			glTranslated(0.5, 0.0, -1.0);
			glutWireCube(1.0);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
/* 10th link *薬指第2関節まで*/
	glPushMatrix();
	glTranslated(0.5, 0.0, -0.5);
	glRotated((double)fin_10, 0.0, 0.0, 1.0);
	glRotated((double)fin_10_y, 0.0, 1.0, 0.0);
	glTranslated(0.25, 0.0, 0.7);
	glScaled(0.45, 0.3, 0.4);
	glutWireCube(1.0);
		glPushMatrix();/* 11th link *薬指第1関節まで*/
		glTranslated(0.5, 0.0, 0.5);
		glRotated((double)fin_11, 0.0, 0.0, 1.0);
		glTranslated(0.5, 0.0, -0.45);
		glutWireCube(1.0);
			glPushMatrix();/* 12th link *薬指爪*/
			glTranslated(0.5, 0.0, 1.0);
			glRotated((double)fin_12, 0.0, 0.0, 1.0);
			glTranslated(0.5, 0.0, -1.0);
			glutWireCube(1.0);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
/* 13th link *小指第2関節まで*/
	glPushMatrix();
	glTranslated(0.5, 0.0, -0.5);
	glRotated((double)fin_13, 0.0, 0.0, 1.0);
	glRotated((double)fin_13_y, 0.0, 1.0, 0.0);
	glTranslated(0.25, 0.0, 1.1);
	glScaled(0.45, 0.3, 0.4);
	glutWireCube(1.0);
		glPushMatrix();/* 14th link *小指第1関節まで*/
		glTranslated(0.5, 0.0, 0.5);
		glRotated((double)fin_14, 0.0, 0.0, 1.0);
		glTranslated(0.5, 0.0, -0.45);
		glutWireCube(1.0);
			glPushMatrix();/* 15th link *小指爪*/
			glTranslated(0.5, 0.0, 1.0);
			glRotated((double)fin_15, 0.0, 0.0, 1.0);
			glTranslated(0.5, 0.0, -1.0);
			glutWireCube(1.0);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
/* 16th link *親指第一関節まで*/
	glTranslated(0.3, 0.0, 0.1);
	glPushMatrix();
	glTranslated(0.5, 0.0, -0.5);
	glRotated(20.0, 0.0, 1.0, 0.0);
	glRotated((double)fin_16, 1.0, 0.0, 0.0);
	glTranslated(-0.6, 0.0, -0.5);
	glScaled(0.45, 0.3, 0.4);
	glutWireCube(1.0);
		glPushMatrix();/* 17th link *親指爪*/
		glTranslated(0.5, 0.0, 0.5);
		glRotated((double)fin_17, 0.0, 0.0, 1.0);
		glTranslated(0.5, 0.0, -0.45);
		glutWireCube(1.0);
		glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}

void myReshape (int width, int height)
{
	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)width/(double)height, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslated( -2.0, 3.0, -5.0); 
	gluLookAt(0.0, 4.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // move to enable viewing
}

void myTimer(int value)
{   
	
	if (value == 1)
	{	
		if (rps_type == 0) {//グー
			timenow = time(NULL) - start_time;
			glutTimerFunc(samplingTime, myTimer, 1);
			if (timenow <= 2) {
				shoulder = (shoulder + 2) % 360;//肩を振り上げる
				elbow = (elbow + 2) % 360;//肘を振り上げる
				glutPostRedisplay();
			}
			if (timenow > 3 && timenow <= 5) {
				shoulder = (shoulder - 2) % 360;//肩を振り下げる
				elbow = (elbow - 2) % 360;//肘を振り下げる
				glutPostRedisplay();
			}
			if (timenow <= 5) {
				hand = (hand + 2) % 360;//手を上げる
				fin_4 = (fin_4 - 3) % 360;//人差し指曲げる
				fin_5 = (fin_5 - 2) % 360;
				fin_6 = (fin_6 -1) % 360;
				fin_7 = (fin_7 - 3) % 360;//中指曲げる
				fin_8 = (fin_8 - 2) % 360;
				fin_9 = (fin_9 - 2) % 360;
				fin_10 = (fin_10 - 3) % 360;//薬指曲げる
				fin_11 = (fin_11 - 2) % 360;
				fin_12 = (fin_12 - 2) % 360;
				fin_13 = (fin_13 - 3) % 360;//小指曲げる
				fin_14 = (fin_14 - 2) % 360;
				fin_15 = (fin_15 - 2) % 360;
				fin_16 = (fin_16 - 1) % 360;//親指曲げる
				fin_17 = (fin_17 - 2) % 360;
				glutPostRedisplay();
			}
			
		}
		if (rps_type == 1) {//チョキ

			timenow = time(NULL) - start_time;
			glutTimerFunc(samplingTime, myTimer, 1);
			if (timenow <= 2) {
				shoulder = (shoulder + 2) % 360;//肩を振り上げる
				elbow = (elbow + 2) % 360;//肘を振り上げる
				glutPostRedisplay();
			}
			if (timenow > 3 && timenow <= 5) {
				shoulder = (shoulder - 2) % 360;//肩を振り下げる
				elbow = (elbow - 2) % 360;//肘を振り下げる
				fin_4_y = (fin_4_y + 1) % 360;//人差し指の第2関節までを開く
				fin_7_y = (fin_7_y - 1) % 360;//中指の第2関節までを開く
				glutPostRedisplay();
			}

			if (timenow <= 5) {//kusu
				hand = (hand + 2) % 360;//手を上げる
				fin_10 = (fin_10 - 3) % 360;//薬指曲げる
				fin_11 = (fin_11 - 3) % 360;
				fin_12 = (fin_12 - 3) % 360;
				fin_13 = (fin_13 - 3) % 360;//小指曲げる
				fin_14 = (fin_14 - 3) % 360;
				fin_15 = (fin_15 - 3) % 360;
				fin_16 = (fin_16 - 1) % 360;//親指曲げる
				fin_17 = (fin_17 - 2) % 360;
				glutPostRedisplay();
			}
		}
		if (rps_type == 2) {//パー
			timenow = time(NULL) - start_time;
			glutTimerFunc(samplingTime, myTimer, 1);
			if (timenow <= 2) {
				shoulder = (shoulder + 2) % 360;//肩を振り上げる
				elbow = (elbow + 2) % 360;//肘を振り上げる
				glutPostRedisplay();
			}
			if (timenow <= 3) {
				fin_10_y = (fin_10_y - 1) % 360;//薬指を開く
				glutPostRedisplay();
			}
			if (timenow > 3 && timenow <= 5) {
				shoulder = (shoulder - 2) % 360;//肩を振り下げる
				elbow = (elbow - 2) % 360;//肘を振り下げる
				glutPostRedisplay();
			}
			if (timenow <= 4) {
				fin_13_y = (fin_13_y + -1) % 360;//小指を開く
				glutPostRedisplay();
			}
			if (timenow <= 5) {
				hand = (hand + 2) % 360;//手を上げる
				fin_4_y = (fin_4_y + 1) % 360;//人差し指を開く
				glutPostRedisplay();
			}

		}
		glutPostRedisplay();		
	}
	
}


int main(int argc, char** argv){	
	start_time = time(NULL);
	rps_type = 0;//0の場合グー、１の場合チョキ、２の場合パー
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutTimerFunc(samplingTime, myTimer, 1);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay); 
	glutMainLoop();
	return 0;
}
