/**
 * @file
 * @brief Qt-виджет игрового поля Тетриса (View).
 *
 * Работает через C API Тетриса из BrickGame v1.0:
 *   userInput(action, hold)  — ввод пользователя
 *   updateCurrentState()     — получение состояния GameInfo_t
 *
 * Архитектура совпадает со Snake View (Qt):
 * View (TetrisWidget) ↔ ViewModel/Backend (C API) ↔ Model (C static state)
 */

#pragma once

#include <QFont>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/tetris/backend.h"

namespace s21 {

/**
 * @brief Qt-виджет игрового поля Тетриса.
 *
 * Отображает поле, следующую фигуру, HUD и оверлеи.
 * Ввод транслируется в UserAction_t и передаётся в userInput().
 * QTimer периодически вызывает userInput(ERRENUM) для авто-падения фигуры.
 */
class TetrisWidget : public QWidget {
  Q_OBJECT

public:
  explicit TetrisWidget(QWidget *parent = nullptr);
  ~TetrisWidget() override = default;

  /// Запустить игру (стартует таймер)
  void startGame();
  /// Остановить игру (останавливает таймер)
  void stopGame();

signals:
  /// Игра завершена (Q нажата или pause == -1)
  void gameEnded();

protected:
  void paintEvent(QPaintEvent *) override;
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  void onTimer();

private:
  QTimer *timer_;
  GameInfo_t gameinfo_{};

  // ─── Константы разметки ──────────────────────────────────────────────────
  static constexpr int kCellSize = 30;  ///< px на клетку
  static constexpr int kHudWidth = 180; ///< ширина HUD
  static constexpr int kPad = 10;       ///< внешний отступ

  int fieldPxW() const { return X_FIELD * kCellSize; }
  int fieldPxH() const { return Y_FIELD * kCellSize; }

  /// Таблица цветов фигур (COLOR_PAIR 1-7 → QColor)
  static QColor pieceColor(int idx);

  // ─── Рендеринг ───────────────────────────────────────────────────────────
  void drawBackground(QPainter &p);
  void drawField(QPainter &p);
  void drawNextPiece(QPainter &p);
  void drawHud(QPainter &p);
  void drawOverlay(QPainter &p);
};

} // namespace s21
