#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QGraphicsItem>
#include <QPaintEvent>
#include <qgraphicsitem.h>
#include <qwidget.h>

// Chess board class, used to draw the chessboard and manage chess pieces
class chessBoard : public QGraphicsObject {

public:
  explicit chessBoard(QGraphicsItem *parent = nullptr);
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

signals:
};

#endif // CHESSBOARD_H
