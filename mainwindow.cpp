#include "mainwindow.h"
#include "chesspiece.h"
#include "ui_mainwindow.h"
#include "welcomewidget.h"
#include <QDateTime>
#include <QLCDNumber>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QThread>
#include <QtConcurrent>
#include <qdatetime.h>
#include <qgridlayout.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qtimer.h>

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::mainwindow) {
  setFixedSize(540, 600);
  ui->setupUi(this);
}

MainWindow::MainWindow(GameType gameType, WelcomeWindow *welcomeWindow,
                       QWidget *parent)
    : QDialog(parent), ui(new Ui::mainwindow), _gameType(gameType),
      _welcomeWindow(welcomeWindow) {
  setFixedSize(540, 600);
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
  if (_welcomeWindow != nullptr) {
    _welcomeWindow->close();
  }
}

void MainWindow::gameInit() {
  // set window title
  switch (_gameType) {
  case GameType::PERSON_VS_PERSON:
    setWindowTitle("CHINESE CHESS(Person vs Person)");
    break;
  case GameType::PERSON_VS_MACHINE:
    setWindowTitle("CHINESE CHESS(Person vs Machine)");
    break;
  default:
    break;
  }

  ui->_view->setFixedSize(540, 600);
  ui->_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->_view->setRenderHint(QPainter::Antialiasing);
  ui->_view->setCacheMode(QGraphicsView::CacheBackground);
  ui->_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  ui->_view->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
  ui->_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  ui->_view->setFrameShape(QFrame::NoFrame);
  ui->_view->setSceneRect(0, 0, 540, 600);
  ui->_view->setStyleSheet("background: transparent;");
  ui->_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->_view->setAlignment(Qt::AlignCenter);
  ui->_view->setStyleSheet("background: transparent;border:1px");

  if (_scene == nullptr) {
    _scene = new QGraphicsScene(this);
    _scene->setSceneRect(0, 0, 1200, 750);
    _scene->setBackgroundBrush(QBrush(QColor(255, 255, 255, 0)));
    _scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    _scene->setSceneRect(0, 0, 1200, 750);
    ui->_view->setScene(_scene);
  }

  if (_chessBoard == nullptr) {
    _chessBoard = new chessBoard();
    _scene->addItem(_chessBoard);
  }

  if (_chesspieces.empty()) {
    for (int i = 0; i < 5; i++) {
      _chesspieces.push_back(
          new chessPiece(ChessType::BING, ChessColor::BLACK, 3, i * 2));
    }

    _chesspieces.push_back(
        new chessPiece(ChessType::PAO, ChessColor::BLACK, 2, 1));
    _chesspieces.push_back(
        new chessPiece(ChessType::PAO, ChessColor::BLACK, 2, 7));

    _chesspieces.push_back(
        new chessPiece(ChessType::CHE, ChessColor::BLACK, 0, 0));
    _chesspieces.push_back(
        new chessPiece(ChessType::MA, ChessColor::BLACK, 0, 1));
    _chesspieces.push_back(
        new chessPiece(ChessType::XIANG, ChessColor::BLACK, 0, 2));
    _chesspieces.push_back(
        new chessPiece(ChessType::SHI, ChessColor::BLACK, 0, 3));
    _chesspieces.push_back(
        new chessPiece(ChessType::JIANG, ChessColor::BLACK, 0, 4));

    _chesspieces.push_back(
        new chessPiece(ChessType::SHI, ChessColor::BLACK, 0, 5));
    _chesspieces.push_back(
        new chessPiece(ChessType::XIANG, ChessColor::BLACK, 0, 6));
    _chesspieces.push_back(
        new chessPiece(ChessType::MA, ChessColor::BLACK, 0, 7));
    _chesspieces.push_back(
        new chessPiece(ChessType::CHE, ChessColor::BLACK, 0, 8));

    for (int i = 0; i < 5; i++) {
      _chesspieces.push_back(
          new chessPiece(ChessType::BING, ChessColor::RED, 6, i * 2));
    }

    _chesspieces.push_back(
        new chessPiece(ChessType::PAO, ChessColor::RED, 7, 1));
    _chesspieces.push_back(
        new chessPiece(ChessType::PAO, ChessColor::RED, 7, 7));

    _chesspieces.push_back(
        new chessPiece(ChessType::CHE, ChessColor::RED, 9, 0));
    _chesspieces.push_back(
        new chessPiece(ChessType::MA, ChessColor::RED, 9, 1));
    _chesspieces.push_back(
        new chessPiece(ChessType::XIANG, ChessColor::RED, 9, 2));
    _chesspieces.push_back(
        new chessPiece(ChessType::SHI, ChessColor::RED, 9, 3));
    _chesspieces.push_back(
        new chessPiece(ChessType::JIANG, ChessColor::RED, 9, 4));

    _chesspieces.push_back(
        new chessPiece(ChessType::SHI, ChessColor::RED, 9, 5));
    _chesspieces.push_back(
        new chessPiece(ChessType::XIANG, ChessColor::RED, 9, 6));
    _chesspieces.push_back(
        new chessPiece(ChessType::MA, ChessColor::RED, 9, 7));
    _chesspieces.push_back(
        new chessPiece(ChessType::CHE, ChessColor::RED, 9, 8));
  }

  if (_locationHints.empty()) {
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 9; j++) {
        _locationHints.push_back(new locationHint(i, j));
      }
    }
  }

  for (auto &hint : _locationHints) {
    _scene->addItem(hint);
  }

  for (auto &chess : _chesspieces) {
    _scene->addItem(chess);
  }

  _currentPlayer = ChessColor::RED;

  if (!_systemTimer) {
    _systemTimer = new QTimer(this);
    connect(_systemTimer, &QTimer::timeout, this, [&] {
      QString title;
      if (_gameType == GameType::PERSON_VS_PERSON) {
        title = "CHINESE CHESS(Person VS Person)";
      } else {
        title = "CHINESE CHESS(Person VS Machine)";
      }

      if (_currentPlayer == ChessColor::RED) {
        title += " (RED)";
      } else if (_currentPlayer == ChessColor::BLACK) {
        title += " (BLACK)";
      }
      setWindowTitle(title);

      if (_gameType == GameType::PERSON_VS_MACHINE &&
          _currentPlayer == ChessColor::BLACK && !isGameOver()) {
        if (!aiMoveFuture.isRunning()) {
          aiMoveFuture = QtConcurrent::run([this] {
            makeAIMove();
            QMetaObject::invokeMethod(
                this, [this] { _currentPlayer = ChessColor::RED; },
                Qt::QueuedConnection);
          });
        }
      }

      // check if game is over
      if (isGameOver()) {
        QString winner;
        if (_currentPlayer == ChessColor::RED) {
          winner = "BLACK";
        } else {
          winner = "RED";
        }
        QMessageBox::StandardButton btn = QMessageBox::information(
            this, "Game Over", "Game Over, " + winner + " win!");
        if (btn == QMessageBox::Ok) {
          _systemTimer->stop();
        }
      }
    });
  }

  _systemTimer->start(500);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    // get position
    QPointF pos = ui->_view->mapToScene(event->pos());
    int row = pos.y() / 60;
    int col = pos.x() / 60;
    qDebug() << "row: " << row << " col: " << col;

    // reset
    for (auto &chess : _chesspieces) {
      chess->setSelected(false);
    }

    if (_selectedPiece != nullptr) {

      if (_selectedPiece->getColor() != _currentPlayer) {
        // reset
        for (auto &piece : _chesspieces) {
          piece->setSelected(false);
        }
        // reset
        for (auto &hint : _locationHints) {
          hint->setShow(false);
        }
        _selectedPiece = nullptr;
        return;
      }

      // move pieses
      for (auto &hint : _locationHints) {
        hint->setShow(false);
      }

      // get available position
      QVector<QPair<int, int>> locations =
          getPieceAvailableLocations(_selectedPiece);
      bool canMove = false;
      for (auto location : locations) {
        if (location.first == row && location.second == col) {
          canMove = true;
          break;
        }
      }

      if (canMove) {
        // move
        movePieceTo(_selectedPiece, row, col, true);
        _currentPlayer = _currentPlayer == ChessColor::RED ? ChessColor::BLACK
                                                           : ChessColor::RED;
      } else {
        _selectedPiece->setSelected(false);
        _selectedPiece = nullptr;
      }
    } else {
      // Choose chess piece
      chessPiece *findPiece = nullptr;
      for (auto &chess : _chesspieces) {
        if (chess->isDead()) {
          continue;
        }
        if (chess->row() == row && chess->col() == col) {
          chess->setSelected(true);
          findPiece = chess;
          break;
        }
      }

      if (findPiece != nullptr) {
        _selectedPiece = findPiece;
        // Highlight available locations
        QVector<QPair<int, int>> locations =
            getPieceAvailableLocations(_selectedPiece);
        for (auto &hint : _locationHints) {
          hint->setShow(false);
        }
        for (auto location : locations) {
          for (auto &hint : _locationHints) {
            if (hint->row() == location.first &&
                hint->col() == location.second) {
              hint->setShow(true);
            }
          }
        }
      } else {
        if (_selectedPiece != nullptr) {
          _selectedPiece->setSelected(false);
          _selectedPiece = nullptr;
        }
      }
    }
  } else if (event->button() == Qt::RightButton) {
    // Right click to choose
    if (_selectedPiece != nullptr) {
      _selectedPiece->setSelected(false);
      _selectedPiece = nullptr;
    }
  }
  ui->_view->update();
}

QVector<QPair<int, int>>
MainWindow::getPieceAvailableLocations(chessPiece *piece) {
  QVector<QPair<int, int>> locations;
  if (piece == nullptr) {
    return locations;
  }

  // Black chess piece
  switch (piece->getType()) {
  case ChessType::JIANG:
    locations = JIANGAvailableLocations(piece);
    break;
  case ChessType::CHE:
    locations = CHEAvailableLocations(piece);
    break;
  case ChessType::MA:
    locations = MAAvailableLocations(piece);
    break;
  case ChessType::PAO:
    locations = PAOAvailableLocations(piece);
    break;
  case ChessType::BING:
    locations = BINGAvailableLocations(piece);
    break;
  case ChessType::XIANG:
    locations = XIANGAvailableLocations(piece);
    break;
  case ChessType::SHI:
    locations = SHIAvailableLocations(piece);
    break;
  default:
    break;
  }
  return locations;
}

QVector<QPair<int, int>>
MainWindow::JIANGAvailableLocations(chessPiece *piece) {
  QVector<QPair<int, int>> locations;
  if (piece == nullptr) {
    return locations;
  }

  int row = piece->row();
  int col = piece->col();

  // Determination of center point and corner point
  auto isCenterOrCorner = [row, col](ChessColor color) -> bool {
    if (color == ChessColor::BLACK) {
      // Black's center (1,4) and four corners (0,3), (0,5), (2,3), (2,5)
      return (row == 1 && col == 4) || (row == 0 && (col == 3 || col == 5)) ||
             (row == 2 && (col == 3 || col == 5));
    } else {
      // The red square has a center (8,4) and four corners (7,3), (7,5), (9,3), (9,5)
      return (row == 8 && col == 4) || (row == 7 && (col == 3 || col == 5)) ||
             (row == 9 && (col == 3 || col == 5));
    }
  };

  // Check if the piece is the black JIANG
  if (piece->getColor() == ChessColor::BLACK) {
    // Black general restricted to row 0-2 and column 3-5
    // Check possible moves: left, right, up, down
    if (col - 1 >= 3 && !hasPieceAt(row, col - 1, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row, col - 1)); // Move left
    }
    if (col + 1 <= 5 && !hasPieceAt(row, col + 1, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row, col + 1)); // Move right
    }
    if (row - 1 >= 0 && !hasPieceAt(row - 1, col, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row - 1, col)); // Move up
    }
    if (row + 1 <= 2 && !hasPieceAt(row + 1, col, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row + 1, col)); // Move down
    }

    // Add diagonal moves only if the piece is at the center or one of the
    // corners
    if (isCenterOrCorner(ChessColor::BLACK)) {
      if (row - 1 >= 0 && col - 1 >= 3 && row - 1 <= 2 && col - 1 <= 5 &&
          !hasPieceAt(row - 1, col - 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row - 1, col - 1)); // Move up-left
      }
      if (row - 1 >= 0 && col + 1 >= 3 && row - 1 <= 2 && col + 1 <= 5 &&
          !hasPieceAt(row - 1, col + 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row - 1, col + 1)); // Move up-right
      }
      if (row + 1 >= 0 && col - 1 >= 3 && row + 1 <= 2 && col - 1 <= 5 &&
          !hasPieceAt(row + 1, col - 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row + 1, col - 1)); // Move down-left
      }
      if (row + 1 >= 0 && col + 1 >= 3 && row + 1 <= 2 && col + 1 <= 5 &&
          !hasPieceAt(row + 1, col + 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row + 1, col + 1)); // Move down-right
      }
    }

  } else if (piece->getColor() == ChessColor::RED) {
    // Red general restricted to row 7-9 and column 3-5
    // Check possible moves: left, right, up, down
    if (col - 1 >= 3 && !hasPieceAt(row, col - 1, ChessColor::RED)) {
      locations.append(QPair<int, int>(row, col - 1)); // Move left
    }
    if (col + 1 <= 5 && !hasPieceAt(row, col + 1, ChessColor::RED)) {
      locations.append(QPair<int, int>(row, col + 1)); // Move right
    }
    if (row - 1 >= 7 && !hasPieceAt(row - 1, col, ChessColor::RED)) {
      locations.append(QPair<int, int>(row - 1, col)); // Move up
    }
    if (row + 1 <= 9 && !hasPieceAt(row + 1, col, ChessColor::RED)) {
      locations.append(QPair<int, int>(row + 1, col)); // Move down
    }

    // Add diagonal moves only if the piece is at the center or one of the
    // corners
    if (isCenterOrCorner(ChessColor::RED)) {
      if (row - 1 >= 7 && col - 1 >= 3 && row - 1 <= 9 && col - 1 <= 5 &&
          !hasPieceAt(row - 1, col - 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row - 1, col - 1)); // Move up-left
      }
      if (row - 1 >= 7 && col + 1 >= 3 && row - 1 <= 9 && col + 1 <= 5 &&
          !hasPieceAt(row - 1, col + 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row - 1, col + 1)); // Move up-right
      }
      if (row + 1 >= 7 && col - 1 >= 3 && row + 1 <= 9 && col - 1 <= 5 &&
          !hasPieceAt(row + 1, col - 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row + 1, col - 1)); // Move down-left
      }
      if (row + 1 >= 7 && col + 1 >= 3 && row + 1 <= 9 && col + 1 <= 5 &&
          !hasPieceAt(row + 1, col + 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row + 1, col + 1)); // Move down-right
      }
    }
  }

  return locations;
}

QVector<QPair<int, int>> MainWindow::SHIAvailableLocations(chessPiece *piece) {
  QVector<QPair<int, int>> locations;
  if (piece == nullptr) {
    return locations;
  }

  int row = piece->row();
  int col = piece->col();

  // Check if the piece is the black SHI
  if (piece->getColor() == ChessColor::BLACK) {
    // Black Guard is restricted to row 0-2 and col 3-5, moving diagonally
    if (col - 1 >= 3 && !hasPieceAt(row, col - 1, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row, col - 1)); // Move left
    }
    if (col + 1 <= 5 && !hasPieceAt(row, col + 1, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row, col + 1)); // Move right
    }
    if (row - 1 >= 0 && !hasPieceAt(row - 1, col, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row - 1, col)); // Move up
    }
    if (row + 1 <= 2 && !hasPieceAt(row + 1, col, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row + 1, col)); // Move down
    }

    // Check diagonal moves only for four corners and center (0,3), (0,5),
    // (2,3), (2,5), (1,4)
    if ((row == 0 && col == 3) || (row == 0 && col == 5) ||
        (row == 2 && col == 3) || (row == 2 && col == 5) ||
        (row == 1 && col == 4)) {
      // Check all four diagonal moves
      if (row - 1 >= 0 && col - 1 >= 3 && row - 1 <= 2 && col - 1 <= 5 &&
          !hasPieceAt(row - 1, col - 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row - 1, col - 1)); // Move up-left
      }
      if (row - 1 >= 0 && col + 1 >= 3 && row - 1 <= 2 && col + 1 <= 5 &&
          !hasPieceAt(row - 1, col + 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row - 1, col + 1)); // Move up-right
      }
      if (row + 1 >= 0 && col - 1 >= 3 && row + 1 <= 2 && col - 1 <= 5 &&
          !hasPieceAt(row + 1, col - 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row + 1, col - 1)); // Move down-left
      }
      if (row + 1 >= 0 && col + 1 >= 3 && row + 1 <= 2 && col + 1 <= 5 &&
          !hasPieceAt(row + 1, col + 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row + 1, col + 1)); // Move down-right
      }
    }

  } else if (piece->getColor() == ChessColor::RED) {
    // Red Guard is restricted to row 7-9 and col 3-5, moving diagonally
    if (col - 1 >= 3 && !hasPieceAt(row, col - 1, ChessColor::RED)) {
      locations.append(QPair<int, int>(row, col - 1)); // Move left
    }
    if (col + 1 <= 5 && !hasPieceAt(row, col + 1, ChessColor::RED)) {
      locations.append(QPair<int, int>(row, col + 1)); // Move right
    }
    if (row - 1 >= 7 && !hasPieceAt(row - 1, col, ChessColor::RED)) {
      locations.append(QPair<int, int>(row - 1, col)); // Move up
    }
    if (row + 1 <= 9 && !hasPieceAt(row + 1, col, ChessColor::RED)) {
      locations.append(QPair<int, int>(row + 1, col)); // Move down
    }

    // Check diagonal moves only for four corners and center (7,3), (7,5),
    // (9,3), (9,5), (8,4)
    if ((row == 7 && col == 3) || (row == 7 && col == 5) ||
        (row == 9 && col == 3) || (row == 9 && col == 5) ||
        (row == 8 && col == 4)) {
      // Check all four diagonal moves
      if (row - 1 >= 7 && col - 1 >= 3 && row - 1 <= 9 && col - 1 <= 5 &&
          !hasPieceAt(row - 1, col - 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row - 1, col - 1)); // Move up-left
      }
      if (row - 1 >= 7 && col + 1 >= 3 && row - 1 <= 9 && col + 1 <= 5 &&
          !hasPieceAt(row - 1, col + 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row - 1, col + 1)); // Move up-right
      }
      if (row + 1 >= 7 && col - 1 >= 3 && row + 1 <= 9 && col - 1 <= 5 &&
          !hasPieceAt(row + 1, col - 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row + 1, col - 1)); // Move down-left
      }
      if (row + 1 >= 7 && col + 1 >= 3 && row + 1 <= 9 && col + 1 <= 5 &&
          !hasPieceAt(row + 1, col + 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row + 1, col + 1)); // Move down-right
      }
    }
  }

  return locations;
}

QVector<QPair<int, int>> MainWindow::BINGAvailableLocations(chessPiece *piece) {
  QVector<QPair<int, int>> locations;
  if (piece == nullptr) {
    return locations;
  }

  int row = piece->row();
  int col = piece->col();

  // Check if the piece is a black BING
  if (piece->getColor() == ChessColor::BLACK) {
    // Black Soldier can only move forward before crossing the river
    if (row + 1 <= 9 && !hasPieceAt(row + 1, col, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row + 1, col)); // Move forward
    }

    // After crossing the river (row >= 5), it can also move left and right
    if (row >= 5) {
      if (col - 1 >= 0 && !hasPieceAt(row, col - 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row, col - 1)); // Move left
      }
      if (col + 1 <= 8 && !hasPieceAt(row, col + 1, ChessColor::BLACK)) {
        locations.append(QPair<int, int>(row, col + 1)); // Move right
      }
    }

  } else if (piece->getColor() == ChessColor::RED) {
    // Red Soldier can only move forward before crossing the river
    if (row - 1 >= 0 && !hasPieceAt(row - 1, col, ChessColor::RED)) {
      locations.append(QPair<int, int>(row - 1, col)); // Move forward
    }

    // After crossing the river (row <= 4), it can also move left and right
    if (row <= 4) {
      if (col - 1 >= 0 && !hasPieceAt(row, col - 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row, col - 1)); // Move left
      }
      if (col + 1 <= 8 && !hasPieceAt(row, col + 1, ChessColor::RED)) {
        locations.append(QPair<int, int>(row, col + 1)); // Move right
      }
    }
  }

  return locations;
}

QVector<QPair<int, int>>
MainWindow::XIANGAvailableLocations(chessPiece *piece) {
  QVector<QPair<int, int>> locations;
  if (piece == nullptr) {
    return locations;
  }

  int row = piece->row();
  int col = piece->col();

  // Check if the piece is the black bishop
  if (piece->getColor() == ChessColor::BLACK) {
    // Black bishop can only move within rows 0-4 (can't cross the river)
    // Check for all possible Tian Zi Ge moves which is ‘田’ shape
    if (row + 2 <= 4 && col + 2 <= 8 &&
        !hasPieceAt(row + 1, col + 1, ChessColor::BLACK) &&
        !hasPieceAt(row + 2, col + 2, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row + 2, col + 2)); // Move down-right
    }
    if (row + 2 <= 4 && col - 2 >= 0 &&
        !hasPieceAt(row + 1, col - 1, ChessColor::BLACK) &&
        !hasPieceAt(row + 2, col - 2, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row + 2, col - 2)); // Move down-left
    }
    if (row - 2 >= 0 && col + 2 <= 8 &&
        !hasPieceAt(row - 1, col + 1, ChessColor::BLACK) &&
        !hasPieceAt(row - 2, col + 2, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row - 2, col + 2)); // Move up-right
    }
    if (row - 2 >= 0 && col - 2 >= 0 &&
        !hasPieceAt(row - 1, col - 1, ChessColor::BLACK) &&
        !hasPieceAt(row - 2, col - 2, ChessColor::BLACK)) {
      locations.append(QPair<int, int>(row - 2, col - 2)); // Move up-left
    }

  } else if (piece->getColor() == ChessColor::RED) {
    // Red bishop can only move within rows 5-9 (can't cross the river)
    // Check for all possible Tian Zi Ge moves which is ‘田’ shape
    if (row + 2 <= 9 && col + 2 <= 8 &&
        !hasPieceAt(row + 1, col + 1, ChessColor::RED) &&
        !hasPieceAt(row + 2, col + 2, ChessColor::RED)) {
      locations.append(QPair<int, int>(row + 2, col + 2)); // Move down-right
    }
    if (row + 2 <= 9 && col - 2 >= 0 &&
        !hasPieceAt(row + 1, col - 1, ChessColor::RED) &&
        !hasPieceAt(row + 2, col - 2, ChessColor::RED)) {
      locations.append(QPair<int, int>(row + 2, col - 2)); // Move down-left
    }
    if (row - 2 >= 5 && col + 2 <= 8 &&
        !hasPieceAt(row - 1, col + 1, ChessColor::RED) &&
        !hasPieceAt(row - 2, col + 2, ChessColor::RED)) {
      locations.append(QPair<int, int>(row - 2, col + 2)); // Move up-right
    }
    if (row - 2 >= 5 && col - 2 >= 0 &&
        !hasPieceAt(row - 1, col - 1, ChessColor::RED) &&
        !hasPieceAt(row - 2, col - 2, ChessColor::RED)) {
      locations.append(QPair<int, int>(row - 2, col - 2)); // Move up-left
    }
  }

  return locations;
}

QVector<QPair<int, int>> MainWindow::MAAvailableLocations(chessPiece *piece) {
  QVector<QPair<int, int>> locations;
  if (piece == nullptr) {
    return locations;
  }

  int row = piece->row();
  int col = piece->col();

  // Define all 8 ways the horse can move in the "sun" shape which is "日" shape
  QVector<QPair<QPair<int, int>, QPair<int, int>>> moves = {
      // Previous space -> left and right space
      {{-1, 0}, {-2, -1}},
      {{-1, 0}, {-2, 1}},
      // Next space -> Two space to the left and right
      {{1, 0}, {2, -1}},
      {{1, 0}, {2, 1}},
      // One space left -> two spaces up and down
      {{0, -1}, {-1, -2}},
      {{0, -1}, {1, -2}},
      // One space to the right -> two spaces up and down
      {{0, 1}, {-1, 2}},
      {{0, 1}, {1, 2}}};

  for (const auto &move : moves) {
    // leg for horse position
    int legRow = row + move.first.first;
    int legCol = col + move.first.second;

    // Target location
    int targetRow = row + move.second.first;
    int targetCol = col + move.second.second;

    // 1. Check if the horse's foot is out of bounds
    if (legRow < 0 || legRow > 9 || legCol < 0 || legCol > 8) {
      continue; // The horse's leg is out of bounds and cannot move.
    }

    // 2. Check whether the horse's foot is blocked by a chess piece
    if (hasPieceAt(legRow, legCol, piece->getColor())) {
      continue; // There is a chess piece in the horse's foot and cannot be moved.
    }

    // 3. Check if the target position is out of bounds
    if (targetRow < 0 || targetRow > 9 || targetCol < 0 || targetCol > 8) {
      continue; // The target location is out of bounds and cannot be moved.
    }

    // 4. Check if there is a chess piece of the same color at the target position
    if (!hasPieceAt(targetRow, targetCol, piece->getColor())) {
      locations.append(QPair<int, int>(targetRow, targetCol)); // Can be moved
    }
  }

  return locations;
}

QVector<QPair<int, int>> MainWindow::PAOAvailableLocations(chessPiece *piece) {
  QVector<QPair<int, int>> locations;
  if (piece == nullptr) {
    return locations;
  }

  int row = piece->row();
  int col = piece->col();

  // Define offsets in four directions: up (-1, 0), down (1, 0), left (0, -1), right (0, 1)
  QVector<QPair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  // Traverse every direction
  for (const auto &dir : directions) {
    bool foundCannonStand = false; // Mark whether the stand for cannon is found
    int dRow = dir.first;
    int dCol = dir.second;

    // Scan step by step in that direction
    for (int step = 1;; step++) {
      int newRow = row + dRow * step;
      int newCol = col + dCol * step;

      // 1. Check if out of bounds
      if (newRow < 0 || newRow > 9 || newCol < 0 || newCol > 8) {
        break; // Beyond board boundaries
      }

      // 2. Check if there is a chess piece at the current position
      if (hasPieceAt(newRow, newCol, ChessColor::BLACK) ||
          hasPieceAt(newRow, newCol, ChessColor::RED)) {
        if (!foundCannonStand) {
          // 2.1 If the Cannon stand has not been found, record the cannon stand position and stop adding movements.
          foundCannonStand = true;
        } else {
          // 2.2 If you already have a cannon stand and there is an enemy chess piece at the current position, you can destory the chess piece.
          if (!hasPieceAt(newRow, newCol, piece->getColor())) {
            locations.append(QPair<int, int>(newRow, newCol)); // Cannon destory chess piece
          }
          break; // stop after destory chess piece
        }
      } else {
        // 3. If there is no chess piece at the current position and no cannon stand is found, you can move normally
        if (!foundCannonStand) {
          locations.append(QPair<int, int>(newRow, newCol)); // normal move
        }
      }
    }
  }

  return locations;
}

QVector<QPair<int, int>> MainWindow::CHEAvailableLocations(chessPiece *piece) {
  QVector<QPair<int, int>> locations;
  if (piece == nullptr) {
    return locations;
  }

  int row = piece->row();
  int col = piece->col();

  // Define the offset in four directions: up (-1, 0), down (1, 0), left (0, -1), right (0, 1)
  QVector<QPair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

  // Traverse each direction
  for (const auto &dir : directions) {
    int dRow = dir.first;
    int dCol = dir.second;

    // Scan step by step in this direction
    for (int step = 1;; step++) {
      int newRow = row + dRow * step;
      int newCol = col + dCol * step;

      // 1. Check if it is out of bounds
      if (newRow < 0 || newRow > 9 || newCol < 0 || newCol > 8) {
        break; // Beyond the board boundaries
      }

      // 2. Check if there is a chess piece at the current position
      if (hasPieceAt(newRow, newCol, ChessColor::BLACK) ||
          hasPieceAt(newRow, newCol, ChessColor::RED)) {
        // 2.1 If it is an enemy, you can destory it.
        ChessColor color = piece->getColor() == ChessColor::RED
                               ? ChessColor::BLACK
                               : ChessColor::RED;
        if (hasPieceAt(newRow, newCol, color)) {
          locations.append(QPair<int, int>(newRow, newCol)); // chariot destory chess piece
        }
        break;
      }

      // 3. There is no chess piece in the current grid, so it can be moved normally.
      locations.append(QPair<int, int>(newRow, newCol)); // normal move
    }
  }

  return locations;
}

bool MainWindow::hasPieceAt(int row, int col, ChessColor color) {
  for (auto &chess : _chesspieces) {
    if (chess->row() == row && chess->col() == col &&
        chess->getColor() == color && !chess->isDead()) {
      return true;
    }
  }
  return false;
}

void MainWindow::movePieceTo(chessPiece *piece, int newRow, int newCol,
                             bool real) {
  // Check if there is a chess piece in the target position
  chessPiece *targetPiece = nullptr;
  for (auto &chess : _chesspieces) {
    if (chess->row() == newRow && chess->col() == newCol && !chess->isDead()) {
      targetPiece = chess;
      break;
    }
  }

  // If there is an enemy, destory it
  if (targetPiece != nullptr && targetPiece->getColor() != piece->getColor()) {
    targetPiece->setDead(true); // Mark destoryed pieces as Dead
    if (!real) {
      _capturedPieces.push(targetPiece);
    }
  }

  // Move a chess to a new position
  piece->setRow(newRow);
  piece->setCol(newCol);
  piece->setSelected(false); // Uncheck after moving
  _selectedPiece = nullptr;
}

int MainWindow::checkGameOver() {
  chessPiece *blackKing = nullptr;
  chessPiece *redKing = nullptr;

  // Find the black general and the red general
  for (auto &chess : _chesspieces) {
    if (chess->getType() == ChessType::JIANG) {
      if (chess->getColor() == ChessColor::BLACK) {
        blackKing = chess;
      } else if (chess->getColor() == ChessColor::RED) {
        redKing = chess;
      }
    }
  }

  // Check if the black or red king has been destoryed
  if (blackKing == nullptr || blackKing->isDead()) {
    // Black is destoryed, Red wins
    qDebug() << "Red wins!";
    return -1;
  }

  if (redKing == nullptr || redKing->isDead()) {
    // Red is destoryed, Black wins
    qDebug() << "Black wins!";
    return 1;
  }

  return 0;
}

void MainWindow::makeAIMove() {
  chessPiece *bestPiece = nullptr;
  QPair<int, int> bestMove;
  int bestValue = -100000;
  qDebug() << __FUNCTION__;
  // Traverse all AI chess pieces and find the best move
  for (auto &piece : _chesspieces) {
    if (piece->getColor() == ChessColor::BLACK && !piece->isDead()) {
      QVector<QPair<int, int>> availableMoves =
          getPieceAvailableLocations(piece);
      for (const auto &move : availableMoves) {
        // Backup location
        int oldRow = piece->row();
        int oldCol = piece->col();
        // Simulate movement
        movePieceTo(piece, move.first, move.second, false);

        // Call the Minimax algorithm to obtain the situation score
        int boardValue = minimax(2, false, -100000, 100000); // Search depth is 3

        // Undo Move
        undoMove(piece, oldRow, oldCol);
        if (boardValue > bestValue) {
          bestValue = boardValue;
          bestPiece = piece;
          bestMove = move;
        }
      }
    }
  }

  // move
  if (bestPiece != nullptr) {
    movePieceTo(bestPiece, bestMove.first, bestMove.second, true);
    _currentPlayer = ChessColor::RED; // back to red party
  }
  ui->_view->update();
}

int MainWindow::minimax(int depth, bool isMaximizingPlayer, int alpha,
                        int beta) {
  if (depth == 0 || isGameOver()) {
    return evaluateBoard();
  }

  if (isMaximizingPlayer) {
    int maxEval = -100000;
    for (auto &piece : _chesspieces) {
      if (piece->getColor() == ChessColor::BLACK && !piece->isDead()) {
        QVector<QPair<int, int>> availableMoves =
            getPieceAvailableLocations(piece);
        for (const auto &move : availableMoves) {
          // backup previous position
          int oldRow = piece->row();
          int oldCol = piece->col();
          // fake move
          movePieceTo(piece, move.first, move.second, false);

          // find best move
          int eval = minimax(depth - 1, false, alpha, beta);

          // restore move
          undoMove(piece, oldRow, oldCol);

          maxEval = std::max(maxEval, eval);
          alpha = std::max(alpha, eval);
          if (beta <= alpha) {
            break;
          }
        }
      }
    }
    return maxEval;
  } else {
    int minEval = 100000;
    for (auto &piece : _chesspieces) {
      if (piece->getColor() == ChessColor::RED && !piece->isDead()) {
        QVector<QPair<int, int>> availableMoves =
            getPieceAvailableLocations(piece);
        for (const auto &move : availableMoves) {
          int oldRow = piece->row();
          int oldCol = piece->col();
          // fake move
          movePieceTo(piece, move.first, move.second, false);

          // find best move
          int eval = minimax(depth - 1, true, alpha, beta);

          // restore move
          undoMove(piece, oldRow, oldCol);

          minEval = std::min(minEval, eval);
          beta = std::min(beta, eval);
          if (beta <= alpha) {
            break;
          }
        }
      }
    }
    return minEval;
  }
}

int MainWindow::evaluateBoard() {
  int score = 0;

  for (const auto &piece : _chesspieces) {
    if (piece->isDead()) {
      continue;
    }

    int pieceValue = 0;
    switch (piece->getType()) {
    case ChessType::JIANG:
      pieceValue = 1000;
      break;
    case ChessType::CHE:
      pieceValue = 500;
      break;
    case ChessType::MA:
      pieceValue = 300;
      break;
    case ChessType::PAO:
      pieceValue = 300;
      break;
    case ChessType::BING:
      pieceValue = 100;
      break;
    case ChessType::XIANG:
    case ChessType::SHI:
      pieceValue = 200;
      break;
    default:
      break;
    }

    if (piece->getColor() == ChessColor::BLACK) {
      score += pieceValue;
    } else {
      score -= pieceValue;
    }
  }
  return score;
}

void MainWindow::undoMove(chessPiece *piece, int oldRow, int oldCol) {
  piece->setRow(oldRow);
  piece->setCol(oldCol);

  if (!_capturedPieces.empty()) {
    chessPiece *restoredPiece = _capturedPieces.top();
    _capturedPieces.pop();
    restoredPiece->setDead(false); // Recover destoryed pieces
  }
}

bool MainWindow::isGameOver() {
  bool ret = false;
  for (auto chiese : _chesspieces) {
    if (chiese->getType() == ChessType::JIANG) {
      if (chiese->isDead()) {
        ret = true;
        break;
      }
    }
  }
  return ret;
}
