#ifndef AIKIDO_SAMPLEABLE_H_
#define AIKIDO_SAMPLEABLE_H_

#include <limits>
#include <memory>
#include <boost/optional.hpp>

#include "../util/RNG.hpp"

namespace aikido {
namespace sampleable {

template <class T> class SampleGenerator;
template <class T> class SampleableConstraint;

template <class T>
class SampleableConstraint {
public:
  virtual std::unique_ptr<SampleGenerator<T>> createSampleGenerator() const = 0;
};

template <class T>
class SampleGenerator {
public:
  /// Returns T. If all samples are exhausted, returns empty optional.
  virtual boost::optional<T> sample() = 0;
  virtual int getNumSamples() const = 0;
  virtual bool canSample() const = 0;
  static constexpr int NO_LIMIT = std::numeric_limits<int>::max();
};


} // namespace sampleable
} // namespace aikido

#endif // AIKIDO_SAMPLEABLE_H_

