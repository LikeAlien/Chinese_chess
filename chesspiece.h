#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <QGraphicsObject>

enum class ChessType { NONE, JIANG, SHI, XIANG, MA, CHE, PAO, BING };
enum class ChessColor { NONE, RED, BLACK };

class locationHint : public QGraphicsObject {
public:
  explicit locationHint(QGraphicsItem *parent = nullptr);
  explicit locationHint(int row, int col, QGraphicsItem *parent = nullptr);
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
  int row() const { return _row; }
  int col() const { return _col; }
  bool show() const { return _show; }
  void setShow(bool show) { _show = show; }

private:
  int _row;
  int _col;
  bool _show = false;
};

QString chessTypeToString(ChessType type, ChessColor color);


class chessPiece : public QGraphicsObject {

public:
  explicit chessPiece(QGraphicsItem *parent = nullptr);
  explicit chessPiece(ChessType type, ChessColor color, int row, int col,
                      QGraphicsItem *parent = nullptr);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

  ChessType getType() const { return _type; }
  ChessColor getColor() const { return _color; }
  bool isDead() const { return _dead; }
  void setDead(bool dead) { _dead = dead; }
  int row() const { return _row; }
  int col() const { return _col; }
  bool isSelected() const { return _selected; }
  void setSelected(bool selected) { _selected = selected; }
  void setRow(int row) { _row = row; }
  void setCol(int col) { _col = col; }
private:
  ChessType _type = ChessType::NONE;
  ChessColor _color = ChessColor::NONE;
  bool _dead = false;
  int _row = 0;
  int _col = 0;
  bool _selected = false;
};
#endif // CHESSPIECE_H
