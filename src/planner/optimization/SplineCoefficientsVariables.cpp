#include "aikido/planner/optimization/SplineCoefficientsVariables.hpp"

namespace aikido {
namespace planner {
namespace optimization {

//==============================================================================
std::size_t SplineCoefficientsVariables::getDimension() const
{
  std::size_t dim = 0u;

  for (const auto& segment : mSegments)
  {
    // This should be guaranteed by Spline::addSegment(). Here we just check
    // this for sure.
    assert(
        static_cast<std::size_t>(segment.mCoefficients.rows())
        == mStateSpace->getDimension());

    dim += segment.mCoefficients.cols();
  }

  dim *= mStateSpace->getDimension();

  return dim;
}

//==============================================================================
void SplineCoefficientsVariables::setVariables(const Eigen::VectorXd& variables)
{
  // TODO(JS): Check the dimension of variables

  int index = 0;
  for (auto& segment : mSegments)
  {
    const auto rows = mStateSpace->getDimension();
    const auto cols = segment.mCoefficients.cols();
    const auto numLocalVariables = rows * cols;

    segment.mCoefficients = Eigen::Map<const Eigen::MatrixXd>(
        variables.segment(index, numLocalVariables).data(), rows, cols);

    index += numLocalVariables;
  }

  assert(static_cast<std::size_t>(index) == getDimension());
}

//==============================================================================
void SplineCoefficientsVariables::getVariables(Eigen::VectorXd& variables) const
{
  variables.resize(getDimension());

  int index = 0;
  for (const auto& segment : mSegments)
  {
    const auto rows = mStateSpace->getDimension();
    const auto cols = segment.mCoefficients.cols();
    const auto numLocalVariables = rows * cols;

    variables.segment(index, numLocalVariables)
        = Eigen::Map<const Eigen::VectorXd>(
            segment.mCoefficients.data(), numLocalVariables);

    index += numLocalVariables;
  }

  assert(static_cast<std::size_t>(index) == getDimension());
}

} // namespace optimization
} // namespace planner
} // namespace aikido