#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <filesystem> // Requires C++17 or later
#ifdef _WIN32
#define PATH_SEPARATOR ';'
#else
#define PATH_SEPARATOR ':'
#endif

using namespace std;

std::string get_path(std::string command)
{
  char *path_env_ptr = std::getenv("PATH");
  if (!path_env_ptr)
  {
    return ""; // Return empty if PATH is not set
  }

  std::string path_env = path_env_ptr;
  std::stringstream ss(path_env);
  std::string path;
  while (std::getline(ss, path, PATH_SEPARATOR))
  {
    std::string abs_path = path + '/' + command;
    if (std::filesystem::exists(abs_path))
    {
      return abs_path;
    }
  }
  return "";
}

int main()
{
  const int ECHO_LEN = 5;
  const int TYPE_LEN = 5;

  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true)
  {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit 0")
    {
      exit(0);
    }
    else if (input.find("echo ") == 0)
    {
      std::string text = input.substr(ECHO_LEN);
      std::cout << text << std::endl;
    }
    else if (input.find("type ") == 0)
    {
      std::string text = input.substr(TYPE_LEN);
      if (text == "echo")
      {
        std::cout << "echo is a shell builtin\n";
      }
      else if (text == "exit")
      {
        std::cout << "exit is a shell builtin\n";
      }
      else if (text == "type")
      {
        std::cout << "type is a shell builtin\n";
      }
      else
      {
        std::string command_path = get_path(text);
        if (!command_path.empty())
        {
          std::cout << text << " is " << command_path << std::endl;
        }
        else
        {
          std::cout << text << ": not found\n";
        }
      }
    }
    else
    {
      std::string command_path = get_path(input);
      if (!command_path.empty())
      {
        std::cout << input << " is located at " << command_path << std::endl;
      }
      else
      {
        std::cout << input << ": command not found" << std::endl;
      }
    }
  }

  return 0;
}
