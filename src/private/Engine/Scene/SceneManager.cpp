#include "Engine/Scene/SceneManager.h"
#include "Engine/Core.h"

SceneManager::SceneManager() {
	this->error = Error::GetInstance();
	this->core = Core::GetInstance();

	/* Sample scene */
	this->actualScene = new Scene("SampleScene");

	GameObject* sampleObj = new GameObject("TestObj");
	ConstantBuffer constantBuffer;
	constantBuffer.View = XMMatrixTranspose(XMMatrixLookToLH(XMVectorSet(0.f, 0.f, -2.f, 0.f), XMVectorSet(0.f, 0.f, 1.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	constantBuffer.Projection = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), (float)this->core->width / (float)this->core->height, .1f, 300.f));
	
	Component* sampleComponent = new Mesh(&sampleObj->transform, constantBuffer);
	sampleComponent->LoadModel("f16.obj");
	sampleObj->AddComponent(sampleComponent);

	this->actualScene->objs.push_back(sampleObj);
	this->actualScene->PreRender();
}

void SceneManager::AddScene(Scene* scene) {
	this->scenes[scene->name] = scene;
}

void SceneManager::LoadScene(std::string name) {
	if (this->scenes.count(name) <= 0) {
		error->Throw("[ERROR] Scene not found.");
		std::cout << "[ERROR] Scene not found." << std::endl;
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