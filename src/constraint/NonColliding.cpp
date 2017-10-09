#include <aikido/constraint/NonColliding.hpp>

namespace aikido {
namespace constraint {

//==============================================================================
NonColliding::NonColliding(
    statespace::dart::MetaSkeletonStateSpacePtr _statespace,
    std::shared_ptr<dart::collision::CollisionDetector> _collisionDetector,
    dart::collision::CollisionOption _collisionOptions)
  : statespace(std::move(_statespace))
  , collisionDetector(std::move(_collisionDetector))
  , collisionOptions(std::move(_collisionOptions))
{
  if (!statespace)
    throw std::invalid_argument("_statespace is nullptr.");

  if (!collisionDetector)
    throw std::invalid_argument("_collisionDetector is nullptr.");
}

//==============================================================================
statespace::StateSpacePtr NonColliding::getStateSpace() const
{
  return statespace;
}

//==============================================================================
bool NonColliding::isSatisfied(
    const aikido::statespace::StateSpace::State* _state, bool _givenState) const
{
  auto skelStatePtr = static_cast<const aikido::statespace::dart::
                                      MetaSkeletonStateSpace::State*>(_state);
  statespace->setState(skelStatePtr);

  bool collision = false;
  dart::collision::CollisionResult collisionResult;
  for (auto groups : groupsToPairwiseCheck)
  {
    collision = collisionDetector->collide(
        groups.first.get(),
        groups.second.get(),
        collisionOptions,
        &collisionResult);
    if (collision)
    {
      if (_givenState)
      {
        auto selfCollidingBodies = collisionResult.getCollidingBodyNodes();
        for (const auto& elem : selfCollidingBodies)
        {
          std::cout << elem->getName() << " ";
        }
        std::cout << "in collision" << std::endl;
      }
      return false;
    }
  }

  for (auto group : groupsToSelfCheck)
  {
    collision = collisionDetector->collide(
        group.get(), collisionOptions, &collisionResult);
    if (collision)
    {
      if (_givenState)
      {
        auto selfCollidingBodies = collisionResult.getCollidingBodyNodes();
        for (const auto& elem : selfCollidingBodies)
        {
          std::cout << elem->getName() << " ";
        }
        std::cout << "in collision" << std::endl;
      }
      return false;
    }
  }
  return true;
}

//==============================================================================
void NonColliding::addPairwiseCheck(
    std::shared_ptr<dart::collision::CollisionGroup> _group1,
    std::shared_ptr<dart::collision::CollisionGroup> _group2)
{
  groupsToPairwiseCheck.emplace_back(std::move(_group1), std::move(_group2));
}

//==============================================================================
void NonColliding::addSelfCheck(
    std::shared_ptr<dart::collision::CollisionGroup> _group)
{
  groupsToSelfCheck.emplace_back(std::move(_group));
}

} // namespace constraint
} // namespace aikido
