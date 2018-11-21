#ifndef _CG_HW1_DBLPEN_H_
#define _CG_HW1_DBLPEN_H_

#include "Vector.hpp"
#include <deque>

/**
 * This class is for representing a single double pendulum system abstractly.
 * 
*/

class DoublePendulum
{
public:

	DoublePendulum(Vector3f rootPosition, float topRodLength, float bottomRodLength, float topBobWeight, float bottomBobWeight)
	{
		this->rootPosition = rootPosition;
		this->topRodLength = topRodLength;
		this->bottomRodLength = bottomRodLength;
		this->topBobWeight = topBobWeight;
		this->bottomBobWeight = bottomBobWeight;
	}

	void render()
	{
		// reminder : the radius of a bob should be proportional to the weight of the bob ( r : weight^(0.33) )

		// you can use GLUquadricObj to draw sphere more easily, but please remember to Translate to the 
		// correct position.

		//GLUquadricObj* quad = gluNewQuadric();
		//gluSphere(quad, r, 20, 20);

		// write your code here ...
		Vector3f topBobPos = GetTopBobPosition();
		Vector3f bottomBobPos = GetBottomBobPosition();

		glPushMatrix();
			glBegin(GL_LINES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(rootPosition.x, rootPosition.y, rootPosition.z);
				glVertex3f(topBobPos.x, topBobPos.y, topBobPos.z);
			glEnd();
			glPushMatrix();
				glTranslatef(topBobPos.x, topBobPos.y, topBobPos.z);
				glRotatef(-topAngle * 180 / M_PI, 0, 0, 1);
				glNormal3f(rootPosition.x - topBobPos.x, rootPosition.y - topBobPos.y, rootPosition.z - topBobPos.z);
				glColor3f(0.33f, 0.33f, 0.33f);
				glutSolidCube(pow(topBobWeight, 0.33));
			glPopMatrix();
			glBegin(GL_LINES);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(topBobPos.x, topBobPos.y, topBobPos.z);
				glVertex3f(bottomBobPos.x, bottomBobPos.y, bottomBobPos.z);
			glEnd();
			glPushMatrix();
				glTranslatef(bottomBobPos.x, bottomBobPos.y, bottomBobPos.z);
				glNormal3f(topBobPos.x - bottomBobPos.x, topBobPos.y - bottomBobPos.y, topBobPos.z - bottomBobPos.z);
				GLUquadricObj* quad2 = gluNewQuadric();
				if (isLightSource){
					glColor3f(1.0f, 1.0f, 1.0f);
					gluQuadricDrawStyle(quad2, GLU_FILL);
					gluQuadricNormals(quad2, GLU_SMOOTH);
					gluQuadricTexture(quad2, GL_TRUE);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, textureID);//sun texture
					gluSphere(quad2, pow(bottomBobWeight, 0.33), 20, 20);
					glDisable(GL_TEXTURE_2D);
					isUsingTexture = true;
				}
				else{
					glColor3f(0.22f, 0.22f, 0.22f);
					gluSphere(quad2, pow(bottomBobWeight, 0.33), 20, 20);
				}
			glPopMatrix();
			if (isUsingTexture){
				points.push_back(bottomBobPos);
				if (points.size() > 90)
					points.pop_front();
				glBegin(GL_LINE_STRIP);
				for (int i = 0; i < points.size(); i++)
					glVertex3f(points[i].x, points[i].y, points[i].z);
				glEnd();
			}
		glPopMatrix();
	}

	void updatePhysics(float deltaTime)
	{
		// Reference : https://www.youtube.com/watch?v=uWzPe_S-RVE

		// this method should be erased when publishing template project
		const float g = 9.8;

		const float m1 = topBobWeight;
		const float m2 = bottomBobWeight;
		const float a1 = topAngle;
		const float a2 = bottomAngle;
		const float r1 = topRodLength;
		const float r2 = bottomRodLength;
		const float a1_v = topAngleVelocity;
		const float a2_v = bottomAngleVelocity;

		// calculate topAngleAcceleration
		// write your code here...
		// topAngleAcceleration = ...
		float num1 = -g * (2 * m1 + m2) * sin(a1);
		float num2 = -m2 * g * sin(a1 - 2 * a2);
		float num3 = -2 * sin(a1 - a2) * m2;
		float num4 = a2_v * a2_v * r2 + a1_v * a1_v * r1 * cos(a1 - a2);
		float den = r1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
		topAngleAcceleration = (num1 + num2 + num3 * num4) / den;

		// calculate bottomAngleAcceleration
		// write your code here
		// bottomAngleAcceleration = ...
		num1 = 2 * sin(a1 - a2);
		num2 = (a1_v * a1_v * r1 * (m1 + m2));
		num3 = g * (m1 + m2) * cos(a1);
		num4 = a2_v * a2_v * r2 * m2 * cos(a1 - a2);
		den = r2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
		bottomAngleAcceleration = num1 * (num2 + num3 + num4) / den;

		// apply and record
		topAngleVelocity += topAngleAcceleration * deltaTime;
		bottomAngleVelocity += bottomAngleAcceleration * deltaTime;

		topAngle += topAngleVelocity * deltaTime;
		bottomAngle += bottomAngleVelocity * deltaTime;
	}

public:
	float topRodLength;
	float topBobWeight;
	float bottomRodLength;
	float bottomBobWeight;

	// for rendering settings
	bool isLightSource = false;
	bool isUsingTexture = false;
	GLuint textureID = 0;
	std::deque<Vector3f> points;

	Vector3f GetTopBobPosition()
	{
		// write your code here...
		Vector3f temp;
		temp.z = rootPosition.z;
		temp.y = rootPosition.y - topRodLength * cos(topAngle);
		temp.x = rootPosition.x - topRodLength * sin(topAngle);
		return temp;
	}

	Vector3f GetBottomBobPosition()
	{
		// write your code here...
		Vector3f temp = GetTopBobPosition(), temp2;
		temp2.z = temp.z;
		temp2.y = temp.y - bottomRodLength * cos(bottomAngle);
		temp2.x = temp.x - bottomRodLength * sin(bottomAngle);
		return temp2;
	}

private:
	Vector3f rootPosition;

	// for updating motion
	float topAngle = M_PI / 4; // in this template, this is represented in radian form
	float bottomAngle = 0; // in this template, this is represented in radian form

	float topAngleVelocity = 0;
	float bottomAngleVelocity = 0;

	float topAngleAcceleration = 0;
	float bottomAngleAcceleration = 0;
};

#endif