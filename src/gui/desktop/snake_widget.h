/**
 * @file
 * @brief Qt-виджет игрового поля Змейки (MVVM: View / QWidget).
 *
 * SnakeWidget — подписчик на ViewModel (SnakeController):
 *  - читает состояние через геттеры ViewModel (привязка ViewModel → View);
 *  - отправляет команды через ProcessUserInput() (привязка View → ViewModel).
 *
 * Рендеринг выполняется в paintEvent(), анимация — через QTimer.
 */

#pragma once

#include <QFont>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/fsm.h"

namespace s21 {

/**
 * @brief Виджет игрового поля Змейки (MVVM: View).
 *
 * Отображает игровое поле, змейку, яблоко, HUD и оверлейные сообщения.
 * Не содержит бизнес-логики — только рендеринг и передача ввода в ViewModel.
 */
class SnakeWidget : public QWidget {
  Q_OBJECT

public:
  explicit SnakeWidget(SnakeController &controller, QWidget *parent = nullptr);
  ~SnakeWidget() override = default;

  /// Запустить игру (стартует таймер, передаёт фокус)
  void startGame();
  /// Остановить игру (останавливает таймер)
  void stopGame();

signals:
  /// Игра завершена (Q нажата)
  void gameEnded();

protected:
  /// Рендеринг кадра — читает данные из ViewModel
  void paintEvent(QPaintEvent *event) override;
  /// Ввод пользователя — отправляет команды в ViewModel
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  /// Вызывается QTimer (~60 fps): Tick() + update()
  void onTimer();

private:
  SnakeController &controller_; ///< ViewModel (источник данных)
  QTimer *timer_;

  // ─── Константы разметки ──────────────────────────────────────────────────
  static constexpr int kCellSize = 25;  ///< Пикселей на клетку
  static constexpr int kHudWidth = 180; ///< Ширина HUD-панели
  static constexpr int kPad = 10;       ///< Внешний отступ

  int fieldPxW() const; ///< Ширина поля в пикселях
  int fieldPxH() const; ///< Высота поля в пикселях

  // ─── Методы рендеринга ───────────────────────────────────────────────────
  void drawBackground(QPainter &p);
  void drawGrid(QPainter &p);
  void drawSnake(QPainter &p);
  void drawApple(QPainter &p);
  void drawHud(QPainter &p);
  void drawOverlay(QPainter &p);
};

} // namespace s21
