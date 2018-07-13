#include "linear_algebra.h"

GLfloat *rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
		GLfloat c = (GLfloat)cos((double)angle);
		GLfloat s = (GLfloat)sin((double)angle);
		GLfloat *result = malloc(4*4*sizeof(GLfloat));
		
		GLfloat len = sqrt((x*x)+(y*y)+(z*z));
		x /= len;
		y /= len;
		z /= len;

		result[ 0] = x*x*(1.0f-c)+c;
		result[ 1] = x*y*(1.0f-c)-z*s;
		result[ 2] = x*z*(1.0f-c)+y*s;
		result[ 3] = 0.0f;

		result[ 4] = y*x*(1.0f-c)+z*s;
		result[ 5] = y*y*(1.0f-c)+c;
		result[ 6] = y*z*(1.0f-c)-x*s;
		result[ 7] = 0.0f;

		result[ 8] = z*x*(1.0f-c)-y*s;
		result[ 9] = z*y*(1.0f-c)+x*s;
		result[10] = z*z*(1.0f-c)+c;
		result[11] = 0.0f;

		result[12] = 0.0f;
		result[13] = 0.0f;
		result[14] = 0.0f;
		result[15] = 1.0f;

		return result;
}

GLfloat *ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
			   GLfloat nearVal, GLfloat farVal)
{
	    GLfloat tx = -(right+left)/(right-left);
		GLfloat ty = -(top+bottom)/(top-bottom);
		GLfloat tz = -(farVal+nearVal)/(farVal-nearVal);
		GLfloat *result = malloc(4*4*sizeof(GLfloat));

		result[ 0] = 2.0f/(right-left);
		result[ 1] = 0.0f;
		result[ 2] = 0.0f;
		result[ 3] = tx;

		result[ 4] = 0.0f;
		result[ 5] = 2.0f/(top-bottom);
		result[ 6] = 0.0f;
		result[ 7] = ty;

		result[ 8] = 0.0f;
		result[ 9] = 0.0f;
		result[10] = -2.0f/(farVal-nearVal);
		result[11] = tz;

		result[12] = 0.0f;
		result[13] = 0.0f;
		result[14] = 0.0f;
		result[15] = 1.0f;

		return result;
}
