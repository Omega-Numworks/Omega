#include "symbol_controller.h"
#include <assert.h>
#include <poincare/preferences.h>

using namespace Shared;
using namespace Poincare;

namespace Settings {

SymbolController::SymbolController(Responder * parentResponder) :
  GenericSubController(parentResponder)
{
  for (int i = 0; i < k_totalNumberOfSwitchCells; i++) {
    m_switchCells[i].setMessageFont(KDFont::LargeFont);
  }
}

bool SymbolController::handleEvent(Ion::Events::Event event) {
  Preferences * preferences = Preferences::sharedPreferences();
  Poincare::Preferences::SymbolMultiplication symbolofMultiplication = preferences->symbolofMultiplication();
  bool autoSymbol = (int)preferences->autoSymbol(); 
  if (event == Ion::Events::OK || event == Ion::Events::EXE){
    switch(selectedRow()){
      case 0:
        {
          symbolofMultiplication = Poincare::Preferences::SymbolMultiplication::Cross;
          break;
        }
      case 1:
        {
          symbolofMultiplication = Poincare::Preferences::SymbolMultiplication::MiddleDot;
          break;
        }
      case 2:
        {
          symbolofMultiplication = Poincare::Preferences::SymbolMultiplication::Star;
          break;
        }
      case 3:
        {
          autoSymbol = !autoSymbol;
          break;
        }
      default:
        {
          GenericSubController::handleEvent(event);
        }
    }
    preferences->setSymbolMultiplication(symbolofMultiplication);
    autoSymbol ? preferences->SetAutoSymbol(Poincare::Preferences::AutoSymbol::True) : preferences->SetAutoSymbol(Poincare::Preferences::AutoSymbol::False);
    m_selectableTableView.reloadData();
    return true;
  } else {
    return GenericSubController::handleEvent(event);
  }
}

HighlightCell * SymbolController::reusableCell(int index, int type) {
  assert(type == 1 || type == 2);
  if (type == 2) {
    assert(index >= 0 && index < k_totalNumberOfSwitchCells);
    return &m_switchCells[index];
  }
  return nullptr;
}

int SymbolController::reusableCellCount(int type) {
  assert(type == 1 || type == 2);
  if (type == 2) {
    return k_totalNumberOfSwitchCells;
  }
  return 0;
}

void SymbolController::willDisplayCellForIndex(HighlightCell * cell, int index) {
  GenericSubController::willDisplayCellForIndex(cell, index);

  MessageTableCellWithSwitch * mySwitchCell = (MessageTableCellWithSwitch *)cell;

  Preferences * preferences = Preferences::sharedPreferences();
  Poincare::Preferences::SymbolMultiplication symbolofMultiplication = preferences->symbolofMultiplication();
  bool autoSymbol = (int)preferences->autoSymbol(); 

  if (index == 0) {
    SwitchView * mySwitch = (SwitchView *)mySwitchCell->accessoryView();
    if(symbolofMultiplication == Poincare::Preferences::SymbolMultiplication::Cross){
      mySwitch->setState(true);
    } else {
      mySwitch->setState(false);
    }
  }
  else if (index == 1) {
    SwitchView * mySwitch = (SwitchView *)mySwitchCell->accessoryView();
    if(symbolofMultiplication == Poincare::Preferences::SymbolMultiplication::MiddleDot){
      mySwitch->setState(true);
    } else {
      mySwitch->setState(false);
    }
  }
  else if (index == 2) {
    SwitchView * mySwitch = (SwitchView *)mySwitchCell->accessoryView();
    if(symbolofMultiplication == Poincare::Preferences::SymbolMultiplication::Star){
      mySwitch->setState(true);
    } else {
      mySwitch->setState(false);
    }
  }
  else if (index == 3){
    SwitchView * mySwitch = (SwitchView *)mySwitchCell->accessoryView();
    if(autoSymbol == true){
      mySwitch->setState(true);
    } else {
      mySwitch->setState(false);
    }
  }
}

int SymbolController::typeAtLocation(int i, int j) {
  switch (j) {
    case 0:
    case 1:
    case 2:
    case 3:
      return 2;
    default:
      return 1;
  }
}

}