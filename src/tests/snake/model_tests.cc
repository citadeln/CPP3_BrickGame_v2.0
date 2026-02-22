#include <gtest/gtest.h>

#include "../src/snake_model.hpp"

namespace s21 {

TEST(ModelTest, TestMovement) {
  SnakeModel model;
  model.MoveForward();
  // Проверка правильности передвижений
}

TEST(ModelTest, TestEatApple) {
  SnakeModel model;
  model.EatApple();
  // Проверка увеличения длины змейки
}

}  // namespace s21