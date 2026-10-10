#include <iostream>
#include <limits>
#include <cstddef>

int main()
{
  long long x = 0;
  size_t nowdlina = 0;
  size_t maxdlina = 0;
  long long prev1 = 0;
  long long prev2 = 0;
  size_t sumprev = 0;
  size_t kol = 0;

  const int error1 = 1;
  const int error2 = 2;
  const int error0 = 0;
  const size_t maxlen = std::numeric_limits< size_t >::max();

  while (std::cin >> x) {
    if (x == 0) {
      break;
    }
    kol += 1;

    if (kol == maxlen) {
      std::cerr << "длинная последовательность\n";
      return error2;
    }

    if (x % 2 == 0) {
      nowdlina += 1;
      if (nowdlina > maxdlina) {
        maxdlina = nowdlina;
      }
    } else {
      nowdlina = 0;
    }

    if (kol > 2) {
      bool overflow = false;
      long long sum = 0;
      if (prev2 > 0) {
        if (prev1 > std::numeric_limits< long long >::max() - prev2) {
          overflow = true;
        } else {
          sum = prev1 + prev2;
        }
      } else if (prev2 < 0) {
        if (prev1 < std::numeric_limits< long long >::min() - prev2) {
          overflow = true;
        } else {
          sum = prev1 + prev2;
        }
      } else {
        sum = prev1;
      }
      if (!overflow && x == sum) {
        sumprev += 1;
      }
    }
    prev2 = prev1;
    prev1 = x;
  }

  if (std::cin.fail()) {
    std::cerr << "некорректные данные\n";
    return error1;
  }

  std::cout << maxdlina << "\n";

  bool flag = true;

  if (kol < 3) {
    std::cerr << "короткая последовательность\n";
    flag = false;
  } else {
    std::cout << sumprev << "\n";
  }

  if (flag) {
    return error0;
  } else {
    return error2;
  }
}
