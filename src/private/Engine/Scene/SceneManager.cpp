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

	EditorCamera* editorCamera = new EditorCamera("Editor camera");

	Component* sampleComponent = new Mesh(&sampleObj->transform);
	sampleComponent->LoadModel("f16.fbx");
	sampleObj->AddComponent(sampleComponent);
	this->actualScene->AddObject(editorCamera);
	this->actualScene->AddObject(sampleObj);
	this->actualScene->SetActualCamera("Editor camera");
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
	if (!this->actualScene)
		this->dbg->Throw("[ERROR] No active scenes found");

	return this->actualScene;
}

void SceneManager::Update() {
	this->actualScene->Update();
}