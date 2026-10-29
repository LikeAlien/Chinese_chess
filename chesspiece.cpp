#include "chesspiece.h"
#include <QGraphicsItem>
#include <QPainter>
#include <qcolor.h>
#include <qgraphicsitem.h>
#include <qnamespace.h>
#include <qpen.h>
#include <qpoint.h>

QString chessTypeToString(ChessType type, ChessColor color) {
  if (color == ChessColor::RED) {
    switch (type) {
    case ChessType::CHE:
      return "俥";
    case ChessType::MA:
      return "傌";
    case ChessType::PAO:
      return "炮";
    case ChessType::BING:
      return "兵";
    case ChessType::JIANG:
      return "帥";
    case ChessType::SHI:
      return "仕";
    case ChessType::XIANG:
      return "相";
    default:
      return "null";
    }
  } else {
    switch (type) {
    case ChessType::CHE:
      return "車";
    case ChessType::MA:
      return "馬";
    case ChessType::PAO:
      return "砲";
    case ChessType::BING:
      return "卒";
    case ChessType::JIANG:
      return "將";
    case ChessType::SHI:
      return "士";
    case ChessType::XIANG:
      return "象";
    default:
      return "null";
    }
  }
}

chessPiece::chessPiece(QGraphicsItem *parent) : QGraphicsObject(parent) {}

chessPiece::chessPiece(ChessType type, ChessColor color, int row, int col,
                       QGraphicsItem *parent)
    : _type(type), _color(color), _col(col), _row(row),
      QGraphicsObject(parent) {}

QRectF chessPiece::boundingRect() const { return QRectF(0, 0, 60, 60); }

void chessPiece::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  double m_offset = 30;
  double m_distance = 60;
  double m_radius = m_distance / 2;

  if (_dead) {
    return;
  }

  // Enable anti-aliasing
  painter->setRenderHint(QPainter::Antialiasing, true);

  // Draw an offset shadow
  painter->setPen(Qt::NoPen);
  painter->setBrush(QColor(0, 0, 0, 80));


  // Create a circle-centered gradient to draw the chess pieces.
  QRadialGradient gradient(
      QPointF(m_offset + _col * m_distance, m_offset + _row * m_distance),
      m_radius + 5);

  gradient.setColorAt(0, QColor(255, 0, 0, 255));
  gradient.setColorAt(0.95, QColor(239, 196, 149, 255));
  gradient.setColorAt(1, QColor(0, 0, 0, 255));

  painter->setBrush(gradient);

  painter->setPen(QPen(Qt::black, 2)); // Black border
  painter->drawEllipse(
      QPoint(m_offset + _col * m_distance, m_offset + _row * m_distance), 30,
      30);

  painter->setBrush(QBrush(QColor(246, 207, 166)));
  painter->setPen(QPen(Qt::black, 1)); // Black border
  painter->drawEllipse(
      QPoint(m_offset + _col * m_distance, m_offset + _row * m_distance), 25,
      25);

  if (_selected) {
    painter->setPen(QPen(Qt::darkRed, 3));
    painter->drawEllipse(
        QPoint(m_offset + _col * m_distance, m_offset + _row * m_distance), 25,
        25);
  }

  QFont font("Microsoft Yahei", 25, QFont::Bold);
  painter->setFont(font);
  if (_color == ChessColor::RED)
    painter->setPen(QPen(Qt::red, 2));
  else
    painter->setPen(QPen(Qt::black, 2));
  painter->drawText(m_offset + _col * m_distance - 25,
                    m_offset + _row * m_distance - 25, 50, 50, Qt::AlignCenter,
                    chessTypeToString(_type, _color));
}

locationHint::locationHint(QGraphicsItem *parent) : QGraphicsObject(parent) {}

locationHint::locationHint(int row, int col, QGraphicsItem *parent)
    : _row(row), _col(col), QGraphicsObject(parent) {}

QRectF locationHint::boundingRect() const { return QRectF(0, 0, 60, 60); }

void locationHint::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  double m_offset = 30;
  double m_distance = 60;
  double m_radius = m_distance / 2;

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(QColor(0, 199, 245, 100), 2));
  if (_show) {

    QRect rect(_col * 60 + 5, _row * 60 + 5, 50, 50);

    int radius = 10;
    painter->drawArc(rect.left(), rect.top(), radius, radius, 90 * 16, 90 * 16);
    painter->drawArc(rect.right() - radius, rect.top(), radius, radius, 0,
                     90 * 16);
    painter->drawArc(rect.left(), rect.bottom() - radius, radius, radius,
                     180 * 16, 90 * 16);
    painter->drawArc(rect.right() - radius, rect.bottom() - radius, radius,
                     radius, 270 * 16, 90 * 16);
  }
}

