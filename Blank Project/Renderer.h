#pragma once
#include "\\campus\home\home2018\b8011981\CSC8502 2021\nclgl\OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
class Camera;
class Shader;
class HeightMap;
class Mesh;
class MeshAnimation;
class MeshMaterial;

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	 ~Renderer(void);
	 void RenderScene()				override;
	 void UpdateScene(float msec)	override;
protected:
	Mesh* quad;
	Mesh* cube;
	Mesh* sphere;

	Mesh* tree1;
	Mesh* tree2;
	Mesh* tree3;
	Mesh* tree4;
	Mesh* tree5;
	std::vector<Mesh*> treeMeshes;

	Mesh* rock1;
	Mesh* rock2;
	Mesh* rock3;
	Mesh* rock4;
	Mesh* rock5;
	Mesh* rock6;
	Mesh* rock7;
	Mesh* rock8;
	std::vector<Mesh*> rockMeshes;

	Mesh* mesh;

	Mesh* firewood;
	
	Mesh* blueJayMesh;
	MeshAnimation* birdAnim;
	MeshMaterial* blueJayMaterial;
	vector<GLuint> blueJayMatTextures;
	std::vector<SceneNode*> birds;

	Mesh* bennuMesh;
	MeshAnimation* bennuAnim;
	MeshMaterial* bennuMat;
	vector<GLuint> bennuMatTextures;

	int currentFrame;
	float frameTime;

	

	Light* light;
	Light* campfireLight;
	
	SceneNode* root;

	Camera* camera;
	std::vector<Vector3> cameraPos;
	std::vector<float> cameraYaws;
	int nextPos = 1;

	HeightMap* heightMap;

	void DrawNode(SceneNode* n);
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawPostProcess();
	void PresentScene();
	void UpdateBirds(float dt);
	void UpdateCampfire(float dt);
	void UpdateSun(float dt);
	void UpdateCamera(float dt);
	bool freeCam = false;

	Shader* animShader;
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* shader;
	Shader* processShader;

	GLuint cubeMap;
	GLuint waterTex;
	GLuint earthTex;
	GLuint earthBump;
	GLuint sandTex;
	GLuint grassTex;
	GLuint sandBump;
	GLuint grassBump;
	GLuint treeTex;
	GLuint treeBump;
	GLuint watermap;
	GLuint rockTex;
	GLuint rockBump1;
	GLuint rockBump2;
	GLuint rockBump3;
	GLuint rockBump4;
	GLuint rockBump5;
	GLuint rockBump6;
	GLuint rockBump7;
	GLuint rockBump8;
	GLuint EagleTex;
	GLuint BennuTex;
	GLuint firewoodTex;
	GLuint firewoodBump;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;

	std::vector<GLuint> rockBumps;
	std::vector<Vector3> aboveSeaLevels;
	std::vector<Vector3> rockLayerPositions;

	void DrawShadowScene();
	void DrawShadowNode(SceneNode* n);
	GLuint shadowTex;
	GLuint shadowFBO;

	GLuint sceneDiffuse;
	GLuint sceneBump;
	float sceneTime;

	Shader* shadowShader;

	vector<Mesh*> sceneMeshes;
	vector<Matrix4> sceneTransforms;
	float waterRotate;
	float waterCycle;
	bool rising = true;
	int texValue = 0;
	float angle = 0;
	Vector4 dayLightValue = Vector4(0.89, 0.86, 0.45, 1);
	Vector4 duskDawnLightValue = Vector4(0.8, 0.7, 0.01, 1);
	Vector4 nightLightValue = Vector4(0, 0, 0.15, 1);
	std::vector<Light> lights;
};
