#ifndef DISCORD_H
#define DISCORD_H

#include "discord_rpc.h"
#include <escher/app.h>
#include <time.h>
#include <stdint.h>
#include <cstring>

namespace discord {
  static const char* applicationID = "740266421115814051";
  class OmegaDiscordRPC {
    public :
      OmegaDiscordRPC() :
        m_startTime(time(0))
        {
            DiscordEventHandlers handlers;
            memset(&handlers, 0, sizeof(handlers));
            Discord_Initialize(applicationID, &handlers, 1, NULL);
        }
        void deinit() {
          Discord_Shutdown();
        }
        void setApp(App::Descriptor * descriptor);
    private :
      int64_t m_startTime;
  };
}

#endif
