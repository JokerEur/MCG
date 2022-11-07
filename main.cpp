#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <iostream>

float Points[][3] = {
	{-12.8f, -3.2f, 0.0f},
	{-9.6f, 3.2f, 0.0f},
	{-6.4f, -3.2f, 0.0f},
	{-3.2f, 3.2f, 0.0f},
	{0.0f, -3.2f, 0.0f},
	{3.2f, 3.2f, 0.0f},
	{6.4f, -3.2f, 0.0f},
	{9.6f, 3.2f, 0.0f},
	{12.8f,-3.2f,0.0f},
	{15.4f, 3.2f, 0.0f}
	};

const std::uint8_t numPoints{10}; // number of points
const std::uint16_t degree{9}; // degree of polynomial
const std::uint16_t order{degree+1};

const std::uint16_t LOD{50};

const std::uint8_t scale{5};
const std::uint8_t moveScale{15};

// static float knots[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0}; // knots for 7 points
// static float knots[] = {0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 5.0, 5.0, 5.0}; //knots for 8 points
// static float knots[] = {0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 6.0, 6.0, 6.0}; // knots for 9 points
static float knots[] = {0.0, 0.0, 0.0, 0.0, 0.0 ,0.0 ,0.0 ,0.0 ,0.0 ,0.0 ,1.0 ,1.0 ,1.0 ,1.0 ,1.0 ,1.0 ,1.0 ,1.0 ,1.0 ,1.0};  // knots for 10 points

float Bspline(int index, int order, float u){
	float coef1, coef2;
	if (order == 1)
	{
		if (index == 0)
			if ((knots[index] <= u) && (u <= knots[index + 1]))
				return 1.0;
		if ((knots[index] < u) && (u <= knots[index + 1]))
			return 1.0;
		else
			return 0.0;
	}else{
		if (knots[index + order - 1] == knots[index])
		{
			if (u == knots[index])
				coef1 = 1;
			else
				coef1 = 0;
		}
		else
			coef1 = (u - knots[index]) / (knots[index + order - 1] - knots[index]);

		if (knots[index + order] == knots[index + 1])
		{
			if (u == knots[index + order])
				coef2 = 1;
			else
				coef2 = 0;
		}
		else
			coef2 = (knots[index + order] - u) / (knots[index + order] - knots[index + 1]);

		return (coef1 * Bspline(index, order - 1, u) + coef2 * Bspline(index + 1, order - 1, u));
	}
}

void GetOutpoint(float t,float OutPoint[]) {

	for(ptrdiff_t i{0}; i < numPoints; ++i) {

		float Val = Bspline(i,order,t);

		if(Val>0.001f) {
			OutPoint[0] += Val * Points[i][0];
			OutPoint[1] += Val * Points[i][1];
			OutPoint[2] += Val * Points[i][2];
		}
	}
}

void OnDraw(){

	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

  	////////!DO NOT TOUCH THE DOG!////////
  										//
	gluLookAt(1, 10, 30, //eye pos		//
			  0, 0, 0,	//aim point		//
			  0, 1, 0);	//up direction	//
										//
	////////!DON'T ASK QUESTIONS!/////////


	//begin of drawing B-spline via De-Boor
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINE_STRIP);

		for(ptrdiff_t i{0} ;i!=LOD;++i) {

			float t  = knots[(sizeof(knots)/sizeof(knots[0]))-1] * i / static_cast<float>(LOD-1);

			if(i==LOD-1) 
				t-=0.001f;
			float Outpoint[3]={0,0,0};

			GetOutpoint(t,Outpoint);
			glVertex3fv(Outpoint);
		}
		
	glEnd();
	//end of drawing B-spline via De-Boor


	//begin of drawing the points
	glPointSize(4);
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
		for (ptrdiff_t i{0}; i < numPoints; ++i){
			glVertex3fv(Points[i]);
		}
	glEnd();
	//end of drawing the points


	//begin of drawing basis function
	glPointSize(1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
		for (ptrdiff_t i {0}; i < sizeof(knots)/sizeof(knots[0]) - order; ++i){
			for (float x{knots[i]}; x <= knots[i+order]; x += 0.0005)
				glVertex3f(x * scale - moveScale,(Bspline(i, order, x) * scale) - moveScale, 0.0);
		}
	glEnd();
	//end of drawing basis function

	glutSwapBuffers();
}

void OnReshape(int w, int h){
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, static_cast<float>(w) / h, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Window");
	glutDisplayFunc(OnDraw);
	glutReshapeFunc(OnReshape);
	glutMainLoop();

	return 0;
}