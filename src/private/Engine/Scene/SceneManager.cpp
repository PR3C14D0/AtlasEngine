#include "Engine/Scene/SceneManager.h"
#include "Engine/Core.h"

SceneManager::SceneManager() {
	this->dbg = Debugger::GetInstance();
	this->core = Core::GetInstance();

	/* Sample scene */
	this->actualScene = new Scene("SampleScene");

	GameObject* sampleObj = new GameObject("TestObj");
	ConstantBuffer* constantBuffer = ConstantBuffer::GetInstance();
	constantBuffer->Projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), (float)this->core->width / (float)this->core->height, .1f, 300.f));
	
	Camera* camera = new Camera("Camera");

	Component* sampleComponent = new Mesh(&sampleObj->transform);
	sampleComponent->LoadModel("f16.obj");
	sampleObj->AddComponent(sampleComponent);

	this->actualScene->AddObject(camera);
	this->actualScene->AddObject(sampleObj);
	this->actualScene->PreRender();
}

void SceneManager::AddScene(Scene* scene) {
	this->scenes[scene->name] = scene;
}

void SceneManager::LoadScene(std::string name) {
	if (this->scenes.count(name) <= 0) {
		dbg->Throw("[ERROR] Scene not found.");
		return;
	}
	this->actualScene = this->scenes[name];
	return;
}

Scene* SceneManager::GetActualScene() {
	return this->actualScene;
}

void SceneManager::Update() {
	this->actualScene->Update();
}