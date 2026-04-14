/**
 * @file
 * @brief Реализация Qt-виджета игрового поля Змейки (MVVM: View).
 */

#include "snake_widget.h"

#include <algorithm>

#include "../../brick_game/snake/backend.h"

namespace s21 {

// ─────────────────────────────────────────────────────────────────────────────
// Конструктор
// ─────────────────────────────────────────────────────────────────────────────

SnakeWidget::SnakeWidget(SnakeController &controller, QWidget *parent)
    : QWidget(parent), controller_(controller) {
  setFocusPolicy(Qt::StrongFocus);

  // Фиксированный размер окна: поле + HUD + отступы
  setFixedSize(kPad + fieldPxW() + kPad + kHudWidth + kPad,
               kPad + fieldPxH() + kPad);

  // Таймер ~60 fps: вызывает Tick() (проверка внутреннего таймера ViewModel)
  // и перерисовку
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &SnakeWidget::onTimer);
  timer_->start(16);
}

// ─────────────────────────────────────────────────────────────────────────────
// Вспомогательные
// ─────────────────────────────────────────────────────────────────────────────

int SnakeWidget::fieldPxW() const {
  return SnakeModel::kFieldWidth * kCellSize;
}

int SnakeWidget::fieldPxH() const {
  return SnakeModel::kFieldHeight * kCellSize;
}

// ─────────────────────────────────────────────────────────────────────────────
// Слот таймера
// ─────────────────────────────────────────────────────────────────────────────

void SnakeWidget::onTimer() {
  controller_.Tick(); // ViewModel проверяет внутренний таймер скорости
  update();           // запрос перерисовки
}

// ─────────────────────────────────────────────────────────────────────────────
// Ввод пользователя (View → ViewModel)
// ─────────────────────────────────────────────────────────────────────────────

void SnakeWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Up:
    controller_.ProcessUserInput('U');
    break;
  case Qt::Key_Down:
    controller_.ProcessUserInput('D');
    break;
  case Qt::Key_Left:
    controller_.ProcessUserInput('L');
    break;
  case Qt::Key_Right:
    controller_.ProcessUserInput('R');
    break;
  case Qt::Key_Return:
  case Qt::Key_Enter:
    controller_.ProcessUserInput('\n');
    break;
  case Qt::Key_P:
    controller_.ProcessUserInput('p');
    break;
  case Qt::Key_Q:
    controller_.ProcessUserInput('q');
    window()->close();
    break;
  default:
    QWidget::keyPressEvent(event);
    return;
  }
  update();
}

// ─────────────────────────────────────────────────────────────────────────────
// Рендеринг (ViewModel → View)
// ─────────────────────────────────────────────────────────────────────────────

void SnakeWidget::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);

  drawBackground(p);
  drawGrid(p);
  drawSnake(p);
  drawApple(p);
  drawHud(p);
  drawOverlay(p);
}

// ─── Фон ─────────────────────────────────────────────────────────────────────

void SnakeWidget::drawBackground(QPainter &p) {
  // Общий фон окна
  p.fillRect(rect(), QColor(18, 18, 28));

  // Фон игрового поля
  QRect field(kPad, kPad, fieldPxW(), fieldPxH());
  p.fillRect(field, QColor(26, 26, 40));

  // Рамка поля
  p.setPen(QPen(QColor(60, 60, 100), 2));
  p.drawRect(field);
}

// ─── Сетка ───────────────────────────────────────────────────────────────────

void SnakeWidget::drawGrid(QPainter &p) {
  p.setPen(QPen(QColor(35, 35, 52), 1));

  for (int r = 1; r < SnakeModel::kFieldHeight; ++r) {
    int y = kPad + r * kCellSize;
    p.drawLine(kPad, y, kPad + fieldPxW(), y);
  }
  for (int c = 1; c < SnakeModel::kFieldWidth; ++c) {
    int x = kPad + c * kCellSize;
    p.drawLine(x, kPad, x, kPad + fieldPxH());
  }
}

// ─── Змейка ──────────────────────────────────────────────────────────────────

void SnakeWidget::drawSnake(QPainter &p) {
  const auto positions = controller_.GetSnakePositions();
  bool is_head = true;

  for (const auto &[row, col] : positions) {
    int x = kPad + col * kCellSize + 2;
    int y = kPad + row * kCellSize + 2;
    int sz = kCellSize - 4;

    if (is_head) {
      p.setBrush(QColor(80, 230, 110));
      p.setPen(QPen(QColor(40, 190, 70), 1));
    } else {
      p.setBrush(QColor(45, 160, 65));
      p.setPen(QPen(QColor(30, 120, 45), 1));
    }
    p.drawRoundedRect(x, y, sz, sz, 5, 5);
    is_head = false;
  }
}

// ─── Яблоко ──────────────────────────────────────────────────────────────────

void SnakeWidget::drawApple(QPainter &p) {
  const auto [row, col] = controller_.GetApplePosition();
  int x = kPad + col * kCellSize + 3;
  int y = kPad + row * kCellSize + 3;
  int sz = kCellSize - 6;

  p.setBrush(QColor(220, 55, 55));
  p.setPen(QPen(QColor(180, 20, 20), 1));
  p.drawEllipse(x, y, sz, sz);
}

// ─── HUD (статистика) ────────────────────────────────────────────────────────

void SnakeWidget::drawHud(QPainter &p) {
  int hudX = kPad + fieldPxW() + kPad;
  int hudW = kHudWidth - kPad;

  QFont labelFont("Monospace", 9, QFont::Normal);
  QFont valueFont("Monospace", 15, QFont::Bold);

  // Вспомогательная лямбда: рисует блок с подписью и значением
  auto drawBlock = [&](int y, const QString &label, const QString &value) {
    p.fillRect(hudX, y, hudW, 58, QColor(32, 32, 50));
    p.setPen(QPen(QColor(60, 60, 100), 1));
    p.drawRect(hudX, y, hudW, 58);

    p.setPen(QColor(110, 110, 160));
    p.setFont(labelFont);
    p.drawText(QRect(hudX, y + 6, hudW, 18), Qt::AlignCenter, label);

    p.setPen(QColor(220, 230, 255));
    p.setFont(valueFont);
    p.drawText(QRect(hudX, y + 26, hudW, 26), Qt::AlignCenter, value);
  };

  int hi = std::max(controller_.GetHighScore(), controller_.GetScore());
  drawBlock(kPad, "HI-SCORE", QString("%1").arg(hi, 6, 10, QChar('0')));
  drawBlock(kPad + 68, "SCORE",
            QString("%1").arg(controller_.GetScore(), 6, 10, QChar('0')));
  drawBlock(kPad + 136, "LEVEL", QString::number(controller_.GetLevel()));
  drawBlock(kPad + 204, "SPEED",
            QString("0.%1").arg(controller_.GetSpeed() / 50000000LL));

  // Подсказки управления
  p.setFont(QFont("Monospace", 8));
  p.setPen(QColor(80, 80, 120));
  int tipY = kPad + 275;
  for (const char *tip : {"↑↓←→  move", "P      pause", "Q      quit"}) {
    p.drawText(QRect(hudX, tipY, hudW, 18), Qt::AlignLeft, QString("  ") + tip);
    tipY += 18;
  }
}

// ─── Оверлейные сообщения ────────────────────────────────────────────────────

void SnakeWidget::drawOverlay(QPainter &p) {
  const int status = controller_.GetPauseStatus();
  const SnakeState state = controller_.GetState();

  QRect fieldRect(kPad, kPad, fieldPxW(), fieldPxH());

  auto fillOverlay = [&]() { p.fillRect(fieldRect, QColor(0, 0, 0, 165)); };

  if (state == SnakeState::START) {
    fillOverlay();
    p.setPen(QColor(80, 230, 110));
    p.setFont(QFont("Monospace", 28, QFont::Bold));
    p.drawText(QRect(kPad, kPad, fieldPxW(), fieldPxH() / 2), Qt::AlignCenter,
               "SNAKE");

    p.setPen(QColor(190, 190, 240));
    p.setFont(QFont("Monospace", 12));
    p.drawText(QRect(kPad, kPad + fieldPxH() / 2, fieldPxW(), fieldPxH() / 2),
               Qt::AlignCenter, "Press ENTER to start");

  } else if (status == 1) {
    fillOverlay();
    p.setPen(QColor(255, 215, 60));
    p.setFont(QFont("Monospace", 26, QFont::Bold));
    p.drawText(QRect(kPad, kPad, fieldPxW(), fieldPxH() / 2), Qt::AlignCenter,
               "PAUSED");

    p.setPen(QColor(190, 190, 240));
    p.setFont(QFont("Monospace", 12));
    p.drawText(QRect(kPad, kPad + fieldPxH() / 2, fieldPxW(), fieldPxH() / 2),
               Qt::AlignCenter, "P to resume");

  } else if (status == 3) {
    bool won = static_cast<int>(controller_.GetSnakePositions().size()) >=
               SnakeModel::kWinLength;
    fillOverlay();

    p.setPen(won ? QColor(80, 230, 110) : QColor(220, 55, 55));
    p.setFont(QFont("Monospace", 24, QFont::Bold));
    p.drawText(QRect(kPad, kPad, fieldPxW(), fieldPxH() / 2), Qt::AlignCenter,
               won ? "YOU WIN!" : "GAME OVER");

    p.setPen(QColor(190, 190, 240));
    p.setFont(QFont("Monospace", 11));
    p.drawText(QRect(kPad, kPad + fieldPxH() / 2, fieldPxW(), fieldPxH() / 2),
               Qt::AlignCenter, "ENTER — restart\n  Q   — quit");
  }
}

} // namespace s21
