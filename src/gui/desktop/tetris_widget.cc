/**
 * @file
 * @brief Реализация Qt-виджета игрового поля Тетриса (View).
 */

#include "tetris_widget.h"

#include <algorithm>

namespace s21 {

// ─────────────────────────────────────────────────────────────────────────────
// Таблица цветов — соответствует init_pair() в main.cc (ncurses-версия)
// ─────────────────────────────────────────────────────────────────────────────

QColor TetrisWidget::pieceColor(int idx) {
  static const QColor kColors[] = {
      QColor(26, 26, 40),     // 0 — пусто (фон поля)
      QColor(220, 220, 220),  // 1 — WHITE
      QColor(0, 200, 220),    // 2 — CYAN
      QColor(60, 200, 60),    // 3 — GREEN
      QColor(200, 60, 200),   // 4 — MAGENTA
      QColor(220, 60, 60),    // 5 — RED
      QColor(60, 100, 220),   // 6 — BLUE
      QColor(230, 210, 50),   // 7 — YELLOW
  };
  if (idx < 0 || idx > 7) return kColors[0];
  return kColors[idx];
}

// ─────────────────────────────────────────────────────────────────────────────
// Конструктор
// ─────────────────────────────────────────────────────────────────────────────

TetrisWidget::TetrisWidget(QWidget *parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  setFixedSize(kPad + fieldPxW() + kPad + kHudWidth + kPad,
               kPad + fieldPxH() + kPad);

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &TetrisWidget::onTimer);
  // Таймер запускается явно через startGame()
}

// ─────────────────────────────────────────────────────────────────────────────
// Управление игрой
// ─────────────────────────────────────────────────────────────────────────────

void TetrisWidget::startGame() {
  // Инициализируем состояние (pause=2 = START)
  userInput(UserAction_t::ERRENUM,
            false);  // ← инициализирует глобальное состояние
  gameinfo_ = updateCurrentState();
  timer_->start(16);
}

void TetrisWidget::stopGame() { timer_->stop(); }

// ─────────────────────────────────────────────────────────────────────────────
// Слот таймера
// ─────────────────────────────────────────────────────────────────────────────

void TetrisWidget::onTimer() {
  // Передаём «нет действия» — FSM сам обработает падение по таймеру
  userInput(UserAction_t::ERRENUM, false);
  gameinfo_ = updateCurrentState();

  if (gameinfo_.pause == -1) {
    stopGame();
    emit gameEnded();
    return;
  }

  update();
}

// ─────────────────────────────────────────────────────────────────────────────
// Ввод пользователя (View → Backend)
// ─────────────────────────────────────────────────────────────────────────────

void TetrisWidget::keyPressEvent(QKeyEvent *event) {
  UserAction_t action = UserAction_t::ERRENUM;

  switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
      action = UserAction_t::Start;
      break;
    case Qt::Key_P:
      action = UserAction_t::Pause;
      break;
    case Qt::Key_Left:
      action = UserAction_t::Left;
      break;
    case Qt::Key_Right:
      action = UserAction_t::Right;
      break;
    case Qt::Key_Down:
      action = UserAction_t::Down;
      break;
    case Qt::Key_Space:
      action = UserAction_t::Action;
      break;  // поворот
    case Qt::Key_Q:
      timer_->stop();
      emit gameEnded();
      return;
    default:
      QWidget::keyPressEvent(event);
      return;
  }

  userInput(action, false);
  gameinfo_ = updateCurrentState();
  update();
}

// ─────────────────────────────────────────────────────────────────────────────
// Рендеринг (Backend → View)
// ─────────────────────────────────────────────────────────────────────────────

void TetrisWidget::paintEvent(QPaintEvent *) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);

  drawBackground(p);
  drawField(p);
  drawNextPiece(p);
  drawHud(p);
  drawOverlay(p);
}

// ─── Фон ─────────────────────────────────────────────────────────────────────

void TetrisWidget::drawBackground(QPainter &p) {
  p.fillRect(rect(), QColor(18, 18, 28));

  QRect field(kPad, kPad, fieldPxW(), fieldPxH());
  p.fillRect(field, QColor(26, 26, 40));

  p.setPen(QPen(QColor(60, 60, 100), 2));
  p.drawRect(field);
}

// ─── Поле (field[Y_START+2 .. Y_FIELD+2][X_START .. X_FIELD]) ───────────────

void TetrisWidget::drawField(QPainter &p) {
  if (!gameinfo_.field) return;

  for (int row = 0; row < Y_FIELD; ++row) {
    for (int col = 0; col < X_FIELD; ++col) {
      // Индексация совпадает с ncurses-фронтэндом: field[row+2][col]
      int color_idx = gameinfo_.field[row + Y_START + 2][col + X_START];

      int x = kPad + col * kCellSize;
      int y = kPad + row * kCellSize;

      if (color_idx > 0) {
        QColor c = pieceColor(color_idx);
        p.setBrush(c);
        p.setPen(QPen(c.darker(150), 1));
        p.drawRect(x + 1, y + 1, kCellSize - 2, kCellSize - 2);
      } else {
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(QColor(35, 35, 52), 1));
        p.drawRect(x, y, kCellSize, kCellSize);
      }
    }
  }
}

// ─── Следующая фигура в HUD
// ───────────────────────────────────────────────────

void TetrisWidget::drawNextPiece(QPainter &p) {
  if (!gameinfo_.next) return;

  int hudX = kPad + fieldPxW() + kPad;
  // Область «NEXT» в HUD: маленькая сетка 4×4, ячейка 20px
  static constexpr int kNextCell = 20;
  static constexpr int kNextTop = 60;  // отступ сверху в HUD

  for (int r = 0; r < MAX_FIGURE_SIZE; ++r) {
    for (int c = 0; c < MAX_FIGURE_SIZE; ++c) {
      int color_idx = gameinfo_.next[r][c];
      int x = hudX + 10 + c * kNextCell;
      int y = kNextTop + r * kNextCell;

      if (color_idx > 0) {
        QColor col = pieceColor(color_idx);
        p.setBrush(col);
        p.setPen(QPen(col.darker(150), 1));
        p.drawRect(x + 1, y + 1, kNextCell - 2, kNextCell - 2);
      } else {
        p.setPen(QPen(QColor(40, 40, 60), 1));
        p.setBrush(Qt::transparent);
        p.drawRect(x, y, kNextCell, kNextCell);
      }
    }
  }
}

// ─── HUD (статистика) ────────────────────────────────────────────────────────

void TetrisWidget::drawHud(QPainter &p) {
  int hudX = kPad + fieldPxW() + kPad;
  int hudW = kHudWidth - kPad;

  QFont labelFont("Monospace", 9);
  QFont valueFont("Monospace", 15, QFont::Bold);

  // Блок: подпись + значение
  auto drawBlock = [&](int y, const QString &label, const QString &value) {
    p.fillRect(hudX, y, hudW, 50, QColor(32, 32, 50));
    p.setPen(QPen(QColor(60, 60, 100), 1));
    p.drawRect(hudX, y, hudW, 50);

    p.setPen(QColor(110, 110, 160));
    p.setFont(labelFont);
    p.drawText(QRect(hudX, y + 5, hudW, 16), Qt::AlignCenter, label);

    p.setPen(QColor(220, 230, 255));
    p.setFont(valueFont);
    p.drawText(QRect(hudX, y + 23, hudW, 22), Qt::AlignCenter, value);
  };

  int hi = std::max(gameinfo_.high_score, gameinfo_.score);
  drawBlock(kPad, "HI-SCORE", QString("%1").arg(hi, 6, 10, QChar('0')));
  drawBlock(kPad + 58, "SCORE",
            QString("%1").arg(gameinfo_.score, 6, 10, QChar('0')));
  drawBlock(kPad + 116, "LEVEL", QString::number(gameinfo_.level));
  drawBlock(kPad + 174, "SPEED", QString::number(gameinfo_.speed));

  // Заголовок «NEXT»
  p.setPen(QColor(110, 110, 160));
  p.setFont(labelFont);
  p.drawText(QRect(hudX, kPad + 232, hudW, 16), Qt::AlignCenter, "NEXT");
  // (сама фигура рисуется в drawNextPiece)

  // Управление
  p.setFont(QFont("Monospace", 8));
  p.setPen(QColor(80, 80, 120));
  int tipY = kPad + 330;
  for (const char *tip :
       {"←→  move", "↓    drop", "SPC  rotate", "P    pause", "Q    quit"}) {
    p.drawText(QRect(hudX, tipY, hudW, 16), Qt::AlignLeft, QString("  ") + tip);
    tipY += 16;
  }
}

// ─── Оверлеи ─────────────────────────────────────────────────────────────────

void TetrisWidget::drawOverlay(QPainter &p) {
  QRect fieldRect(kPad, kPad, fieldPxW(), fieldPxH());

  auto fillOverlay = [&]() { p.fillRect(fieldRect, QColor(0, 0, 0, 170)); };

  if (gameinfo_.pause == 2) {
    // START
    fillOverlay();
    p.setPen(QColor(0, 200, 220));
    p.setFont(QFont("Monospace", 26, QFont::Bold));
    p.drawText(QRect(kPad, kPad, fieldPxW(), fieldPxH() / 2), Qt::AlignCenter,
               "TETRIS");

    p.setPen(QColor(190, 190, 240));
    p.setFont(QFont("Monospace", 12));
    p.drawText(QRect(kPad, kPad + fieldPxH() / 2, fieldPxW(), fieldPxH() / 2),
               Qt::AlignCenter, "Press ENTER to start");

  } else if (gameinfo_.pause == 1) {
    // PAUSE
    fillOverlay();
    p.setPen(QColor(255, 215, 60));
    p.setFont(QFont("Monospace", 26, QFont::Bold));
    p.drawText(QRect(kPad, kPad, fieldPxW(), fieldPxH() / 2), Qt::AlignCenter,
               "PAUSED");

    p.setPen(QColor(190, 190, 240));
    p.setFont(QFont("Monospace", 12));
    p.drawText(QRect(kPad, kPad + fieldPxH() / 2, fieldPxW(), fieldPxH() / 2),
               Qt::AlignCenter, "P to resume");

  } else if (gameinfo_.pause == 3) {
    // GAME OVER
    fillOverlay();
    p.setPen(QColor(220, 55, 55));
    p.setFont(QFont("Monospace", 22, QFont::Bold));
    p.drawText(QRect(kPad, kPad, fieldPxW(), fieldPxH() / 2), Qt::AlignCenter,
               "GAME OVER");

    p.setPen(QColor(190, 190, 240));
    p.setFont(QFont("Monospace", 11));
    p.drawText(QRect(kPad, kPad + fieldPxH() / 2, fieldPxW(), fieldPxH() / 2),
               Qt::AlignCenter, "ENTER — restart\n  Q   — quit");
  }
}

}  // namespace s21
