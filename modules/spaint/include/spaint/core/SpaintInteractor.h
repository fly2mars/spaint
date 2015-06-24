/**
 * spaint: SpaintInteractor.h
 */

#ifndef H_SPAINT_SPAINTINTERACTOR
#define H_SPAINT_SPAINTINTERACTOR

#include <boost/shared_ptr.hpp>

#include "SpaintModel.h"
#include "../markers/interface/VoxelMarker.h"
#include "../selectiontransformers/interface/SelectionTransformer.h"
#include "../selectors/Selector.h"

namespace spaint {

/**
 * \brief An instance of this class can be used to interact with the InfiniTAM scene in an spaint model.
 */
class SpaintInteractor
{
  //#################### TYPEDEFS ####################
private:
  typedef boost::shared_ptr<ORUtils::MemoryBlock<SpaintVoxel::PackedLabel> > PackedLabels_Ptr;
  typedef boost::shared_ptr<const ORUtils::MemoryBlock<SpaintVoxel::PackedLabel> > PackedLabels_CPtr;
  typedef boost::shared_ptr<const ITMLib::Objects::ITMRenderState> RenderState_CPtr;
  typedef Selector::Selection Selection;
  typedef boost::shared_ptr<const Selection> Selection_CPtr;
  typedef boost::shared_ptr<SelectionTransformer> SelectionTransformer_Ptr;
  typedef boost::shared_ptr<const VoxelMarker> VoxelMarker_CPtr;
public:
  typedef boost::shared_ptr<const SelectionTransformer> SelectionTransformer_CPtr;

  //#################### PRIVATE VARIABLES ####################
private:
  /** The spaint model. */
  SpaintModel_Ptr m_model;

  /** The selection transformer to use. */
  SelectionTransformer_Ptr m_selectionTransformer;

  /** The selector to use for selecting voxels in the scene. */
  Selector_Ptr m_selector;

  /** The semantic label to use for manually labelling the scene. */
  SpaintVoxel::Label m_semanticLabel;

  /** The voxel marker (used to apply semantic labels to voxels in the scene). */
  VoxelMarker_CPtr m_voxelMarker;

  //#################### CONSTRUCTORS ####################
public:
  /**
   * \brief Constructs an interactor that can be used to interact with the InfiniTAM scene.
   *
   * \param model The spaint model.
   */
  explicit SpaintInteractor(const SpaintModel_Ptr& model);

  //#################### PUBLIC MEMBER FUNCTIONS ####################
public:
  /**
   * \brief Clears the semantic labels of all the voxels in the scene.
   */
  void clear_labels();

  /**
   * \brief Gets the voxels in the scene (if any) that were selected the last time the current selector was updated.
   *
   * \return  The voxels in the scene (if any) that were selected the last time the current selector was updated.
   */
  Selection_CPtr get_selection() const;

  /**
   * \brief Gets the selection transformer that is currently being used.
   *
   * \return  The selection transformer that is currently being used.
   */
  SelectionTransformer_CPtr get_selection_transformer() const;

  /**
   * \brief Gets the selector that is currently being used to select voxels in the scene.
   *
   * \return  The selector that is currently being used to select voxels in the scene.
   */
  Selector_CPtr get_selector() const;

  /**
   * \brief Gets the semantic label that is being used for manually labelling the scene.
   *
   * \return  The semantic label that is being used for manually labelling the scene.
   */
  SpaintVoxel::Label get_semantic_label() const;

  /**
   * \brief Marks a selection of voxels in the scene with the specified semantic label.
   *
   * \param selection The selection of voxels.
   * \param label     The semantic label with which to mark the voxels.
   * \param oldLabels An optional memory block into which to store the old semantic labels of the voxels being marked.
   */
  void mark_voxels(const Selection_CPtr& selection, SpaintVoxel::PackedLabel label, const PackedLabels_Ptr& oldLabels = PackedLabels_Ptr());

  /**
   * \brief Marks a selection of voxels in the scene with the specified semantic labels.
   *
   * \param selection The selection of voxels.
   * \param labels    The semantic labels with which to mark the voxels (one per voxel).
   */
  void mark_voxels(const Selection_CPtr& selection, const PackedLabels_CPtr& labels);

  /**
   * \brief Gets whether or not the current selector is active.
   *
   * \return  true, if the current selector is active, or false otherwise.
   */
  bool selector_is_active() const;

  /**
   * \brief Sets the semantic label to use for manually labelling the scene.
   *
   * \param semanticLabel The semantic label to use for manually labelling the scene.
   */
  void set_semantic_label(SpaintVoxel::Label semanticLabel);

  /**
   * \brief Allows the user to change selector or update the current selector.
   *
   * \param inputState      The current input state.
   * \param renderState     The render state corresponding to the camera from which the scene is being viewed.
   * \param renderingInMono A flag indicating whether or not the scene is currently being rendered in mono.
   */
  void update_selector(const InputState& inputState, const RenderState_CPtr& renderState, bool renderingInMono);
};

//#################### TYPEDEFS ####################

typedef boost::shared_ptr<SpaintInteractor> SpaintInteractor_Ptr;
typedef boost::shared_ptr<const SpaintInteractor> SpaintInteractor_CPtr;

}

#endif
