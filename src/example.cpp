#ifdef _WIN32
#include <windows.h>
#endif

#include <cassert>
#include <iostream>
#include <fstream>

#include "glee/GLee.h"
#include <GL/glu.h>

#include "example.h"
#include "glslshader.h"
#include "InputManager.h"

#define SPEED 25
#define PI 3.14159265358979323846

Example::Example(): m_angle(0.0f), m_lightPosZ(0.0f)
{
	m_GLSLProgram = new GLSLProgram("./../data/basic-fixed.vert", "./../data/basic-fixed.frag");

	m_rotationAngle = 0.5f;
	transX = 0.0f;
	transY = 20.0f;
	transZ = 90.0f;
	moveRate = 0.5f;
	rotate = false;
	m_CamX = -44;
	m_CamY = 35;
	m_CamZ = 100;
	mouseX=0;
	mouseY=0;
	prevMouseX=0;
	prevMouseY=0;
	DeltaTime = 0;
	timer = 0;
	timer = 0;
	isPressed =
	BaseRight = 
	BaseLeft = 
	BaseForward = 
	BaseBackward = 
	CabRight = 
	CabLeft = 
	Arm1Up = 
	Arm1Down = 
	Arm2Up = 
	Arm2Down = 
	BucketUp = 
	BucketDown = false;

	HackyRearLights = 0.0f;
	HackyFrontLights = 0.0f;
}

Example::~Example()
{
	delete m_GLSLProgram;
}

void Example::keyUpEvent(WPARAM input)
{
	if(input ==  VK_UP)
	{
		BaseForward = false;
		HackyFrontLights = 0.0f;
	}

	if(input ==  VK_DOWN)
	{
		BaseBackward = false;
		HackyRearLights = 0.0f;
	}

	if(input ==  VK_LEFT)
		BaseLeft = false;

	if(input ==  VK_RIGHT)
		BaseRight = false;

	if(input ==  'Z')
		CabRight = false;

	if(input ==  'C')
		CabLeft = false;

	if(input ==  'D')
		BucketUp = false;

	if(input ==  'E')
		BucketDown = false;

	if(input ==  'A')
		Arm1Up = false;

	if(input ==  'Q')
		Arm1Down = false;

	if(input ==  'S')
		Arm2Up = false;

	if(input ==  'W')
		Arm2Down = false;
}

void Example::keyDownEvent(WPARAM input)
{
	if( input ==  'T')
	{
		LoadTextures("./../data/Sky.tga", m_SkyTexture, &m_SkyTexID);
		LoadTextures("./../data/Ground.tga",m_GroundTexture,&m_GroundTexID);
		LoadTextures("./../data/Cab.tga",m_CabTexture,&m_CabTexID);
		LoadTextures("./../data/Base.tga",m_BaseTexture,&m_BaseTexID);
		LoadTextures("./../data/Arm1.tga",m_Arm1Texture,&m_Arm1TexID);
		LoadTextures("./../data/Arm2.tga",m_Arm2Texture,&m_Arm2TexID);
		LoadTextures("./../data/Bucket.tga",m_BucketTexture,&m_BucketTexID);
	}

	if(input ==  'N')
		for(int i = 0; i<Models.size();i++)
			Models[i]->m_ShowNormals = !Models[i]->m_ShowNormals;

	if(input ==  VK_CONTROL)
		rotate = !rotate;

	if(input ==  VK_UP)
	{
		BaseForward = true;
		HackyFrontLights = 1.0f;
	}

	if(input ==  VK_DOWN)
	{
		BaseBackward = true;
		HackyRearLights = 1.0f;
	}

	if(input ==  VK_LEFT)
		BaseLeft = true;

	if(input ==  VK_RIGHT)
		BaseRight = true;

	if(input ==  'Z')
		CabRight = true;

	if(input ==  'C')
		CabLeft = true;

	if(input ==  'D')
		BucketUp = true;

	if(input ==  'E')
		BucketDown = true;

	if(input ==  'A')
		Arm1Up = true;

	if(input ==  'Q')
		Arm1Down = true;

	if(input ==  'S')
		Arm2Up = true;

	if(input ==  'W')
		Arm2Down = true;
}

void Example::CheckInput()
{
	if(CabRight)
		ModelCab->m_RotateY+=(SPEED*DeltaTime);

	if(CabLeft)
		ModelCab->m_RotateY-=(SPEED*DeltaTime);

	if(BucketDown)
		if(ModelBucket->m_RotateZ > 177)
			ModelBucket->m_RotateZ-=((SPEED*1.5)*DeltaTime);

	if(BucketUp)
		if(ModelBucket->m_RotateZ < 267)		
			ModelBucket->m_RotateZ+=((SPEED*1.5)*DeltaTime);

	if(Arm1Up)
		if(ModelArm1->m_RotateZ < 38)
			ModelArm1->m_RotateZ+=(SPEED*DeltaTime);

	if(Arm1Down)
		if(ModelArm1->m_RotateZ > -28)
			ModelArm1->m_RotateZ-=(SPEED*DeltaTime);

	if(Arm2Up)
		if(ModelArm2->m_RotateZ < -214)
			ModelArm2->m_RotateZ+=(SPEED*DeltaTime);

	if(Arm2Down)
		if(ModelArm2->m_RotateZ > -283)
			ModelArm2->m_RotateZ-=(SPEED*DeltaTime);

	if(BaseRight)
		ModelBase->m_RotateY-=(SPEED*DeltaTime);

	if(BaseLeft)
		ModelBase->m_RotateY+=(SPEED*DeltaTime);

	if(BaseForward)
	{
		ModelBase->m_offsetX += (sin(((ModelBase->m_RotateY - 90) * PI / 180)))*((SPEED*1.5)*DeltaTime);
		ModelBase->m_offsetZ += (cos(((ModelBase->m_RotateY - 90) * PI / 180)))*((SPEED*1.5)*DeltaTime);
	}
	if(BaseBackward)
	{
		ModelBase->m_offsetX -= (sin(((ModelBase->m_RotateY - 90) * PI / 180)))*((SPEED*1.5)*DeltaTime);
		ModelBase->m_offsetZ -= (cos(((ModelBase->m_RotateY - 90) * PI / 180)))*((SPEED*1.5)*DeltaTime);
	}
}

bool Example::init()
{
	if (!m_GLSLProgram->initialize())
	{
		std::cerr << "Could not initialize the shaders" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.30f, 0.50f, 0.80f, 0.0f);

	m_GLSLProgram->bindAttrib(0, "a_Vertex");
	m_GLSLProgram->bindAttrib(1, "a_TexCoord");
	m_GLSLProgram->bindAttrib(2, "a_Normal");

	m_GLSLProgram->linkProgram();

	m_GLSLProgram->bindShader(); 

	ModelBase = modelLoader.Load("../Content/BackHoeBase.obj");

	ModelCab = modelLoader.Load("../Content/BackHoeCab.obj");
	ModelArm1 = modelLoader.Load("../Content/BackHoeArm1.obj");
	ModelArm2 = modelLoader.Load("../Content/BackHoeArm2.obj");
	ModelBucket = modelLoader.Load("../Content/BackHoeBucket.obj");
	ModelGround = modelLoader.Load("../Content/Ground.obj");
	ModelSky = modelLoader.Load("../Content/SkyBox.obj");

	ModelSky->m_Lights = false;	

	ModelBase->m_offsetX = 0;
	ModelBase->m_offsetY = 0;
	ModelBase->m_offsetZ = 0;	

	ModelCab->m_offsetX = 0;
	ModelCab->m_offsetY = 5.5;
	ModelCab->m_offsetZ = 0;

	ModelArm1->m_offsetX = -5;
	ModelArm1->m_offsetY = 6;
	ModelArm1->m_offsetZ = -1.5;
	ModelArm1->m_RotateX = 0;
	ModelArm1->m_RotateY = 0;
	ModelArm1->m_RotateZ = 0;

	ModelArm2->m_offsetX = -21;
	ModelArm2->m_offsetY = 13;
	ModelArm2->m_offsetZ = 0;
	ModelArm2->m_RotateX = 0;
	ModelArm2->m_RotateY = 0;
	ModelArm2->m_RotateZ = -235;

	ModelBucket->m_offsetX = -8;
	ModelBucket->m_offsetY = 17;
	ModelBucket->m_offsetZ = 0;
	ModelBucket->m_RotateX = 0;
	ModelBucket->m_RotateY = 0;
	ModelBucket->m_RotateZ = 237;

	ModelSky->m_Parent = NULL;
	ModelGround->m_Parent = NULL;
	ModelBase->m_Parent = ModelGround;
	ModelCab->m_Parent = ModelBase;
	ModelArm1->m_Parent = ModelCab;
	ModelArm2->m_Parent = ModelArm1;
	ModelBucket->m_Parent = ModelArm2;

	ModelGround->m_ChildList.push_back(ModelBase);
	ModelBase->m_ChildList.push_back(ModelCab);
	ModelCab->m_ChildList.push_back(ModelArm1);
	ModelArm1->m_ChildList.push_back(ModelArm2);
	ModelArm2->m_ChildList.push_back(ModelBucket);
	
	Models.push_back(ModelGround);
	Models.push_back(ModelBase);
	Models.push_back(ModelCab);
	Models.push_back(ModelArm1);
	Models.push_back(ModelArm2);
	Models.push_back(ModelBucket);
	Models.push_back(ModelSky);

	glEnable(GL_CULL_FACE);

	for (int i = 0;i<Models.size();i++)
		Models[i]->Init();

	LoadTextures("./../data/Sky.tga", m_SkyTexture, &m_SkyTexID);
	LoadTextures("./../data/Ground.tga",m_GroundTexture,&m_GroundTexID);
	LoadTextures("./../data/Cab.tga",m_CabTexture,&m_CabTexID);
	LoadTextures("./../data/Base.tga",m_BaseTexture,&m_BaseTexID);
	LoadTextures("./../data/Arm1.tga",m_Arm1Texture,&m_Arm1TexID);
	LoadTextures("./../data/Arm2.tga",m_Arm2Texture,&m_Arm2TexID);
	LoadTextures("./../data/Bucket.tga",m_BucketTexture,&m_BucketTexID);

	ModelSky->m_ID = m_SkyTexID;
	ModelGround->m_ID = m_GroundTexID;
	ModelBase->m_ID = m_BaseTexID;
	ModelCab->m_ID = m_CabTexID;
	ModelArm1->m_ID = m_Arm1TexID;
	ModelArm2->m_ID = m_Arm2TexID;
	ModelBucket->m_ID = m_BucketTexID;

	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1); 
	glEnableVertexAttribArray(2); 

	InputManager::GetInstance()->registerInput('Q');
	InputManager::GetInstance()->registerInput('A');
	InputManager::GetInstance()->registerInput('W');
	InputManager::GetInstance()->registerInput('S');
	InputManager::GetInstance()->registerInput('E');
	InputManager::GetInstance()->registerInput('D');
	InputManager::GetInstance()->registerInput('N');
	InputManager::GetInstance()->registerInput('R');
	InputManager::GetInstance()->registerInput('Z');
	InputManager::GetInstance()->registerInput('C');
	InputManager::GetInstance()->registerInput('T');
	InputManager::GetInstance()->registerInput(VK_UP);
	InputManager::GetInstance()->registerInput(VK_DOWN);
	InputManager::GetInstance()->registerInput(VK_LEFT);
	InputManager::GetInstance()->registerInput(VK_RIGHT);
	InputManager::GetInstance()->registerInput(VK_CONTROL);

	InputManager::GetInstance()->setListener(this);

	return true;
}

void Example::prepare(float dt)
{
	CheckInput();
	DeltaTime = dt;
	ModelSky->m_RotateY+=(SPEED*DeltaTime);

	const float ROTSPEED = 50.0f;

	if(rotate)
		m_rotationAngle += ROTSPEED * dt;

	if (m_rotationAngle > 360.0f)
		m_rotationAngle -= 360.0f;

	m_angle += 2.0f * dt;

	if (m_angle > 360.0f) 
		m_angle -= 360.0f;

	const float LIGHT_SPEED = 0.0005f;
	
	m_lightPosZ += LIGHT_SPEED;
	
	m_GLSLProgram->sendUniform("time", timer);
}

vector<float> Example::calculateNormalMatrix(const float* modelviewMatrix)
{
	vector<float> M(3 * 3);
	M[0] = modelviewMatrix[0];
	M[1] = modelviewMatrix[1];
	M[2] = modelviewMatrix[2];
	M[3] = modelviewMatrix[4];
	M[4] = modelviewMatrix[5];
	M[5] = modelviewMatrix[6];
	M[6] = modelviewMatrix[8];
	M[7] = modelviewMatrix[9];
	M[8] = modelviewMatrix[10];

	float determinate = M[0] * M[4] * M[8] + M[1] * M[5] * M[6] + M[2] * M[3] * M[7];
	determinate -= M[2] * M[4] * M[6] + M[0] * M[5] * M[7] + M[1] * M[3] * M[8];

	float oneOverDet = 1.0f / determinate;

	vector<float> N(3 * 3);

	N[0] = (M[4] * M[8] - M[5] * M[7]) * oneOverDet;
	N[3] = (M[2] * M[7] - M[1] * M[8]) * oneOverDet;
	N[6] = (M[1] * M[5] - M[2] * M[4]) * oneOverDet;
	N[1] = (M[5] * M[6] - M[3] * M[8]) * oneOverDet;
	N[4] = (M[0] * M[8] - M[2] * M[6]) * oneOverDet;
	N[7] = (M[2] * M[3] - M[0] * M[5]) * oneOverDet;
	N[2] = (M[3] * M[7] - M[4] * M[6]) * oneOverDet;
	N[5] = (M[1] * M[6] - M[8] * M[7]) * oneOverDet;
	N[8] = (M[0] * M[4] - M[1] * M[3]) * oneOverDet;

	return N;
}

void Example::render()
{
	float modelviewMatrix[16];
	float projectionMatrix[16];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(m_CamX, m_CamY, m_CamZ, 0.0, 0.0, 0.0f, 0.0f, 1.0f, 0.0f);

	glRotatef(m_rotationAngle, 0, 1, 0);

	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

	m_GLSLProgram->sendUniform4x4("projection_matrix", projectionMatrix);

	vector<float> normalMatrix = calculateNormalMatrix(modelviewMatrix);    
	m_GLSLProgram->sendUniform3x3("normal_matrix", &normalMatrix[0]);

	m_GLSLProgram->sendUniform("material_ambient", 0.2f, 0.2f, 0.2f, 1.0f);
	m_GLSLProgram->sendUniform("material_diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
	m_GLSLProgram->sendUniform("material_specular", 0.6f, 0.6f, 0.6f, 1.0f);
	m_GLSLProgram->sendUniform("material_emissive", 0.0f, 0.0f, 0.0f, 1.0f);

	m_GLSLProgram->sendUniform("lights[0].ambient", 0.0f, 0.0f, 0.0f, 1.0f);
	m_GLSLProgram->sendUniform("lights[0].diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
	m_GLSLProgram->sendUniform("lights[0].specular", 1.0f, 1.0f, 1.0f, 1.0f);
	m_GLSLProgram->sendUniform("lights[0].constant_attenuation", 1.0f);
	m_GLSLProgram->sendUniform("lights[0].linear_attenuation", 0.0f);
	m_GLSLProgram->sendUniform("lights[0].quadratic_attenuation", 0.0001f);
	m_GLSLProgram->sendUniform("lights[0].position", 0.0f, 30.0f, 0, 1.0f);

	m_GLSLProgram->sendUniform("lights[1].ambient", 0.0f, 0.0f, 0.0f, 1.0f);
	m_GLSLProgram->sendUniform("lights[1].diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
	m_GLSLProgram->sendUniform("lights[1].specular", 1.0f, 1.0f, 1.0f, 1.0f);
	m_GLSLProgram->sendUniform("lights[1].constant_attenuation", 1.0f);
	m_GLSLProgram->sendUniform("lights[1].linear_attenuation", 0.0f);
	m_GLSLProgram->sendUniform("lights[1].quadratic_attenuation", 0.0001f);
	m_GLSLProgram->sendUniform("lights[1].position", 41, 30.0f,41, 1.0f);

	m_GLSLProgram->sendUniform("lights[2].ambient", 0.0f, 0.0f, 0.0f, 1.0f);
	m_GLSLProgram->sendUniform("lights[2].diffuse", HackyRearLights, 0.0f, 0.0f, HackyRearLights);
	m_GLSLProgram->sendUniform("lights[2].specular", HackyRearLights, HackyRearLights, HackyRearLights, HackyRearLights);
	m_GLSLProgram->sendUniform("lights[2].constant_attenuation", 1.0f);
	m_GLSLProgram->sendUniform("lights[2].linear_attenuation", 0.0f);
	m_GLSLProgram->sendUniform("lights[2].quadratic_attenuation", 0.001f);
	m_GLSLProgram->sendUniform("lights[2].position", ModelBase->m_offsetX, ModelBase->m_offsetY,ModelBase->m_offsetZ - 10, 1.0f);

	m_GLSLProgram->sendUniform("lights[3].ambient", 0.0f, 0.0f, 0.0f, 1.0f);
	m_GLSLProgram->sendUniform("lights[3].diffuse", HackyFrontLights, HackyFrontLights, HackyFrontLights, HackyFrontLights);
	m_GLSLProgram->sendUniform("lights[3].specular", HackyFrontLights, HackyFrontLights, HackyFrontLights, HackyFrontLights);
	m_GLSLProgram->sendUniform("lights[3].constant_attenuation", 1.0f);
	m_GLSLProgram->sendUniform("lights[3].linear_attenuation", 0.0f);
	m_GLSLProgram->sendUniform("lights[3].quadratic_attenuation", 0.001f);
	m_GLSLProgram->sendUniform("lights[3].position", ModelBase->m_offsetX, ModelBase->m_offsetY,ModelBase->m_offsetZ + 10, 1.0f);

	Models[6]->Draw(m_GLSLProgram);

	glBindTexture(GL_TEXTURE_2D, m_BaseTexID);
	m_GLSLProgram->sendUniform("texture0", 0); 
	Models[0]->Draw(m_GLSLProgram);
}

void Example::shutdown()
{}

void Example::onResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, float(width) / float(height), 1.0f, 4000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Example::LoadTextures(const string& filename, TargaImage &Texture, GLuint* ID)
{
	if (!Texture.load(filename))
		std::cerr << "Could not load the grass texture" << std::endl;		

	glGenTextures(1, ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Texture.getWidth(), Texture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, Texture.getImageData());
}