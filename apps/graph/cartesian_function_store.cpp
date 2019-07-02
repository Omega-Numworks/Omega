#include "cartesian_function_store.h"
extern "C" {
#include <assert.h>
#include <stddef.h>
}
#include <ion.h>

using namespace Shared;

namespace Graph {

Ion::Storage::Record::ErrorStatus CartesianFunctionStore::addEmptyModel() {
  Ion::Storage::Record::ErrorStatus error;
  CartesianFunction newModel = CartesianFunction::NewModel(&error);
  return error;
}

void CartesianFunctionStore::setMemoizedModelAtIndex(int cacheIndex, Ion::Storage::Record record) const {
  assert(cacheIndex >= 0 && cacheIndex < maxNumberOfMemoizedModels());
  m_functions[cacheIndex] = CartesianFunction(record);
}

ExpressionModelHandle * CartesianFunctionStore::memoizedModelAtIndex(int cacheIndex) const {
  assert(cacheIndex >= 0 && cacheIndex < maxNumberOfMemoizedModels());
  return &m_functions[cacheIndex];
}

}
