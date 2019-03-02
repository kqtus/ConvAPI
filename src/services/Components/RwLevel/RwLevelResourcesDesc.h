#pragma once
#include <vector>
#include "../Level/ILevelResourceDesc.h"

class CRwLevelResourcesDesc : public ILevelResourcesDesc
{
public:
	using TWStrVec = std::vector<std::wstring>;

	/** ILevelResourcesDesc interface **/
	virtual bool InitFromDir(const std::wstring& dir_name, const std::string& level_name) override;

	virtual CRwLevelResourcesDesc* Clone() const override;

	virtual std::string GetName() const override;
	/** End of ILevelResourcesDesc interface **/

	virtual void GetUsedArchivePaths(TWStrVec& out_paths) const;
	virtual void GetItemPlacementFilePaths(TWStrVec& out_paths) const;
	virtual void GetItemDefinitionFilePaths(TWStrVec& out_paths) const;

protected:
	std::string m_LevelName;
	TWStrVec m_UsedImgPaths;
	TWStrVec m_IplPaths;
	TWStrVec m_IdePaths;
};