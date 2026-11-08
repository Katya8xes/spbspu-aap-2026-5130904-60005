#include <cctype>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <new>

namespace khrapova
{

  const int success = 0;
  const int input_error_code = 1;
  const int memory_error_code = 1;
  const size_t alphabet_size = 26;
  const size_t initial_memory = 16;

  char *appendDigits(char *dest, const char *src)
  {
    size_t len = std::strlen(dest);
    for (size_t i = 0; src[i] != '\0'; ++i)
    {
      if (std::isdigit(static_cast< unsigned char >(src[i])))
      {
        dest[len++] = src[i];
      }
    }
    dest[len] = '\0';
    return dest;
  }

  void markLetters(bool *seen, const char *s)
  {
    for (size_t i = 0; s[i] != '\0'; ++i)
    {
      const unsigned char c = static_cast< unsigned char >(s[i]);
      if (std::isalpha(c))
      {
        const int idx = std::tolower(c) - 'a';
        if (idx >= 0 && static_cast< size_t >(idx) < alphabet_size)
        {
          seen[idx] = true;
        }
      }
    }
  }

  char *makeLatinUnion(char *dest, const char *src1, const char *src2)
  {
    bool seen[alphabet_size] = {};
    markLetters(seen, src1);
    markLetters(seen, src2);
    size_t n = 0;
    for (size_t i = 0; i < alphabet_size; ++i)
    {
      if (seen[i])
      {
        dest[n++] = static_cast< char >('a' + i);
      }
    }
    dest[n] = '\0';
    return dest;
  }

  char *readString(std::istream &in)
  {
    size_t memory = initial_memory;
    size_t length = 0;
    char *buffer = nullptr;

    try
    {
      buffer = new char[memory];
    }
    catch (const std::bad_alloc &)
    {
      return nullptr;
    }

    char c = 0;
    while (in.get(c) && c != '\n')
    {
      if (length + 1 >= memory)
      {
        const size_t new_memory = memory * 2;
        char *new_buffer = nullptr;
        try
        {
          new_buffer = new char[new_memory];
        }
        catch (const std::bad_alloc &)
        {
          delete[] buffer;
          return nullptr;
        }
        std::memcpy(new_buffer, buffer, length);
        delete[] buffer;
        buffer = new_buffer;
        memory = new_memory;
      }
      buffer[length++] = c;
    }

    buffer[length] = '\0';
    return buffer;
  }

  char *allocate(size_t n)
  {
    try
    {
      return new char[n];
    }
    catch (const std::bad_alloc &)
    {
      return nullptr;
    }
  }

  int process(std::istream &in, std::ostream &out, std::ostream &err)
  {
    const char *const second16 = "g1h2k";
    const char *const second20 = "def_ghk";

    char *const main_string = readString(in);
    if (main_string == nullptr)
    {
      err << "не удалось выделить память\n";
      return memory_error_code;
    }
    if (in.bad() || main_string[0] == '\0')
    {
      err << "не удалось считать строку\n";
      delete[] main_string;
      return input_error_code;
    }

    char *const result16 = allocate(std::strlen(main_string) + std::strlen(second16) + 1);
    char *const result20 = allocate(alphabet_size + 1);
    if (result16 == nullptr || result20 == nullptr)
    {
      err << "не удалось выделить память\n";
      delete[] result16;
      delete[] result20;
      delete[] main_string;
      return memory_error_code;
    }

    std::strcpy(result16, main_string);
    appendDigits(result16, second16);
    makeLatinUnion(result20, main_string, second20);

    out << result20 << '\n';
    out << result16 << '\n';

    delete[] result16;
    delete[] result20;
    delete[] main_string;
    return success;
  }

}

int main()
{
  return khrapova::process(std::cin, std::cout, std::cerr);
}
