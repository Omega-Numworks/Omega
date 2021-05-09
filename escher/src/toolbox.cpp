#include <escher/toolbox.h>
#include <escher/metric.h>
#include <assert.h>
#include <string.h>

Toolbox::Toolbox(Responder * parentResponder, I18n::Message title) :
  NestedMenuController(parentResponder, title),
  m_messageTreeModel(nullptr)
{}

void Toolbox::viewWillAppear() {
  m_messageTreeModel = (ToolboxMessageTree *)rootModel();
  NestedMenuController::viewWillAppear();
}

int Toolbox::numberOfRows() const {
  if (m_messageTreeModel == nullptr) {
    m_messageTreeModel = (ToolboxMessageTree *)rootModel();
  }
  return m_messageTreeModel->numberOfChildren();
}

int Toolbox::reusableCellCount(int type) {
  return maxNumberOfDisplayedRows();
}

void Toolbox::willDisplayCellForIndex(HighlightCell * cell, int index) {
  ToolboxMessageTree * messageTree = (ToolboxMessageTree *)m_messageTreeModel->childAtIndex(index);
  if (messageTree->numberOfChildren() == 0) {
    MessageTableCellWithMessage * myCell = (MessageTableCellWithMessage *)cell;
    myCell->setMessage(messageTree->label());
    myCell->setAccessoryMessage(messageTree->text());
    myCell->setAccessoryTextColor(Palette::SecondaryText);
    return;
  }
  MessageTableCell * myCell = (MessageTableCell *)cell;
  myCell->setMessage(messageTree->label());
  myCell->reloadCell();
}

int Toolbox::typeAtLocation(int i, int j) {
  const MessageTree * messageTree = m_messageTreeModel->childAtIndex(j);
  if (messageTree->numberOfChildren() == 0) {
    return LeafCellType;
  }
  return NodeCellType;
}

bool Toolbox::handleEventForRow(Ion::Events::Event event, int selectedRow) {
  if (event.isKeyboardEvent()) {
    uint8_t key = static_cast<uint8_t>(event.toKey());
    uint8_t min = static_cast<uint8_t>(Ion::Keyboard::Key::Exp);
    uint8_t max = static_cast<uint8_t>(Ion::Keyboard::Key::Plus) - min;
    key-=min;
    if (key >= 0 && key <= max) {
      if (key <= numberOfRows()) {
        return m_selectableTableView.selectCellAtClippedLocation(m_selectableTableView.selectedColumn(), key);
      }
    }
  }
  return NestedMenuController::handleEventForRow(event, selectedRow);
}

bool Toolbox::selectSubMenu(int selectedRow) {
  m_selectableTableView.deselectTable();
  m_messageTreeModel = static_cast<const ToolboxMessageTree *>(m_messageTreeModel->childAtIndex(selectedRow));
  if (m_messageTreeModel->isFork()) {
    assert(m_messageTreeModel->numberOfChildren() < 0);
    m_messageTreeModel = static_cast<const ToolboxMessageTree *>(m_messageTreeModel->childAtIndex(indexAfterFork()));
  }
  return NestedMenuController::selectSubMenu(selectedRow);
}

bool Toolbox::returnToPreviousMenu() {
  m_selectableTableView.deselectTable();
  int currentDepth = m_stack.depth();
  int index = 0;
  ToolboxMessageTree * parentMessageTree = (ToolboxMessageTree *)rootModel();
  Stack::State * previousState = m_stack.stateAtIndex(index++);
  while (currentDepth-- > 1) {
    parentMessageTree = (ToolboxMessageTree *)parentMessageTree->childAtIndex(previousState->selectedRow());
    previousState = m_stack.stateAtIndex(index++);
    if (parentMessageTree->isFork()) {
      parentMessageTree = (ToolboxMessageTree *)parentMessageTree->childAtIndex(indexAfterFork());
    }
  }
  m_messageTreeModel = parentMessageTree;
  return NestedMenuController::returnToPreviousMenu();
}
