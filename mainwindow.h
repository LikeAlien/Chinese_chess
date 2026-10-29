#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessboard.h"
#include "chesspiece.h"
#include "ui_mainwindow.h"
#include <QFuture>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qgridlayout.h>
#include <qlcdnumber.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <stack>

class WelcomeWindow;

namespace Ui {
class mainwindow;
}

enum class GameType {
  NONE,              // None
  PERSON_VS_PERSON,  // PVP
  PERSON_VS_MACHINE, // PVE
};

class MainWindow : public QDialog {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  MainWindow(GameType gameType, WelcomeWindow *welcomeWindow,
             QWidget *parent = nullptr);
  ~MainWindow();
  void setGameType(GameType gameType) {
    _gameType = gameType;
    gameInit();
  }
  GameType getGameType() const { return _gameType; }

  // Override mouse click events
  void mousePressEvent(QMouseEvent *event) override;

public slots:
  void makeAIMove();

protected:
  void gameInit();
  QVector<QPair<int, int>> getPieceAvailableLocations(chessPiece *piece);
  QVector<QPair<int, int>> JIANGAvailableLocations(chessPiece *piece);
  QVector<QPair<int, int>> SHIAvailableLocations(chessPiece *piece);
  QVector<QPair<int, int>> XIANGAvailableLocations(chessPiece *piece);
  QVector<QPair<int, int>> MAAvailableLocations(chessPiece *piece);
  QVector<QPair<int, int>> PAOAvailableLocations(chessPiece *piece);
  QVector<QPair<int, int>> BINGAvailableLocations(chessPiece *piece);
  QVector<QPair<int, int>> CHEAvailableLocations(chessPiece *piece);
  bool hasPieceAt(int row, int col, ChessColor color);
  void movePieceTo(chessPiece *piece, int newRow, int newCol, bool real = true);
  int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta);
  int evaluateBoard();
  void undoMove(chessPiece *piece, int oldRow, int oldCol);
  bool isGameOver();
  int checkGameOver();

private:
  GameType _gameType = GameType::NONE;
  WelcomeWindow *_welcomeWindow = nullptr;
  chessBoard *_chessBoard = nullptr;
  QGraphicsScene *_scene = nullptr;
  QVector<chessPiece *> _chesspieces;
  QVector<locationHint *> _locationHints;
  chessPiece *_selectedPiece = nullptr;
  ChessColor _currentPlayer = ChessColor::RED;
  std::stack<chessPiece *> _capturedPieces;
  QFuture<void> aiMoveFuture;
  QTimer *_systemTimer = nullptr;

  Ui::mainwindow *ui;
};
#endif // MAINWINDOW_H
