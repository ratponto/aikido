#include <aikido/statespace/SO3StateSpace.hpp>
#include <dart/math/Geometry.h>
#include <iostream>

namespace aikido {
namespace statespace {

//=============================================================================
SO3StateSpace::SO3StateSpace::State::State()
  : mValue(1., 0., 0., 0.)
{
}

//=============================================================================
SO3StateSpace::SO3StateSpace::State::State(const Quaternion& _quaternion)
  : mValue(_quaternion)
{
  // TODO: Check if normalized.
}

//=============================================================================
auto SO3StateSpace::State::getQuaternion() const -> const Quaternion&
{
  return mValue;
}

//=============================================================================
void SO3StateSpace::State::setQuaternion(const Quaternion& _quaternion)
{
  // TODO: Check if normalized.
  mValue = _quaternion;
}

//=============================================================================
auto SO3StateSpace::createState() const -> ScopedState
{
  return ScopedState(this);
}

//=============================================================================
auto SO3StateSpace::getQuaternion(const State* _state) const
  -> const Quaternion&
{
  return _state->mValue;
}

//=============================================================================
void SO3StateSpace::setQuaternion(
  State* _state, const Quaternion& _quaternion) const
{
  _state->mValue = _quaternion;
}

//=============================================================================
size_t SO3StateSpace::getStateSizeInBytes() const
{
  return sizeof(State);
}

//=============================================================================
StateSpace::State* SO3StateSpace::allocateStateInBuffer(void* _buffer) const
{
  return new (_buffer) State;
}

//=============================================================================
void SO3StateSpace::freeStateInBuffer(StateSpace::State* _state) const
{
  static_cast<State*>(_state)->~State();
}

//=============================================================================
auto SO3StateSpace::createSampleableConstraint(
  std::unique_ptr<util::RNG> _rng) const -> SampleableConstraintPtr
{
  throw std::runtime_error(
    "SO3StateSpace::createSampleableConstraint is not implemented.");
}

//=============================================================================
void SO3StateSpace::compose(
  const StateSpace::State* _state1, const StateSpace::State* _state2,
  StateSpace::State* _out) const
{
  auto state1 = static_cast<const State*>(_state1);
  auto state2 = static_cast<const State*>(_state2);
  auto out = static_cast<State*>(_out);

  out->mValue = state1->mValue * state2->mValue;
}

//=============================================================================
void SO3StateSpace::expMap(
  const Eigen::VectorXd& _tangent, StateSpace::State* _out) const
{
  auto out = static_cast<State*>(_out);

  // TODO: Skip these checks in release mode.
  if (_tangent.rows() != 3)
  {
    std::stringstream msg;
    msg << "_tangent has incorrect size: expected 3"
        << ", got " << _tangent.rows() << ".\n";
    throw std::runtime_error(msg.str());
  }

  Eigen::Vector6d tangent(Eigen::Vector6d::Zero());
  tangent.topRows(3) = _tangent;

  Eigen::Isometry3d transform = dart::math::expMap(tangent);
  out->setQuaternion(Quaternion(transform.rotation()));
}

//=============================================================================
int SO3StateSpace::getDimension() const
{
  return 3;
}

} // namespace statespace
} // namespace aikido