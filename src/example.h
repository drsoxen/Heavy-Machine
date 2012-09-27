#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include <windows.h>
#include <vector>
#include <string>
#include "vertex.h"
#include "Model.h"
#include "targa.h"
#include "ModelLoader.h"
#include "InputListener.h"


class GLSLProgram; 

using std::vector;
using std::string;


class Example:public InputListener
{
public:
	ModelLoader modelLoader;

	Model* model;

	vector<Model*>Models;

	Example();
    virtual ~Example();

	bool init();
	void prepare(float dt);
	void render();
	void shutdown();
	void handleInput(WPARAM wparam);
	void onResize(int width, int height);
	void keyDownEvent(WPARAM input);
	void keyUpEvent(WPARAM input);
	void CheckInput();	

	void LoadTextures(const string& filename, TargaImage &Texture, GLuint* ID);

    vector<float> calculateNormalMatrix(const float* modelviewMatrix);

	float DeltaTime;
	float timer;
	float transX;
	float transY;
	float transZ;
	float moveRate;
	bool rotate;
	float m_CamX;
	float m_CamY;
	float m_CamZ;
	int mouseX,mouseY,prevMouseX,prevMouseY;
	bool isPressed;
	float HackyRearLights;
	float HackyFrontLights;


	bool BaseRight;
	bool BaseLeft;
	bool BaseForward;
	bool BaseBackward;
	bool CabRight;
	bool CabLeft;
	bool Arm1Up;
	bool Arm1Down;
	bool Arm2Up;
	bool Arm2Down;
	bool BucketUp;
	bool BucketDown;

	TargaImage m_CabTexture;
    TargaImage m_Arm1Texture;
    TargaImage m_Arm2Texture;
    TargaImage m_BaseTexture;
    TargaImage m_BucketTexture;
    TargaImage m_GroundTexture;
    TargaImage m_SkyTexture;

	Model* ModelBase;
	Model* ModelCab;
	Model* ModelArm1;
	Model* ModelArm2;
	Model* ModelBucket;
	Model* ModelGround;
	Model* ModelSky;

private:
	float m_rotationAngle;
	float m_lightPosZ;
	float m_angle;

	GLSLProgram* m_GLSLProgram;

	GLuint m_BaseTexID;
	GLuint m_CabTexID;
    GLuint m_Arm1TexID;    
    GLuint m_Arm2TexID;    
    GLuint m_BucketTexID;
    GLuint m_GroundTexID;
    GLuint m_SkyTexID;	
};

#endif
