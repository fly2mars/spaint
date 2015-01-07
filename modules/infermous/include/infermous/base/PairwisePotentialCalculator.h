/**
 * infermous: PairwisePotentialCalculator.h
 */

#ifndef H_INFERMOUS_PAIRWISEPOTENTIALCALCULATOR
#define H_INFERMOUS_PAIRWISEPOTENTIALCALCULATOR

#include <boost/shared_ptr.hpp>

namespace infermous {

/**
 * \brief An instance of a class derived from an instantiation of this class template can be used to calculate pairwise potentials for a 2D CRF.
 */
template <typename Label>
class PairwisePotentialCalculator
{
  //#################### DESTRUCTOR ####################
public:
  /**
   * \brief Destroys the pairwise potential calculator.
   */
  virtual ~PairwisePotentialCalculator() {}

  //#################### PUBLIC ABSTRACT MEMBER FUNCTIONS ####################
public:
  /**
   * \brief TODO
   */
  virtual float calculate_potential(const Label& l1, const Label& l2) const = 0;
};

//#################### TYPEDEFS ####################

template <typename Label> using PairwisePotentialCalculator_Ptr = boost::shared_ptr<PairwisePotentialCalculator<Label> >;
template <typename Label> using PairwisePotentialCalculator_CPtr = boost::shared_ptr<const PairwisePotentialCalculator<Label> >;

}

#endif