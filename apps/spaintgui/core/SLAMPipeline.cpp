/**
 * spaintgui: SLAMPipeline.cpp
 * Copyright (c) Torr Vision Group, University of Oxford, 2016. All rights reserved.
 */

#include "SLAMPipeline.h"
using namespace spaint;

#ifdef WITH_OPENCV
#include <spaint/ocv/OpenCVUtil.h>
#endif

#include <spaint/pipelinecomponents/SLAMComponent.h>

//#################### CONSTRUCTORS ####################

SLAMPipeline::SLAMPipeline(const Settings_Ptr& settings,
    const std::string& resourcesDir,
    const CompositeImageSourceEngine_Ptr& imageSourceEngine,
    const std::string& trackerConfig,
    spaint::SLAMComponent::MappingMode mappingMode,
    spaint::SLAMComponent::TrackingMode trackingMode)
: MultiScenePipeline("slam", settings, resourcesDir, 2)
// Need to use 2 labels to avoid crash.
// TODO fix it
// using 0 crashes for an invalid MemoryBlock allocation
// using 1 -> Application.cpp#715 needs at least 2 valid labels
{
  const std::string sceneID = Model::get_world_scene_id();
  m_slamComponents[sceneID].reset(new SLAMComponent(
      m_model,
      sceneID,
      imageSourceEngine,
      trackerConfig,
      mappingMode,
      trackingMode
  ));
}

//#################### PUBLIC MEMBER FUNCTIONS ####################

void SLAMPipeline::set_mode(Mode mode)
{
  // The only supported mode
  m_mode = MODE_NORMAL;
}
