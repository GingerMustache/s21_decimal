#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int output = 0;
  int count_1 = 0, count_2 = 0;
  int val_1 = 0, val_2 = 0;
  int index = 0;
  unsigned long long res_of_summ = 0;

  if (result) {
    s21_decimal step = {0};
    s21_set_dec_number_to_0(result);

    while (index != 32) {  // проверка что на что умножать будет
      val_1 = s21_get_bit(&value_1, index);
      val_2 = s21_get_bit(&value_2, index);
      count_1 += val_1;
      count_2 += val_2;
      index++;
    }
    index = 0;
    // case 1
    if (count_1 < count_2) {
      while (index != 96) {
        if (s21_get_bit(&value_1, index)) {
          step = value_2;
          s21_shift_bits(&step, index);
          res_of_summ = s21_add(step, *result, result);
          s21_set_dec_number_to_0(&step);
        }
        index++;
      }
    } else {
      while (index != 96) {  // проверить переполнение
        if (s21_get_bit(&value_2, index)) {
          step = value_1;
          s21_shift_bits(&step, index);
          res_of_summ = s21_add(step, *result, result);
          s21_set_dec_number_to_0(&step);
        }
        index++;
      }
    }
    (void)res_of_summ;
  }
  return output;
}
