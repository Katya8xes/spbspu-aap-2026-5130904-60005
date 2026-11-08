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
      unsigned char c = static_cast< unsigned char >(s[i]);
      if (std::isalpha(c))
      {
        int idx = std::tolower(c) - 'a';
        if (idx >= 0 && idx < 26)
        {
          seen[idx] = true;
        }
      }
    }
  }

  char *makeLatinUnion(char *dest, const char *src1, const char *src2)
  {
    bool seen[26] = {};
    markLetters(seen, src1);
    markLetters(seen, src2);
    size_t n = 0;
    for (int i = 0; i < 26; ++i)
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
    size_t memory = 16;
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
        size_t newMemory = memory * 2;
        char *newBuffer = nullptr;
        try
        {
          newBuffer = new char[newMemory];
        }
        catch (const std::bad_alloc &)
        {
          delete[] buffer;
          return nullptr;
        }
        std::memcpy(newBuffer, buffer, length);
        delete[] buffer;
        buffer = newBuffer;
        memory = newMemory;
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
    const char *second16 = "g1h2k";
    const char *second20 = "def_ghk";

    char *mainString = readString(in);
    if (mainString == nullptr)
    {
      err << "не удалось выделить память\n";
      return memory_error_code;
    }
    if (in.bad())
    {
      err << "не удалось считать строку\n";
      delete[] mainString;
      return input_error_code;
    }

    char *result16 = allocate(std::strlen(mainString) + std::strlen(second16) + 1);
    char *result20 = allocate(27);
    if (result16 == nullptr || result20 == nullptr)
    {
      err << "не удалось выделить память\n";
      delete[] result16;
      delete[] result20;
      delete[] mainString;
      return memory_error_code;
    }

    std::strcpy(result16, mainString);
    appendDigits(result16, second16);
    makeLatinUnion(result20, mainString, second20);

    out << result16 << '\n';
    out << result20 << '\n';

    delete[] result16;
    delete[] result20;
    delete[] mainString;
    return success;
  }

}

int main()
{
  return khrapova::process(std::cin, std::cout, std::cerr);
}
