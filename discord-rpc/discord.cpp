#include "discord.h"
#include <apps/i18n.h>
#include <stdio.h>

namespace discord {

  struct I18NIcons {
    I18n::Message message;
    const char* iconName;
  };
  
  static I18NIcons IconsList[] = {
    {I18n::Message::AtomApp,       "atom"},
    {I18n::Message::CalculApp,     "calculation"},
    {I18n::Message::CodeApp,       "code"},
    {I18n::Message::FunctionApp,   "graph"},
    {I18n::Message::ProbaApp,      "probability"},
    {I18n::Message::RegressionApp, "regression"},
    {I18n::Message::RpnApp,        "rpn"},
    {I18n::Message::SequenceApp,   "sequence"},
    {I18n::Message::SettingsApp,   "settings"},
    {I18n::Message::SolverApp,     "solver"},
    {I18n::Message::StatsApp,      "stats"},
  };

  void OmegaDiscordRPC::setApp(App::Descriptor * descriptor) {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = I18n::translate(descriptor->name());
    char buffer[90];
    sprintf(buffer, "E%s-O%s", Ion::softwareVersion(), Ion::omegaVersion());
    discordPresence.details = buffer;
    discordPresence.startTimestamp = m_startTime;
    discordPresence.smallImageKey= "omega";
    discordPresence.largeImageKey= "omega";
    for(I18NIcons p : IconsList) {
      if(p.message == descriptor->name()){
        discordPresence.largeImageKey = p.iconName;
      }
    };
    Discord_UpdatePresence(&discordPresence);
  }
}
