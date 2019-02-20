#include "Renderer.h"
#include <algorithm>

#include "IRenderable.h"
#include "IRenderSource.h"

bool CRenderer::AddRenderSource(IRenderSource* render_src)
{
	if (std::find(m_RenderSources.begin(), m_RenderSources.end(), render_src) == m_RenderSources.end())
	{
		m_RenderSources.push_back(render_src);
		return true;
	}
	return false;
}

bool CRenderer::RemoveRenderSource(IRenderSource* render_src)
{
	return std::remove_if(m_RenderSources.begin(), m_RenderSources.end(), [&render_src](IRenderSource* lhs)
	{
		return lhs == render_src;
	}) != m_RenderSources.end();
}

void CRenderer::SetWindowWidth(unsigned int width)
{
	m_ClientWidth = width;
	OnWindowWidthSet();
}

void CRenderer::SetWindowHeight(unsigned int height)
{
	m_ClientHeight = height;
	OnWindowHeightSet();
}

unsigned int CRenderer::GetWindowWidth() const
{
	return m_ClientWidth;
}

unsigned int CRenderer::GetWindowHeight() const
{
	return m_ClientHeight;
}

bool CRenderer::BuildGeomBuffers()
{
	IRenderSource::TRenderables renderables;
	for (auto& render_src : m_RenderSources)
	{
		render_src->GetRenderables(renderables);
	}

	bool res = true;
	for (auto& renderable : renderables)
	{
		res &= BuildGeomBuffers(renderable);
	}

	return res;
}