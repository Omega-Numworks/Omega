#include <escher.h>

namespace Code {

  class toolboxIonKeys : public ViewController {
    public :
      toolboxIonKeys();
      View * view() override;
      bool handleEvent(Ion::Events::Event e) override;
      void setSender(InputEventHandler * sender) { m_view.setSender(sender); }
    private :
      class toolboxIonView : public View {
        public :
          toolboxIonView();
          void drawRect(KDContext * ctx, KDRect rect) const override;
          void setSender(InputEventHandler * sender) { m_sender = sender; }
        private :
          InputEventHandler * m_sender;
      };
      toolboxIonView m_view;
  };

}
