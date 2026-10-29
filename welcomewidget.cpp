#include "welcomewidget.h"
#include <qgridlayout.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <qtextbrowser.h>

WelcomeWindow::WelcomeWindow() : mousePressed(false) {

  setMinimumWidth(600);
  setMinimumHeight(300);
  setupButtons();
}

void WelcomeWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    // Press ESC to close the window
    close();
  } else {
    // Other keys are handled by the base class
    QWidget::keyPressEvent(event);
  }
}

void WelcomeWindow::setupButtons() {
  if (!_personVsMachine) {
    _personVsMachine = new QPushButton("Person VS  Machine", this);
  }

  if (!_personVsPerson) {
    _personVsPerson = new QPushButton("Person VS Person", this);
  }

  if (!_mainLayout) {
    _mainLayout = new QGridLayout(this);
  }

  if (!_gameInfo) {
    _gameInfo = new QTextBrowser(this);
    _gameInfo->setText(
        "Chinese Chess, or Xiangqi (象棋), is a traditional strategic board "
        "game for two players,"
        "originating from China. The game is played on a board with 9 vertical "
        "and 10 horizontal lines. Each player commands an army of 16 pieces: "
        "one General, two Guards, two Bishops, two Horses, two Chariots, "
        "two Cannons, and five Soldiers."
        "The primary objective is to checkmate the opponent’s General. Each "
        "piece has unique movement rules:"
        " the General moves one space within a designated palace; Guards "
        "move diagonally within the palace;"
        " Bishops move two spaces diagonally but cannot cross the river; "
        "Horses move in an \"L\" shape;"
        " Chariots move any number of spaces horizontally or vertically; "
        "Cannons capture by jumping over one piece; "
        "and Soldiers move forward, with additional sideways movement after "
        "crossing the river."
        "Players take turns moving one piece, with the game ending when a "
        "player checkmates the opponent's General."
        " Special rules, such as preventing repeated moves, may lead to a "
        "draw. Xiangqi combines deep strategy and tactical skill, "
        "making it a popular and culturally significant game in Chinese "
        "society.");
  }
  _mainLayout->addWidget(_gameInfo, 0, 0, 10, 10);
  _mainLayout->addWidget(_personVsPerson, 11, 3, Qt::AlignCenter);
  _mainLayout->addWidget(_personVsMachine, 11, 6, Qt::AlignCenter);
  setLayout(_mainLayout);

  connect(_personVsPerson, &QPushButton::clicked, this, [&] {
    if (!_mainWindow) {
      _mainWindow = new MainWindow(GameType::PERSON_VS_PERSON, this);
      _mainWindow->setGameType(GameType::PERSON_VS_PERSON);
      _mainWindow->show();
      hide();
    }
  });

  connect(_personVsMachine, &QPushButton::clicked, this, [&] {
    if (!_mainWindow) {
      _mainWindow = new MainWindow(GameType::PERSON_VS_MACHINE, this);
      _mainWindow->setGameType(GameType::PERSON_VS_MACHINE);
      _mainWindow->show();
      hide();
    }
  });
}

WelcomeWindow::~WelcomeWindow() {}
