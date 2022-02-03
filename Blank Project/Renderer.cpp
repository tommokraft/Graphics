#include "Renderer.h"
#include "..//nclgl/Light.h"
#include "..//nclgl/HeightMap.h"
#include "../nclgl/Shader.h"
#include "..//nclgl/Camera.h"
#include "..//nclgl/MeshAnimation.h"
#include "..//nclgl/MeshMaterial.h"
#define SHADOWSIZE 2048
const int POST_PASSES = 5;
Renderer::Renderer(Window &parent) : OGLRenderer(parent)	{
	srand(time(0));
	mesh = Mesh::LoadFromMeshFile("Eagle.msh");
	sphere = Mesh::LoadFromMeshFile("Sphere.msh");
	bennuMesh = Mesh::LoadFromMeshFile("Bennu.msh");
	quad = Mesh::GenerateQuad(1.0f);
	cube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");
	tree1 = Mesh::LoadFromMeshFile("PalmTreeDualBended.msh");
	tree2 = Mesh::LoadFromMeshFile("PalmTreeDualStraight.msh");
	tree3 = Mesh::LoadFromMeshFile("PalmTreeSingleBended.msh");
	tree4 = Mesh::LoadFromMeshFile("PalmTreeSingleStraight.msh");
	tree5 = Mesh::LoadFromMeshFile("PalmTreeTrio.msh");
	treeMeshes.emplace_back(tree1);
	treeMeshes.emplace_back(tree2);
	treeMeshes.emplace_back(tree3);
	treeMeshes.emplace_back(tree4);
	treeMeshes.emplace_back(tree5);
	rock1 = Mesh::LoadFromMeshFile("Rock_1_v2.msh");
	rock2 = Mesh::LoadFromMeshFile("Rock_2_v2.msh");
	rock3 = Mesh::LoadFromMeshFile("Rock_3_v2.msh");
	rock4 = Mesh::LoadFromMeshFile("Rock_4_v2.msh");
	rock5 = Mesh::LoadFromMeshFile("Rock_5_v2.msh");
	rock6 = Mesh::LoadFromMeshFile("Rock_6_v2.msh");
	rock7 = Mesh::LoadFromMeshFile("Rock_7_v2.msh");
	rock8 = Mesh::LoadFromMeshFile("Rock_8_v2.msh");
	rockMeshes.emplace_back(rock1);
	rockMeshes.emplace_back(rock2);
	rockMeshes.emplace_back(rock3);
	rockMeshes.emplace_back(rock4);
	rockMeshes.emplace_back(rock5);
	rockMeshes.emplace_back(rock6);
	rockMeshes.emplace_back(rock7);
	rockMeshes.emplace_back(rock8);
	firewood = Mesh::LoadFromMeshFile("Firewood02.msh");
	heightMap = new HeightMap(TEXTUREDIR"noise.png");
	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandTex = SOIL_load_OGL_texture(TEXTUREDIR"sand.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassTex = SOIL_load_OGL_texture(TEXTUREDIR"dryGrass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandBump = SOIL_load_OGL_texture(TEXTUREDIR"dryGrassBump.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassBump = SOIL_load_OGL_texture(TEXTUREDIR"SandBump.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthBump = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	treeTex = SOIL_load_OGL_texture(TEXTUREDIR"palmTree.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	treeBump = SOIL_load_OGL_texture(TEXTUREDIR"palmTreeDOT3.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	watermap = SOIL_load_OGL_texture(TEXTUREDIR"watermap.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	rockTex = SOIL_load_OGL_texture(TEXTUREDIR"Rock_a_v2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	rockBump1 = SOIL_load_OGL_texture(TEXTUREDIR"Rock_1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	EagleTex = SOIL_load_OGL_texture(TEXTUREDIR"Eagle.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	firewoodTex = SOIL_load_OGL_texture(TEXTUREDIR"woodtex.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	firewoodBump = SOIL_load_OGL_texture(TEXTUREDIR"woodbump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg", TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	if (!earthTex || !earthBump || !cubeMap || !waterTex || !sandTex || !grassTex || !sandBump || !grassBump || !treeTex || !treeBump || !watermap ) {
		return;
	}
	SetTextureRepeating(earthTex, true);
	SetTextureRepeating(grassTex, true);
	SetTextureRepeating(grassBump, true);
	SetTextureRepeating(sandTex, true);
	SetTextureRepeating(sandBump, true);
	SetTextureRepeating(earthBump, true);
	SetTextureRepeating(waterTex, true);
	SetTextureRepeating(treeTex, true);
	SetTextureRepeating(treeBump, true);
	SetTextureRepeating(rockTex, true);
	SetTextureRepeating(rockBump1, true);
	SetTextureRepeating(EagleTex, true);
	SetTextureRepeating(firewoodTex, true);
	SetTextureRepeating(firewoodBump, true);
	reflectShader = new Shader("reflectVertex.glsl", "reflectFragment.glsl", "", "displaceTCS.glsl", "displaceTES.glsl");
	skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader = new Shader("bumpvertex.glsl", "bumpfragment.glsl");
	shader = new Shader("SceneVertex.glsl", "SceneFragment.glsl");
	animShader = new Shader("SkinningVertex.glsl", "SceneFragment.glsl");
	shadowShader = new Shader("shadowVert.glsl", "shadowFrag.glsl");
	processShader = new Shader("bumpvertex.glsl", "processFrag.glsl");
	if (!reflectShader->LoadSuccess() || !skyboxShader->LoadSuccess() || !lightShader->LoadSuccess() || !shader->LoadSuccess() || !animShader->LoadSuccess() || !processShader->LoadSuccess() ||!shadowShader->LoadSuccess()) {
		return;
	}
	
	Vector3 heightmapSize = heightMap->GetHeightMapSize();
	for (int i = 0; i < 66049; i++) {
		if (heightMap->vertices[i].y > 1500 && heightMap->vertices[i].y < 2300) {
			aboveSeaLevels.push_back(heightMap->vertices[i]);
		}
		if (heightMap->vertices[i].y > 2300) {
			rockLayerPositions.push_back(heightMap->vertices[i]);
		}
	}
	root = new SceneNode();
	for (int i = 0; i < 300; ++i) {
		std::random_shuffle(treeMeshes.begin(), treeMeshes.end());
		SceneNode* s = new SceneNode();
		s->SetMesh(treeMeshes[0]);
		//s->SetMesh(cube)
		s->SetTexture(treeTex);
		s->SetBump(treeBump);
		Vector3 position = aboveSeaLevels[rand() % aboveSeaLevels.size()];
		s->SetTransform(Matrix4::Translation(position));
		s->SetTransform(s->GetTransform() * Matrix4::Rotation(rand() % 360, Vector3(0,1,0)));
		s->SetModelScale(Vector3(50, 50, 50));
		root->AddChild(s);
		sceneMeshes.emplace_back(s->GetMesh());
	}
	for (int i = 0; i < 150; ++i) {
		SceneNode* s = new SceneNode();
		std::random_shuffle(rockMeshes.begin(), rockMeshes.end());
		s->SetMesh(rockMeshes[0]);
		s->SetTexture(rockTex);;
		s->SetBump(rockBump1);
		Vector3 position = rockLayerPositions[rand() % rockLayerPositions.size()];
		s->SetTransform(Matrix4::Translation(position));
		s->SetTransform(s->GetTransform() * Matrix4::Rotation(rand() % 360, Vector3(0, 1, 0)));
		s->SetModelScale(Vector3(1, 1, 1) * (rand() % 100));
		root->AddChild(s);
		sceneMeshes.emplace_back(s->GetMesh());
	}
	Vector3 pos1 = heightMap->GetHeightMapSize() * Vector3(0.25, 3.0f, 0.25);
	float yaw1 = -90.0f;
	Vector3 pos2 = heightMap->GetHeightMapSize() * Vector3(0.25, 3.0f, 0.75);
	float yaw2 = 0.0f;
	Vector3 pos3 = heightMap->GetHeightMapSize() * Vector3(0.75, 3.0f, 0.75);
	float yaw3 = 90.0f;
	Vector3 pos4 = heightMap->GetHeightMapSize() * Vector3(0.75, 3.0f, 0.25);
	float yaw4 = 150.0f;
	Vector3 pos5 = heightMap->GetHeightMapSize() * Vector3(0.55, 2.0f, 0.45);
	float yaw5 = -135.0f;
	cameraPos.emplace_back(pos1);
	cameraPos.emplace_back(pos2);
	cameraPos.emplace_back(pos3);
	cameraPos.emplace_back(pos4);
	cameraPos.emplace_back(pos5);
	cameraYaws.emplace_back(yaw1);
	cameraYaws.emplace_back(yaw2);
	cameraYaws.emplace_back(yaw3);
	cameraYaws.emplace_back(yaw4);
	cameraYaws.emplace_back(yaw5);
	//camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5, 5.0f, 0.5f));
	camera = new Camera(-45.0f, -90.0f,cameraPos[0]);
	//light = new Light(heightmapSize * Vector3(0.25f, 2.5f, 0.5f), duskDawnLightValue, heightmapSize.x);
	lights.reserve(2);
	lights.emplace_back(heightmapSize* Vector3(0.0f, 3.0f, 0.5f), dayLightValue, heightmapSize.z*2, false);
	projMatrix = Matrix4::Perspective(1.0f, 150000.0f, (float)width / (float)height, 45.0f);
	
	
	SceneNode* s1 = new SceneNode();
	s1->SetMesh(firewood);
	s1->SetTexture(firewoodTex);
	s1->SetBump(firewoodBump);
	Vector3 position2 = Vector3(heightmapSize.x / 2, 2550, heightmapSize.z / 2);
	s1->SetTransform(Matrix4::Translation(position2));
	s1->SetModelScale(Vector3(75, 75, 75));
	root->AddChild(s1);
	sceneMeshes.emplace_back(s1->GetMesh());
	lights.emplace_back(Vector3(heightmapSize.x / 2, 2625, heightmapSize.z / 2), Vector4(1, 0.7, 0, 1), 300.0f, false);
	blueJayMesh = mesh;
	birdAnim = new  MeshAnimation("Eagle.anm");
	blueJayMaterial = new MeshMaterial("Eagle.mat");
	for (int i = 0; i < blueJayMesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = blueJayMaterial->GetMaterialForLayer(i);
		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		blueJayMatTextures.emplace_back(texID);
	}
	for (int i = 0; i < 5; i++) {
		SceneNode* s = new SceneNode();

		s->SetMesh(blueJayMesh);
		s->SetTexture(EagleTex);
		s->SetAnim(birdAnim);
		s1->AddChild(s);
		Vector3 position = Vector3(0, 400, 0);
		s->SetTransform(Matrix4::Translation(position));
		s->SetTransform(s->GetTransform() * Matrix4::Rotation(180, Vector3(0, 1, 0)));
		s->SetModelScale(Vector3(50, 50, 50));
		s->animated = true;
		
		birds.emplace_back(s);
		sceneMeshes.emplace_back(s->GetMesh());
	}
	bennuAnim = new  MeshAnimation("Bennu.anm");
	bennuMat = new MeshMaterial("Bennu.mat");
	for (int i = 0; i < bennuMesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = bennuMat->GetMaterialForLayer(i);
		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		bennuMatTextures.emplace_back(texID);
	}
	for (int i = 0; i < 5; i++) {
		SceneNode* s = new SceneNode();
		s->SetMesh(bennuMesh);
		s->SetAnim(bennuAnim);
		s1->AddChild(s);
		Vector3 position1 = Vector3(0, 400, 0);
		s->SetTransform(Matrix4::Translation(position1));
		s->SetModelScale(Vector3(50, 50, 50));
		s->animated = true;

		

		birds.emplace_back(s);
		sceneMeshes.emplace_back(s->GetMesh());
	}
	currentFrame = 0;
	frameTime = 0.0f;
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}
	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &processFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	sceneTime = 0.0f;
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	init = true;
}
Renderer::~Renderer(void)	{
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete root;
}

void Renderer::UpdateScene(float dt) {
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_T)) {
		freeCam = !freeCam;
	}
	if (freeCam) {
		camera->UpdateCamera(dt);
	}
	viewMatrix = camera->BuildViewMatrix();
	//viewMatrix = modelMatrix * viewMatrix * projMatrix;
	waterRotate += dt * 0.001f;
	waterCycle += dt * 0.001f;
	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % birdAnim->GetFrameCount();
		frameTime += 1.0f / birdAnim->GetFrameRate();
	}
	UpdateBirds(dt);
	//UpdateCampfire(dt);
	UpdateSun(dt);
	if (!freeCam) {
		UpdateCamera(dt);
	}
	root->Update(dt);
}
void Renderer::UpdateCamera(float dt) {
	
	Vector3 posChange;
	float yawChange;
	if (nextPos == 0) {
		
		posChange = Vector3(cameraPos[nextPos] - cameraPos[4]);
		yawChange = cameraYaws[4];
	}
	else {
		posChange = Vector3(cameraPos[nextPos] - cameraPos[nextPos - 1]);
		yawChange = cameraYaws[nextPos - 1];
	}
	if (abs(camera->GetPosition().x - cameraPos[nextPos].x) < 10 && abs(camera->GetPosition().z - cameraPos[nextPos].z) < 10) {
		if (nextPos != 4) {
			nextPos++;
		}
		else {
			nextPos = 0;
		}
	}
	else {
		if (nextPos == 4) {
			camera->SetPosition(camera->GetPosition() + ((posChange) / 3000.0f));
			camera->setYaw(yawChange);
		}
		else {
			camera->SetPosition(camera->GetPosition() + ((posChange) / 2000.0f));
			camera->setYaw(yawChange);
		}
	}
}
void Renderer::UpdateSun(float dt){
	Vector3 lightChange;

	if (lights[0].getColour().x > duskDawnLightValue.x) {
		lightChange = Vector3((dayLightValue.x - duskDawnLightValue.x) / 1000, (dayLightValue.y - duskDawnLightValue.y) / 1000, (dayLightValue.z - duskDawnLightValue.z) / 1000);
	}
	else if (lights[0].getColour().x > nightLightValue.x) {
		lightChange = Vector3((duskDawnLightValue.x - nightLightValue.x) / 1000, (duskDawnLightValue.y - nightLightValue.y) / 1000, (duskDawnLightValue.z - nightLightValue.z) / 10000);
	}
	if (abs(lights[0].getColour().x - dayLightValue.x) < 0.01) {
		rising = false;
	}
	else if (abs(lights[0].getColour().x - nightLightValue.x) < 0.01) {
		rising = true;
		lights[0].SetPosition(Vector3(heightMap->GetHeightMapSize().x * 0.0, heightMap->GetHeightMapSize().y * 3.0, heightMap->GetHeightMapSize().z * 0.5));
	}
	if (rising) {
		lights[0].SetColour(lights[0].getColour() + Vector4(lightChange.x, lightChange.y, lightChange.z, 0));
		//lights[0].SetPosition(lights[0].getPosition() + Vector3(0, 1, 0));
	}
	else {
		lights[0].SetColour(lights[0].getColour() - Vector4(lightChange.x, lightChange.y, lightChange.z, 0));
		//lights[0].SetPosition(lights[0].getPosition() - Vector3(0, 1, 0));
	}
}
void Renderer::UpdateBirds(float dt) {
	angle += 0.5 * dt;
	for (int i = 0; i < birds.size(); ++i) {
		Vector3 direction = Vector3((200 * i + 1)  * cos(angle + i), 400 , (200 * i + 1) * sin(angle + i ));
		birds[i]->SetTransform(Matrix4::Translation(direction));
		birds[i]->SetTransform(birds[i]->GetTransform() * Matrix4::Rotation(angle * 50 , Vector3(0,-1,0)));
	}
}
void Renderer::UpdateCampfire(float dt) {
 	lights[1].SetPosition(Vector3(lights[1].getPosition().x, 2725 + sin(dt) * 50, lights[1].getPosition().z));
}
void Renderer::RenderScene()	{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	DrawSkybox();
	DrawShadowScene();
	DrawHeightmap();
	DrawNode(root);
	DrawWater();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//DrawPostProcess();
	//PresentScene();
}
void Renderer::DrawPostProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(processShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(processShader->GetProgram(), "sceneTex"), 0);
	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 0);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
		quad->Draw();
		glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[1]);
		quad->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}
void Renderer::PresentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(lightShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "diffuseTex"), 0);
	quad->Draw();
}
void Renderer::DrawShadowNode(SceneNode* n) {
	if (n->GetMesh() && n->animated == false) {
		modelMatrix = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		UpdateShaderMatrices();
		n->Draw(*this);
	}
	else if (n->GetMesh() && n->animated == true) {
		modelMatrix = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		UpdateShaderMatrices();
		for (int i = 0; i < n->GetMesh()->GetSubMeshCount(); ++i) {
			n->GetMesh()->DrawSubMesh(i);
		}
	}
	for (vector <SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}
}
void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	BindShader(shadowShader);
	viewMatrix = Matrix4::BuildViewMatrix(lights[0].getPosition(), Vector3(0, 0, 0));
	projMatrix = Matrix4::Perspective(1, heightMap->GetHeightMapSize().z*5, 1,179);
	shadowMatrix = projMatrix * viewMatrix;
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();
	//heightMap->Draw();
	DrawShadowNode(root);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::DrawSkybox() {
	//glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glDepthMask(GL_FALSE);
	BindShader(skyboxShader);
	UpdateShaderMatrices();
	quad->Draw();
	glDepthMask(GL_TRUE);
}
void Renderer::DrawHeightmap() {
	BindShader(lightShader);
	SetShaderLight(lights);
	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex);
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthBump); 
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "sandTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, sandTex);
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "grassTex"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grassTex);
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "sandBump"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, sandBump);
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "grassBump"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, grassBump);
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "shadowTex"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	modelMatrix.ToIdentity();  
	textureMatrix.ToIdentity();
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 150000.0f, (float)width / (float)height, 45.0f);
	UpdateShaderMatrices();
	heightMap->Draw();
}
void Renderer::DrawWater() {
	BindShader(reflectShader);
	quad->type = GL_PATCHES;
	glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "cubeTex"), 2);
	glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "heightMap"), 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, watermap);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	Vector3 hSize = heightMap->GetHeightMapSize();
	modelMatrix = Matrix4::Translation(hSize * 0.5f) * Matrix4::Scale(hSize * 0.5f) * Matrix4::Rotation(-90, Vector3(1, 0, 0));
	textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(100, 100, 100)) * Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));
	UpdateShaderMatrices();

	quad->Draw();
	quad->type = GL_TRIANGLE_STRIP;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawNode(SceneNode* n) {
	BindShader(shader);
	if (n->GetMesh() && n->animated == false) {
		SetShaderLight(lights);
		UpdateShaderMatrices();
		Matrix4  model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, n->GetTexture());
		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, model.values);
		glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)& n->GetColour());
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "bumpTex"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, n->GetBump());
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "shadowTex"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, shadowTex);
		n->Draw(*this);
	}
	
	else if(n->GetMesh() && n->animated == true) {
		
		BindShader(animShader);
		SetShaderLight(lights);
		UpdateShaderMatrices();
		glUniform1i(glGetUniformLocation(animShader->GetProgram(), "diffuseTex"), 0);
		Matrix4  model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());

		glUniformMatrix4fv(glGetUniformLocation(animShader->GetProgram(), "modelMatrix"), 1, false, model.values);
		
		vector<Matrix4> frameMatrices;
		const Matrix4* invBindPose = n->GetMesh()->GetInverseBindPose();
		const Matrix4* frameData = n->GetAnim()->GetJointData(currentFrame);
		for (unsigned int i = 0; i < n->GetMesh()->GetJointCount(); ++i) {
			frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
		}
		int j = glGetUniformLocation(animShader->GetProgram(), "joints");
		glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());
		for (int i = 0; i < n->GetMesh()->GetSubMeshCount(); ++i) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, bennuMatTextures[i]);
			n->GetMesh()->DrawSubMesh(i);
		}
	}
	for (vector <SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}
}
