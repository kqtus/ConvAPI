#include "Scene.h"
#include <algorithm>

#include "IRenderable.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	for (auto& renderable : m_Renderables)
		delete renderable;
}

int CScene::GetObjectsCount()
{
	return m_Renderables.size();
}

bool CScene::AddObject(IRenderable* obj)
{
	if (std::find(m_Renderables.begin(), m_Renderables.end(), obj) == m_Renderables.end())
	{
		m_Renderables.push_back(obj);
		return true;
	}
	
	return false;
}

void CScene::GetRenderables(TRenderables& renderables) const
{
	renderables = m_Renderables;
}

std::string CScene::GetName() const
{
	return "Scene";
}
