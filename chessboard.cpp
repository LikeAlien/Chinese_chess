#include "chessboard.h"
#include <QPainter>
#include <qmainwindow.h>
#include <qobject.h>

chessBoard::chessBoard(QGraphicsItem *parent) : QGraphicsObject(parent) {}

QRectF chessBoard::boundingRect() const { return QRectF(0, 0, 750, 560); }

void chessBoard::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setRenderHint(QPainter::Antialiasing, true);

  double m_offset = 30;
  double m_distance = 60;
  double m_radius = m_distance / 2;

  // Draw a rectangular background
  painter->setPen(Qt::NoPen);
  painter->setBrush(QBrush(QColor(169, 89, 32)));
  painter->drawRect(0, 0, 540, 600);
  // Create a centered gradient.
  QRadialGradient gradient(QPointF(270, 300), 300);

  gradient.setColorAt(0, QColor(249, 215, 172, 255));
  gradient.setColorAt(0.8, QColor(245, 200, 154, 255));
  gradient.setColorAt(1, QColor(231, 173, 125, 255));

  // Set the brush to gradient
  painter->setBrush(gradient);
  painter->drawRect(5, 5, 530, 590);

  painter->setPen(QPen(Qt::black, 2));
  for (int i = 0; i < 10; i++) {
    painter->drawLine(
        QPoint(m_offset, m_offset + i * m_distance),
        QPoint(m_offset + 8 * m_distance, m_offset + i * m_distance));
  }
  for (int i = 0; i < 9; i++) {
    if (i == 0 || i == 8) {
      painter->drawLine(
          QPoint(m_offset + i * m_distance, m_offset),
          QPoint(m_offset + i * m_distance, m_offset + 9 * m_distance));
    } else {
      painter->drawLine(
          QPoint(m_offset + i * m_distance, m_offset),
          QPoint(m_offset + i * m_distance, m_offset + 4 * m_distance));
      painter->drawLine(
          QPoint(m_offset + i * m_distance, m_offset + 5 * m_distance),
          QPoint(m_offset + i * m_distance, m_offset + 9 * m_distance));
    }
  }

  painter->drawLine(
      QPoint(m_offset + 3 * m_distance, m_offset),
      QPoint(m_offset + 5 * m_distance, m_offset + 2 * m_distance));
  painter->drawLine(
      QPoint(m_offset + 3 * m_distance, m_offset + 2 * m_distance),
      QPoint(m_offset + 5 * m_distance, m_offset));
  painter->drawLine(
      QPoint(m_offset + 3 * m_distance, m_offset + 7 * m_distance),
      QPoint(m_offset + 5 * m_distance, m_offset + 9 * m_distance));
  painter->drawLine(
      QPoint(m_offset + 3 * m_distance, m_offset + 9 * m_distance),
      QPoint(m_offset + 5 * m_distance, m_offset + 7 * m_distance));

  QRect rect1(m_offset + m_distance, m_offset + 4 * m_distance, m_distance,
              m_distance);
  QRect rect2(m_offset + 2 * m_distance, m_offset + 4 * m_distance, m_distance,
              m_distance);
  QRect rect3(m_offset + 5 * m_distance, m_offset + 4 * m_distance, m_distance,
              m_distance);
  QRect rect4(m_offset + 6 * m_distance, m_offset + 4 * m_distance, m_distance,
              m_distance);
  painter->setFont(QFont("FangSong", m_radius * 5 / 6, 800));
  painter->drawText(rect1, "楚", QTextOption(Qt::AlignCenter));
  painter->drawText(rect2, "河", QTextOption(Qt::AlignCenter));
  painter->drawText(rect3, "漢", QTextOption(Qt::AlignCenter));
  painter->drawText(rect4, "界", QTextOption(Qt::AlignCenter));
}