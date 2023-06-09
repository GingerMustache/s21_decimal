#include "s21_decimal.h"
// Доп. функция, вынесена т.к. используется не только тут
void s21_set_bits_from_int_to_decimal(int src, s21_decimal *dst,
                                      int which_int_part_to_fill) {
  unsigned int exp = 0;
  src = fabs((double)src);
  while (src >= pow(2, exp)) exp++;
  while (src) {
    if (src - pow(2, exp) < 0) {
      s21_set_bit_0(dst, exp + which_int_part_to_fill);
    } else {
      src -= pow(2, exp);
      s21_set_bit_1(dst, exp + which_int_part_to_fill);
    }
    exp--;
  }
}

// Конвертер int->decimal
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int output = CONVERSATION_ERROR;
  if (dst) {
    output = CONVERSATION_OK;
    s21_set_dec_number_to_0(dst);
    if ((int)src < 0) {
      if (src != INT_MIN) {
        dst->bits[3] |= (1 << 31);
      } else {
        dst->bits[3] |= (0 << 31);
      }
    }
    s21_set_bits_from_int_to_decimal(src, dst, 0);
  }
  return output;
}

// Конвертер decimal->int
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int output = CONVERSATION_ERROR;

  if (dst) {
    int power_of_decimal = s21_get_power_of_decimal(src);

    if (power_of_decimal < 28) {
      *dst = 0;
      int sign = s21_get_bit(&src, 127);
      int index = 0;
      long long buffer = 0;  // 20 didgets

      while (power_of_decimal) {
        s21_div_decimal_by_10(&src);
        power_of_decimal--;
      }
      while (index != 96) {
        if (s21_get_bit(&src, index)) {
          buffer += pow(2, index);
        }
        index++;
      }
      if (sign) buffer *= -1;

      if (buffer > MAX_INT || buffer < MIN_INT) {
        output = CONVERSATION_ERROR;
      } else {
        output = CONVERSATION_OK;
      }

      if (output == CONVERSATION_OK) {
        *dst = (int)buffer;
      }
    }
  }

  return output;
}
