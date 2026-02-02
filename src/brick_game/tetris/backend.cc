/**
 * @file
 * @brief Основные функции работы игры.
 */

#include "backend.h"
#include "./../../s21_namespace.h"

using namespace s21;

/**
 * @brief Основная функция обработки действий пользователя на основе нажатых
 * клавиш.
 *
 * Функция получает входящее значение нажатой клавиши пользователем
 * и в зависимости от состояния системы принимает решение о дальнейшем
 * выполнении вспомогательных функций.
 *
 * @param action ввод пользователя или его отсутсвие
 * @param hold параметр длительного зажатия клавиши
 */
void userInput(UserAction_t action, bool hold) {
  static Params_t *prms = nullptr;

  prms = create_prms(prms);

  // создание новой игры при gameover
  if (prms->gameinfo->pause == 3) prms = create_prms(NULL);

  chech_timer(prms);

  function func = fsm_table(prms->state, action);
  if (func) func(prms);

  prms->hold = (hold) ? true : false;
}

/**
 * @brief Функция, хранящая в массиве координаты фигуры относительно курсора.
 *
 * Функция принимает тип, состояние фигуры, номер блока и номер координаты,
 * и отдает текущую координату блока относительно блока-курсор.
 *
 * @param figure_type Текущий тип фигуры от 0 до 6
 * @param figure_state Текущее состояние фигуры от 0 до 3
 * @param block_number Номер блока фигуры от 0 до 3
 * @param coord Координата блока 0 - Y, 1 - X
 *
 * @return int Возвращает координату блока.
 */
int get_block_coord(int figure_type, int figure_state, int block_number,
                    int coord) {
  // [номер фигуры][состояние][кол-во блоков][координаты блока]
  static const int tetrominos[FIGURES][FIGURE_STATES][BLOCKS][2] = {
      // I
      {
          {{0, -2}, {0, -1}, {0, 0}, {0, 1}},
          {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
          {{0, -2}, {0, -1}, {0, 0}, {0, 1}},
          {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
      },
      // J
      {
          {{0, -1}, {0, 0}, {0, 1}, {1, 1}},
          {{-1, 0}, {-1, 1}, {0, 0}, {1, 0}},
          {{-1, -1}, {0, -1}, {0, 0}, {0, 1}},
          {{-1, 0}, {0, 0}, {1, 0}, {1, -1}},
      },
      // L
      {
          {{1, -1}, {0, -1}, {0, 0}, {0, 1}},
          {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},
          {{0, -1}, {0, 0}, {0, 1}, {-1, 1}},
          {{-1, -1}, {-1, 0}, {0, 0}, {1, 0}},
      },
      // O
      {
          {{0, -1}, {0, 0}, {1, -1}, {1, 0}},
          {{0, -1}, {0, 0}, {1, -1}, {1, 0}},
          {{0, -1}, {0, 0}, {1, -1}, {1, 0}},
          {{0, -1}, {0, 0}, {1, -1}, {1, 0}},
      },
      // S
      {
          {{0, 0}, {0, 1}, {1, -1}, {1, 0}},
          {{0, 0}, {-1, 0}, {0, 1}, {1, 1}},
          {{0, 0}, {0, 1}, {1, -1}, {1, 0}},
          {{0, 0}, {-1, 0}, {0, 1}, {1, 1}},
      },
      // T
      {
          {{0, -1}, {0, 0}, {0, 1}, {1, 0}},
          {{-1, 0}, {0, 0}, {0, 1}, {1, 0}},
          {{-1, 0}, {0, -1}, {0, 0}, {0, 1}},
          {{-1, 0}, {0, -1}, {0, 0}, {1, 0}},
      },
      // Z
      {
          {{0, 0}, {0, -1}, {1, 0}, {1, 1}},
          {{0, 0}, {1, 0}, {0, 1}, {-1, 1}},
          {{0, 0}, {0, -1}, {1, 0}, {1, 1}},
          {{0, 0}, {1, 0}, {0, 1}, {-1, 1}},
      }};

  return tetrominos[figure_type][figure_state][block_number][coord];
}

/**
 * @brief Функция, создающая основную статичную структуру, хранящую данные о
 * состоянии игры.
 *
 * При первом запуске создает статичную структуру со всеми данными о текущем
 * состоянии игры, заполняет ее стартовыми значениями, и отдает на нее
 * указатель.
 *
 * Структура содержит:
 *  - указатель на структуру с данными о поле и статистике,
 *  - указатель на структуру с данными о фигурах,
 *  - текущее состояние игры,
 *  - время для срабатывания движения вниз.
 *
 * При последующих запусках отдает только указатель на структуру.
 *
 * @param pointer указатель, если передается NULL, то структура создается со
 * стартовыми значенями. Используется при первом запуске или начале новой игры
 * после gameover.
 *
 * @return Params_t* Возвращает указатель на статичную структуру.
 */
Params_t *create_prms(Params_t *pointer) {
  static Params_t prms = {
    nullptr,               // figureinfo
    nullptr,               // gameinfo
    START,                 // state
    {},                    // start_time
    false                  // hold
};

  if (pointer == NULL) {
    prms.figureinfo = create_figure_info();
    prms.gameinfo = get_field_info(prms.figureinfo);
    prms.state = START;
    timespec_get(&prms.start_time, TIME_UTC);
    prms.hold = false;
  }
  return &prms;
}

/**
 * @brief Функция создает статичную структуру с данными по фигурам.
 *
 * Функция создает статичную структуру с данными по фигуре:
 *  - координаты блока-курсора, относительно которого будет отрисовываться
 * фигура
 *  - тип текушей фигуры
 *  - ее состояние
 *  - тип следующей фигуры
 *  - цвет текушей фигуры
 *  - цвет следующей фигуры
 *
 * @return Figureinfo_t* Возвращает указатель статичной структуры.
 */
Figureinfo_t *create_figure_info() {
  static Figureinfo_t current_figure = {
    0,              // y
    0,              // x
    rand() % 7,     // figure_type
    0,              // figure_state
    rand() % 7,     // next_figure_type
    1,              // figure_color
    1               // next_figure_color
};

  current_figure.y = Y_BLOCK;
  current_figure.x = X_BLOCK;
  current_figure.figure_type = rand() % 7;
  current_figure.figure_state = 0;
  current_figure.next_figure_type = rand() % 7;
  // цвета
  if (COLOR) {
    current_figure.figure_color = current_figure.figure_type + 1;
    current_figure.next_figure_color = current_figure.next_figure_type + 1;
  } else {
    current_figure.figure_color = 1;
    current_figure.next_figure_color = 1;
  }

  return &current_figure;
}

/**
 * @brief Функция получения указателя на структуру с данными по полю
 * и статистике.
 *
 * Функция возвращает указатель на статичную структуру с данными по полю и
 * статистике.
 *
 * @param figure_info Принимает указатель на структуры данных по фигурам.
 *
 * @return GameInfo_t* Возвращает указатель статичной структуры.
 */
GameInfo_t *get_field_info(Figureinfo_t *figure_info) {
  static GameInfo_t *field_info = NULL;

  if (figure_info != NULL) field_info = create_field_info(figure_info);

  return field_info;
}

/**
 * @brief Функция создает структуру с начальными данными для поля и статистики.
 *
 * Функция создает:
 *  - массив для поля 20+2х10 - 2 дополнительные строки отводятся
 * под появление фигуры и дальнейший спуск вниз на 2 строки, для проверки
 * условия gameover.
 *  - массив для поля будущей фигуры 4х4
 * Функция заполняет начальные данные для статистики:
 *  - счет,
 *  - рекорд,
 *  - уровень,
 *  - скорость(в наносекундах),
 *  - состояние паузы.
 *
 * Значения паузы:
 *  - -1 - конец игры,
 *  - 0 - игра идет,
 *  - 1 - пауза в игре,
 *  - 2 - начало игры,
 *  - 3 - gameover.
 *
 * @param figure_info Принимает указатель на структуру фигуры для заполнения
 * массива будущей фигуры.
 *
 * @return GameInfo_t Возвращает указатель на статичную структуру.
 */
GameInfo_t *create_field_info(Figureinfo_t *figure_info) {
  static GameInfo_t field_info = {
      nullptr,        // field
      nullptr,        // next
      0,              // score
      0,              // high_score
      1,              // level
      50000000,       // speed
      2               // pause
  };

  static int field[Y_FIELD + 2][X_FIELD];
  static int *field_pointer[Y_FIELD + 2];

  for (int i = 0; i < Y_FIELD + 2; i++) field_pointer[i] = field[i];

  field_info.field = field_pointer;

  for (int y = 0; y < Y_FIELD + 2; y++)
    for (int x = 0; x < X_FIELD; x++) field_info.field[y][x] = 0;

  static int next[MAX_FIGURE_SIZE][MAX_FIGURE_SIZE];

  static int *next_pointer[MAX_FIGURE_SIZE];
  for (int i = 0; i < MAX_FIGURE_SIZE; i++) next_pointer[i] = next[i];

  field_info.next = next_pointer;

  for (int y = 0; y < MAX_FIGURE_SIZE; y++)
    for (int x = 0; x < MAX_FIGURE_SIZE; x++) field_info.next[y][x] = 0;

  for (int i = 0; i < BLOCKS; i++) {
    int y = 1 + get_block_coord(figure_info->next_figure_type, 0, i, 0);
    int x = 2 + get_block_coord(figure_info->next_figure_type, 0, i, 1);
    field_info.next[y][x] = figure_info->next_figure_color;
  }

  field_info.score = 0;
  field_info.high_score = 0;
  hi_score(&field_info);
  field_info.level = 1;
  //скорость в наносекундах
  field_info.speed = ((11 - field_info.level) * 50000000);
  field_info.pause = 2;

  return &field_info;
}

/**
 * @brief Функция удаления или заполнения матрицы по положению фигуры.
 *
 * Функция работает с текущим положением фигуры. В зависимости от 2го
 * принимаемого параметра зануляет или заполняет матрицу по координатами фигуры
 * относительно блока-курсора.
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 * @param param Режим работы функции: 0 - удаляет фигуру, 1 - заполняет матрицу
 * по текущему положению фигуры.
 */
void clear_or_draw_position(Params_t *prms, bool param) {
  for (int i = 0; i < BLOCKS; i++) {
    int y = prms->figureinfo->y +
            get_block_coord(prms->figureinfo->figure_type,
                            prms->figureinfo->figure_state, i, 0);
    int x = prms->figureinfo->x +
            get_block_coord(prms->figureinfo->figure_type,
                            prms->figureinfo->figure_state, i, 1);
    prms->gameinfo->field[y][x] = (param) ? prms->figureinfo->figure_color : 0;
  }
}

/**
 * @brief Функция проверки истечения времени для движения вниз.
 *
 * Функция проверяет разницу между текущем временем и временем начала движения
 * вниз. Если разница больше текущего значения скорости, то стадия игры
 * переходит в шифтинг (принудительный сдвинг вниз на 1 строку).
 *
 * @param prms Указатель на структуру со всеми данными по игре.
 */
void chech_timer(Params_t *prms) {
  if (prms->state == MOVING) {
    struct timespec current_time;
    timespec_get(&current_time, TIME_UTC);

    long long diff =
        ((current_time.tv_sec) * 1000000000 + current_time.tv_nsec) -
        ((prms->start_time.tv_sec) * 1000000000 + prms->start_time.tv_nsec);

    if (diff >= prms->gameinfo->speed) prms->state = SHIFTING;
  }
}

/**
 * @brief Функция работы с рекордом очков.
 *
 * Функция получения или записи рекорда из файла hi-score.txt.
 *
 * Если текущий счет больше рекорда из файла, то перезаписывает файл с новым
 * значением.
 *
 * @param gameinfo Указатель на структуру с данными статистики по игре.
 */
void hi_score(GameInfo_t *gameinfo) {
  FILE *file;

  if (gameinfo->high_score >= gameinfo->score) {
    file = fopen("hi-score.txt", "r");
    if (file) {
      fscanf(file, "%d", &gameinfo->high_score);
      fclose(file);
    } else
      gameinfo->high_score = 0;

  } else {
    file = fopen("hi-score.txt", "w");
    fprintf(file, "%d", gameinfo->score);
    fclose(file);
  }
}

/**
 * @brief Функция обновления состояния игры.
 *
 * Функция получает актуальные данные о состоянии игры.
 *
 * @return GameInfo_t Возвращает структуру с актуальными данными игры.
 */
GameInfo_t updateCurrentState() {
  GameInfo_t *gameinfo = get_field_info(NULL);

  return *gameinfo;
}