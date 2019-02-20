#pragma once

class IRenderSource;

class IRenderer
{
public:
	virtual bool Init() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	virtual bool AddRenderSource(IRenderSource* render_src) = 0;
	virtual bool RemoveRenderSource(IRenderSource* render_src) = 0;

	virtual void Move(float dx, float dy) = 0;
	virtual void Rotate(float dx, float dy) = 0;

	virtual void OnResize() = 0;
	virtual void SetWindowWidth(UINT width) = 0;
	virtual void SetWindowHeight(UINT height) = 0;

	virtual bool BuildGeomBuffers() = 0;
};
