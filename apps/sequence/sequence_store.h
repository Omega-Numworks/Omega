#ifndef SEQUENCE_SEQUENCE_STORE_H
#define SEQUENCE_SEQUENCE_STORE_H

#include "../shared/function_store.h"
#include "../shared/global_context.h"
#include "sequence.h"
#include <stdint.h>
#include <escher.h>

namespace Sequence {

class SequenceStore : public Shared::FunctionStore {
public:
  using Shared::FunctionStore::FunctionStore;
  CodePoint symbol() const override { return Sequence::Symbol(); }
  CodePoint unknownSymbol() const override { return UCodePointUnknownN; }
  /* Sequence Store hold all its Sequences in an array. The Sequence pointers
   * return by modelForRecord are therefore non-expirable. We choose to return
   * Sequence * instead of ExpiringPointer<Sequence>. */
  Sequence * modelForRecord(Ion::Storage::Record record) const { return static_cast<Sequence *>(privateModelForRecord(record)); }
  Ion::Storage::Record::ErrorStatus addEmptyModel() override;
  /* WARNING: after calling removeModel or removeAll, the sequence context
   * need to invalidate its cache as the sequences evaluations might have
   * changed */
  int maxNumberOfModels() const override { return MaxNumberOfSequences; }

  static const char * firstAvailableName(int * nameIndex = nullptr);
  static constexpr const char * k_sequenceNames[MaxNumberOfSequences] = {
    "u", "v"//, "w"
  };

private:
  const char * modelExtension() const override { return Ion::Storage::seqExtension; }
  /* We don't really use model memoization as the number of Sequence is limited
   * and we keep enough Sequences to store them all. */
  void setMemoizedModelAtIndex(int cacheIndex, Ion::Storage::Record record) const override;
  Shared::ExpressionModelHandle * memoizedModelAtIndex(int cacheIndex) const override;
  mutable Sequence m_sequences[MaxNumberOfSequences];
};

}

#endif
