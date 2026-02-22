/**
 * @file
 * @brief Основные функции работы игры.
 */

#include "backend.h"

namespace s21 {

SnakeModel::SnakeModel() {
  // Инициализация начальной длины змейки и яблоко
  snake_.push_back({5, 5});
  snake_.push_back({5, 6});
  snake_.push_back({5, 7});
  snake_.push_back({5, 8});
  SpawnApple();
}

SnakeModel::~SnakeModel() { SaveHighScore(); }

void SnakeModel::MoveForward() {
  // Логика передвижения вперёд
  // Проверка на столкновение с телом змейки и границами поля
  CheckCollision();
  // Передвижение головы и хвоста змейки
}

void SnakeModel::ChangeDirection(char direction) {
  // Обрабатываем смену направления
  dir_ = direction;
}

bool SnakeModel::IsGameOver() const {
  // TODO: проверка столкновения с полем или телом
  return false;
}

bool SnakeModel::HasWon() const {
  // TODO: длина змейки >= 200
  return snake_.size() >= 200;
}

void SnakeModel::EatApple() {
  // TODO: увеличить длину змейки и сгенерировать новое яблоко
  SpawnApple();
}

void SnakeModel::UpdateLevelAndSpeed() {
  // TODO: обновить level_ и speed_ по score_
}

std::vector<std::pair<int, int>> SnakeModel::GetSnakePositions() const {
  return snake_;
}

std::pair<int, int> SnakeModel::GetApplePosition() const { return apple_pos_; }

int SnakeModel::GetScore() const { return score_; }

int SnakeModel::GetHighScore() const { return high_score_; }

int SnakeModel::GetLevel() const { return level_; }

int SnakeModel::GetSpeed() const { return speed_; }

void SnakeModel::CheckCollision() {
  // Проверка условий поражения (столкновения с полем или телом)
  // Признак выигрыша (длина змейки достигла 200)
}

void SnakeModel::SpawnApple() {
  // Случайная позиция для яблока
  std::mt19937 rng;
  std::uniform_int_distribution<> dist(0, 9);
  apple_pos_ = {dist(rng), dist(rng)};
}

void SnakeModel::SaveHighScore() {
  // Сохранение максимального счёта в файл
}

}  // namespace s21