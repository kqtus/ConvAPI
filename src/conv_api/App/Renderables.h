#include "formats/rw/rwcore.h"
#include "IRenderable.h"

class CRwModel : public rw::core::clump, public IRenderable
{
public:
	virtual int GetVertexCount() const override;
	virtual TVertices GetVertices() const override;

	virtual int GetIndexCount() const override;
	virtual TIndices GetIndices() const override;
};