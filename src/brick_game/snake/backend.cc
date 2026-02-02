/**
 * @file
 * @brief Основные функции работы игры.
 */

#include "snake_model.hpp"

namespace s21 {

SnakeModel::SnakeModel() {
    // Инициализация начальной длины змейки и яблоко
    snake_.push_back({5, 5});
    snake_.push_back({5, 6});
    snake_.push_back({5, 7});
    snake_.push_back({5, 8});
    SpawnApple();
}

SnakeModel::~SnakeModel() {
    SaveHighScore();
}

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

// Другие методы реализации...

}  // namespace s21