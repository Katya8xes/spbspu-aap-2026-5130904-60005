#include <iostream>
#include <fstream>
#include <limits>
#include <cstddef>
#include <new>

namespace khrapova {

  const int error_input = 1;
  const int error_data = 2;
  const int success = 0;
  const std::size_t fixed_size = 10000;

  long long max_sum_pob(const int* const matrix, std::size_t cols, std::size_t size, bool& overflow)
  {
    overflow = false;
    if (size <= 1) {
      return 0;
    }

    long long max_sum = std::numeric_limits< long long >::min();
    bool found = false;
    std::size_t max_sum_idx = (size - 1) + (size - 1);

    for (std::size_t s = 0; s <= max_sum_idx; ++s) {
      if (s != size - 1) {
        long long current_sum = 0;
        bool has_elements = false;
        for (std::size_t r = 0; r < size; ++r) {
          if (s >= r) {
            std::size_t c = s - r;
            if (c < size) {
              int val = matrix[r * cols + c];
              if (val > 0 && current_sum > std::numeric_limits< long long >::max() - val) {
                overflow = true;
                return 0;
              }
              if (val < 0 && current_sum < std::numeric_limits< long long >::min() - val) {
                overflow = true;
                return 0;
              }
              current_sum += val;
              has_elements = true;
            }
          }
        }
        if (has_elements) {
          if (!found || current_sum > max_sum) {
            max_sum = current_sum;
            found = true;
          }
        }
      }
    }
    if (found) {
      return max_sum;
    }
    return 0;
  }

  long long max_sum_glav(const int* const matrix, std::size_t cols, std::size_t size, bool& overflow)
  {
    overflow = false;
    if (size <= 1) {
      return 0;
    }

    long long max_sum = std::numeric_limits< long long >::min();
    bool found = false;
    long long min_diff = -static_cast< long long >(size) + 1;
    long long max_diff = static_cast< long long >(size) - 1;

    for (long long diff = min_diff; diff <= max_diff; ++diff) {
      if (diff != 0) {
        long long current_sum = 0;
        bool has_elements = false;
        for (std::size_t r = 0; r < size; ++r) {
          long long c_ll = static_cast< long long >(r) - diff;
          if (c_ll >= 0 && c_ll < static_cast< long long >(size)) {
            int val = matrix[r * cols + c_ll];
            if (val > 0 && current_sum > std::numeric_limits< long long >::max() - val) {
              overflow = true;
              return 0;
            }
            if (val < 0 && current_sum < std::numeric_limits< long long >::min() - val) {
              overflow = true;
              return 0;
            }
            current_sum += val;
            has_elements = true;
          }
        }
        if (has_elements) {
          if (!found || current_sum > max_sum) {
            max_sum = current_sum;
            found = true;
          }
        }
      }
    }
    if (found) {
      return max_sum;
    }
    return 0;
  }

  bool write_results(const char* output_path, long long res13, bool ok13, long long res14, bool ok14)
  {
    std::ofstream output(output_path);
    if (!output) {
      std::cerr << "не удалось открыть файл для записи\n";
      return false;
    }
    if (ok13) {
      output << res13 << "\n";
    }
    if (ok14) {
      output << res14 << "\n";
    }
    return output.good();
  }
}

int main(int argc, char** argv)
{
  using namespace khrapova;

  if (argc != 4) {
    std::cerr << "неверное количество аргументов\n";
    return error_input;
  }

  if (argv[1][0] == '\0' || argv[1][1] != '\0' || argv[1][0] < 1 || argv[1][0] > '2') {
    std::cerr << "первый параметр должен быть 1 или 2\n";
    return error_input;
  }

  bool is_dynamic = false;
  if (argv[1][0] == '2') {
    is_dynamic = true;
  }

  const char* input_path = argv[2];
  const char* output_path = argv[3];

  std::ifstream input(input_path);
  if (!input) {
    std::cerr << "не удалось открыть входной файл\n";
    return error_input;
  }

  long long r_ll = 0, c_ll = 0;
  if (!(input >> r_ll >> c_ll) || r_ll < 0 || c_ll < 0) {
    std::cerr << "неверные размеры матрицы\n";
    return error_data;
  }

  std::size_t rows = static_cast< std::size_t >(r_ll);
  std::size_t cols = static_cast< std::size_t >(c_ll);

  if (rows == 0 && cols == 0) {
    if (write_results(output_path, 0, true, 0, true)) {
      return success;
    }
    return error_data;
  }

  if (rows == 0 || cols == 0) {
    std::cerr << "неверные размеры матрицы (одна из размерностей равна 0)\n";
    return error_data;
  }

  if (rows > std::numeric_limits< std::size_t >::max() / cols || rows * cols > fixed_size) {
    std::cerr << "превышен лимит элементов\n";
    return error_data;
  }

  std::size_t size = 0;
  if (rows < cols) {
    size = rows;
  } else {
    size = cols;
  }

  long long res13 = 0;
  long long res14 = 0;
  bool ok13 = true;
  bool ok14 = true;

  if (!is_dynamic) {
    int data_block[fixed_size];

    bool read_error = false;
    for (std::size_t i = 0; i < rows; ++i) {
      for (std::size_t j = 0; j < cols; ++j) {
        long long val;
        if (!(input >> val)) {
          read_error = true;
          break;
        }
        if (val > std::numeric_limits< int >::max() || val < std::numeric_limits< int >::min()) {
          read_error = true;
          break;
        }
        data_block[i * cols + j] = static_cast< int >(val);
      }
      if (read_error) {
        break;
      }
    }

    if (read_error) {
      std::cerr << "недостаточно данных или неверный формат\n";
      return error_data;
    }

    char extra_char;
    if (input >> extra_char) {
      std::cerr << "в файле содержатся лишние данные\n";
      return error_data;
    }

    bool overflow13 = false;
    bool overflow14 = false;

    res13 = max_sum_pob(data_block, cols, size, overflow13);
    if (overflow13) {
      ok13 = false;
    }

    res14 = max_sum_glav(data_block, cols, size, overflow14);
    if (overflow14) {
      ok14 = false;
    }
  } else {
    int* data_block = nullptr;

    try {
      data_block = new int[rows * cols];
    } catch (const std::bad_alloc&) {
      std::cerr << "не удалось выделить память\n";
      return error_data;
    }

    bool read_error = false;
    for (std::size_t i = 0; i < rows; ++i) {
      for (std::size_t j = 0; j < cols; ++j) {
        long long val;
        if (!(input >> val)) {
          read_error = true;
          break;
        }
        if (val > std::numeric_limits< int >::max() || val < std::numeric_limits< int >::min()) {
          read_error = true;
          break;
        }
        data_block[i * cols + j] = static_cast< int >(val);
      }
      if (read_error) {
        break;
      }
    }

    if (read_error) {
      std::cerr << "недостаточно данных или неверный формат\n";
      delete[] data_block;
      return error_data;
    }

    char extra_char;
    if (input >> extra_char) {
      std::cerr << "в файле содержатся лишние данные\n";
      delete[] data_block;
      return error_data;
    }

    bool overflow13 = false;
    bool overflow14 = false;

    res13 = max_sum_pob(data_block, cols, size, overflow13);
    if (overflow13) {
      ok13 = false;
    }

    res14 = max_sum_glav(data_block, cols, size, overflow14);
    if (overflow14) {
      ok14 = false;
    }

    delete[] data_block;
  }

  if (!write_results(output_path, res13, ok13, res14, ok14)) {
    return error_data;
  }

  if (!ok13 || !ok14) {
    if (!ok13) {
      std::cerr << "переполнение при суммировании (задача 13)\n";
    }
    if (!ok14) {
      std::cerr << "переполнение при суммировании (задача 14)\n";
    }
    return error_data;
  }

  return success;
}
