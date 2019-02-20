#pragma once

class IRenderer;

class IApp
{
public:
	virtual bool Init() = 0;
	virtual void OnResize() = 0;
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual IRenderer* GetRenderer() const = 0;
};