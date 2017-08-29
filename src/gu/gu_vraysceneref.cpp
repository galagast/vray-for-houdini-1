#include "gu_vraysceneref.h"

#include "vfh_log.h"

#include <GU/GU_PackedFactory.h>
#include <UT/UT_MemoryCounter.h>

using namespace VRayForHoudini;

class VRaySceneFactory
	: public GU_PackedFactory
{
public:
	static VRaySceneFactory &getInstance() {
		static VRaySceneFactory theFactory;
		return theFactory;
	}

	GU_PackedImpl* create() const VRAY_OVERRIDE {
		return new VRaySceneRef();
	}

private:
	VRaySceneFactory();

	VUTILS_DISABLE_COPY(VRaySceneFactory);
};

VRaySceneFactory::VRaySceneFactory() :
	GU_PackedFactory("VRaySceneRef", "VRaySceneRef")
{
	// TODO: Register intrinsic
}

VRaySceneRef::VRaySceneRef():
	GU_PackedImpl(),
	m_detail(),
	m_options(),
	m_dirty(false)
{}

VRaySceneRef::VRaySceneRef(const VRaySceneRef &src):
	GU_PackedImpl(src),
	m_detail(),
	m_options(),
	m_dirty(false)
{
	updateFrom(src.m_options);
}

VRaySceneRef::~VRaySceneRef()
{
	clearDetail();
}

GU_PackedFactory *VRaySceneRef::getFactory() const
{
	return &VRaySceneFactory::getInstance();
}

GU_PackedImpl *VRaySceneRef::copy() const
{
	return new VRaySceneRef(*this);
}

bool VRaySceneRef::isValid() const
{
	return m_detail.isValid();
}

void VRaySceneRef::clearData()
{
	// This method is called when primitives are "stashed" during the cooking
	// process.  However, primitives are typically immediately "unstashed" or
	// they are deleted if the primitives aren't recreated after the fact.
	// We can just leave our data.
}

bool VRaySceneRef::load(const UT_Options &options, const GA_LoadMap &map)
{
	return updateFrom(options);
}

void VRaySceneRef::update(const UT_Options &options)
{
	updateFrom(options);
}

bool VRaySceneRef::save(UT_Options &options, const GA_SaveMap &map) const
{
	options.merge(m_options);
	return true;
}

bool VRaySceneRef::getBounds(UT_BoundingBox &box) const
{
	return true;
}

bool VRaySceneRef::getRenderingBounds(UT_BoundingBox &box) const
{
	return getBounds(box);
}

void VRaySceneRef::getVelocityRange(UT_Vector3 &min, UT_Vector3 &max) const
{
	// No velocity attribute on geometry
	min = 0;
	max = 0;
}

void VRaySceneRef::getWidthRange(fpreal &wmin, fpreal &wmax) const
{
	// Width is only important for curves/points.
	wmin = wmax = 0;
}

bool VRaySceneRef::unpack(GU_Detail &destgdp) const
{
	// This may allocate geometry for the primitive
	GU_DetailHandleAutoReadLock gdl(getPackedDetail());
	if (NOT(gdl.isValid())) {
		return false;
	}

	return unpackToDetail(destgdp, gdl.getGdp());
}

int64 VRaySceneRef::getMemoryUsage(bool inclusive) const
{
	int64 mem = inclusive ? sizeof(*this) : 0;
	// Don't count the (shared) GU_Detail, since that will greatly
	// over-estimate the overall memory usage.
	mem += m_detail.getMemoryUsage(false);
	return mem;
}

void VRaySceneRef::countMemory(UT_MemoryCounter &counter, bool inclusive) const
{
	if (counter.mustCountUnshared()) {
		size_t mem = (inclusive) ? sizeof(*this) : 0;
		mem += m_detail.getMemoryUsage(false);
		UT_MEMORY_DEBUG_LOG(theFactory->name(), int64(mem));
		counter.countUnshared(mem);
	}

	// The UT_MemoryCounter interface needs to be enhanced to efficiently count
	// shared memory for details. Skip this for now.
#if 0
	if (detail().isValid())
	{
		GU_DetailHandleAutoReadLock gdh(detail());
		gdh.getGdp()->countMemory(counter, true);
	}
#endif
}

void VRaySceneRef::clearDetail()
{
	m_detail = GU_ConstDetailHandle();
}

bool VRaySceneRef::updateFrom(const UT_Options &options)
{
	if (m_options == options) {
		return false;
	}

	m_options = options;
	m_dirty = true;
	// Notify base primitive that topology has changed
	topologyDirty();

	return true;
}
