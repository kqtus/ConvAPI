#pragma once
#include <vector>
#include "../Renderer/Renderer.h"

#include "IRenderSource.h"

class IScene : public IRenderSource
{
public:
	virtual int GetObjectsCount() = 0;
	virtual bool AddObject(IRenderable* obj) = 0;
};

class CScene : public IScene
{
public:
	CScene();
	virtual ~CScene();

	virtual int GetObjectsCount() override;
	virtual bool AddObject(IRenderable* obj) override;

	virtual void GetRenderables(TRenderables& renderables) const override;
	virtual std::string GetName() const override;

protected:
	TRenderables m_Renderables;
};