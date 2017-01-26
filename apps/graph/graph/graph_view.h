#ifndef GRAPH_GRAPH_VIEW_H
#define GRAPH_GRAPH_VIEW_H

#include <escher.h>
#include "../../curve_view.h"
#include "../../constant.h"
#include "../function_store.h"
#include "../../interactive_curve_view_range.h"

namespace Graph {

class GraphView : public CurveView {
public:
  GraphView(FunctionStore * functionStore, InteractiveCurveViewRange * graphRange, CurveViewCursor * cursor, ::BannerView * bannerView, View * cursorView);
  void drawRect(KDContext * ctx, KDRect rect) const override;
  void setContext(Context * context);
  Context * context() const;
private:
  char * label(Axis axis, int index) const override;
  float evaluateModelWithParameter(Model * expression, float abscissa) const override;
  char m_xLabels[k_maxNumberOfXLabels][Float::bufferSizeForFloatsWithPrecision(Constant::ShortNumberOfSignificantDigits)];
  char m_yLabels[k_maxNumberOfYLabels][Float::bufferSizeForFloatsWithPrecision(Constant::ShortNumberOfSignificantDigits)];
  FunctionStore * m_functionStore;
  Context * m_context;
};

}

#endif
