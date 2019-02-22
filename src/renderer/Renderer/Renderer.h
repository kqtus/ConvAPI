#pragma once
#include "IRenderer.h"
#include <vector>

class IRenderable;
class IRenderSource;

class CRenderer : public IRenderer
{
public:
	CRenderer() { }
	virtual ~CRenderer() { }

	virtual bool AddRenderSource(IRenderSource* render_src) override;
	virtual bool RemoveRenderSource(IRenderSource* render_src) override;

	virtual void SetWindowWidth(unsigned int width) override;
	virtual void SetWindowHeight(unsigned int height) override;

	virtual void Move(float dx, float dy) override { };
	virtual void Rotate(float dx, float dy) override { };

	virtual void OnResize() override { };

	unsigned int GetWindowWidth() const;
	unsigned int GetWindowHeight() const;

protected:
	virtual void OnWindowWidthSet() { }
	virtual void OnWindowHeightSet() { }

	unsigned int m_ClientWidth = 640;
	unsigned int m_ClientHeight = 480;

	std::vector<IRenderSource*> m_RenderSources;
};