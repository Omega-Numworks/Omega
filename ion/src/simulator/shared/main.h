#ifndef ION_SIMULATOR_MAIN_H
#define ION_SIMULATOR_MAIN_H

#ifdef RPC
#include <escher/app.h>
#endif

namespace Ion {
namespace Simulator {
namespace Main {

void init();
void quit();

void setNeedsRefresh();
void refresh();
void relayout();
#ifdef RPC
void setApp(App::Descriptor * descriptor);
#endif

}
}
}

#endif
