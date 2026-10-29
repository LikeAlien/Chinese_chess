#include "mainwindow.h"
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QRegion>
#include <QTextBrowser>
#include <QWidget>
#include <qgridlayout.h>

class WelcomeWindow : public QWidget {
public:
  WelcomeWindow();
  ~WelcomeWindow();
  void keyPressEvent(QKeyEvent *event) override;
  void setupButtons();

private:
  bool mousePressed;
  QPoint mousePos;
  QPushButton *_personVsPerson = nullptr;
  QPushButton *_personVsMachine = nullptr;
  QGridLayout *_mainLayout = nullptr;
  QTextBrowser *_gameInfo = nullptr;
  MainWindow *_mainWindow = nullptr;
};
