/**
 * @file
 * @brief Реализация модели игры Змейка (MVVM: Model).
 */

#include "backend.h"

namespace s21 {

// ─────────────────────────────────────────────────────────────────────────────
// Конструктор / деструктор
// ─────────────────────────────────────────────────────────────────────────────

SnakeModel::SnakeModel() {
  LoadHighScore();
  Reset();
}

SnakeModel::~SnakeModel() { SaveHighScore(); }

// ─────────────────────────────────────────────────────────────────────────────
// Команды
// ─────────────────────────────────────────────────────────────────────────────

void SnakeModel::Reset() {
  snake_.clear();
  // Начальная позиция: 4 сегмента по столбцу 5, голова наверху
  snake_.push_back({5, 10});
  snake_.push_back({6, 10});
  snake_.push_back({7, 10});
  snake_.push_back({8, 10});
  dir_ = 'U';
  next_dir_ = 'U';
  score_ = 0;
  level_ = 1;
  speed_ = kBaseSpeed;
  game_over_ = false;
  apple_eaten_ = false;
  SpawnApple();
}

void SnakeModel::MoveForward() {
  apple_eaten_ = false;
  game_over_ = false;

  // Применяем буферизированное направление
  dir_ = next_dir_;

  // Вычисляем новую позицию головы
  auto [row, col] = snake_.front();
  if (dir_ == 'U')
    --row;
  else if (dir_ == 'D')
    ++row;
  else if (dir_ == 'L')
    --col;
  else if (dir_ == 'R')
    ++col;

  std::pair<int, int> new_head = {row, col};

  // Проверка столкновения со стенками
  if (row < 0 || row >= kFieldHeight || col < 0 || col >= kFieldWidth) {
    game_over_ = true;
    return;
  }

  // Проверка столкновения с телом (исключая последний хвост — он уйдёт)
  for (std::size_t i = 0; i + 1 < snake_.size(); ++i) {
    if (snake_[i] == new_head) {
      game_over_ = true;
      return;
    }
  }

  // Проверка поедания яблока
  if (new_head == apple_pos_) {
    apple_eaten_ = true;
    // Хвост не удаляем — змейка растёт
  } else {
    snake_.pop_back();  // Удаляем хвост — змейка движется
  }

  // Добавляем новую голову
  snake_.insert(snake_.begin(), new_head);

  // Обновляем счёт, уровень и генерируем новое яблоко
  if (apple_eaten_) {
    EatApple();
  }
}

void SnakeModel::ChangeDirection(char direction) {
  // Игнорируем разворот на 180°
  if (!IsOppositeDirection(direction)) {
    next_dir_ = direction;
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Запросы
// ─────────────────────────────────────────────────────────────────────────────

bool SnakeModel::IsGameOver() const { return game_over_; }

bool SnakeModel::HasWon() const {
  return static_cast<int>(snake_.size()) >= kWinLength;
}

bool SnakeModel::IsAppleEaten() const { return apple_eaten_; }

std::vector<std::pair<int, int>> SnakeModel::GetSnakePositions() const {
  return snake_;
}

std::pair<int, int> SnakeModel::GetApplePosition() const { return apple_pos_; }

int SnakeModel::GetScore() const { return score_; }

int SnakeModel::GetHighScore() const { return high_score_; }

int SnakeModel::GetLevel() const { return level_; }

long long SnakeModel::GetSpeed() const { return speed_; }

// ─────────────────────────────────────────────────────────────────────────────
// Приватные методы
// ─────────────────────────────────────────────────────────────────────────────

bool SnakeModel::IsOppositeDirection(char dir) const {
  if (dir_ == 'U' && dir == 'D') return true;
  if (dir_ == 'D' && dir == 'U') return true;
  if (dir_ == 'L' && dir == 'R') return true;
  if (dir_ == 'R' && dir == 'L') return true;
  return false;
}

void SnakeModel::SpawnApple() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> dist_row(0, kFieldHeight - 1);
  std::uniform_int_distribution<> dist_col(0, kFieldWidth - 1);

  std::pair<int, int> candidate;
  bool on_snake = false;

  // Генерируем позицию, пока она не окажется свободной
  do {
    candidate = {dist_row(rng), dist_col(rng)};
    on_snake = false;
    for (const auto &seg : snake_) {
      if (seg == candidate) {
        on_snake = true;
        break;
      }
    }
  } while (on_snake);

  apple_pos_ = candidate;
}

void SnakeModel::EatApple() {
  ++score_;
  if (score_ > high_score_) {
    high_score_ = score_;
    SaveHighScore();
  }
  UpdateLevelAndSpeed();
  SpawnApple();
}

void SnakeModel::UpdateLevelAndSpeed() {
  // Новый уровень каждые 5 яблок, максимум kMaxLevel
  level_ = 1 + score_ / 5;
  if (level_ > kMaxLevel) level_ = kMaxLevel;

  // Скорость: (11 - level) * 50 мс, в наносекундах
  speed_ = static_cast<long long>(11 - level_) * 50000000LL;
}

void SnakeModel::LoadHighScore() {
  std::ifstream file(kHiScoreFile);
  if (file.is_open()) {
    file >> high_score_;
    file.close();
  } else {
    high_score_ = 0;
  }
}

void SnakeModel::SaveHighScore() {
  std::ofstream file(kHiScoreFile);
  if (file.is_open()) {
    file << high_score_;
    file.close();
  }
}

}  // namespace s21
