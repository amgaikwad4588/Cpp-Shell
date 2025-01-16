#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <filesystem>
using namespace std;
#ifdef _WIN32
const char PATH_SEPARATOR = ';';
const std::string EXTENSION = ".exe";
#else
const char PATH_SEPARATOR = ':';
const std::string EXTENSION = "";
#endif

static std::string WORKING_DIR = "/app";
const char *env_var_name = "PATH";
const char *env_var_value = getenv(env_var_name);

std::vector<std::string> path_file_list;
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
std::vector<std::string> splitPath(const std::string &path)
{
  std::vector<std::string> paths;
  std::stringstream ss(path);
  std::string item;
  while (std::getline(ss, item, PATH_SEPARATOR))
  {
    if (!item.empty())
    {
      paths.push_back(item);
    }
  }
  return paths;
}

bool commandExists(const std::string &str)
{
  if (env_var_value == nullptr)
  {
    return false;
  }
  std::string commandWithExt = str + EXTENSION;
  for (const auto &dir : path_file_list)
  {
    std::filesystem::path filePath = std::filesystem::path(dir) / commandWithExt;
    if (std::filesystem::exists(filePath))
    {
      return true;
    }
  }
  return false;
}

void executeCommandFunction(const std::string &str)
{
  system(str.c_str()); // Executes the command with arguments
}

std::vector<std::string> splitString(const std::string &str)
{
  std::vector<std::string> result;
  std::istringstream iss(str);
  std::string word;
  while (iss >> word)
  {
    result.push_back(word);
  }
  return result;
}

int main()
{
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  if (env_var_value != nullptr)
  {
    path_file_list = splitPath(env_var_value);
  }

  std::string input;
  while (true)
  {
    std::cout << "$ ";
    std::getline(std::cin, input);
    std::vector<std::string> stackCommands = splitString(input);

    if (stackCommands[0] == "exit")
    {
      return 0;
    }
    else if (stackCommands[0] == "echo")
    {
      std::cout << input.substr(5) << std::endl;
    }
    else if (stackCommands[0] == "pwd")
    {
      std::filesystem::path currentPath = std::filesystem::current_path();
      std::cout << currentPath.string() << std::endl;
    }
    else if (stackCommands[0] == "cd")
    {
      if (stackCommands.size() < 2)
      {
        std::cerr << "cd: missing argument" << std::endl;
        continue;
      }

      std::string targetPath = stackCommands[1];

      try
      {
        std::filesystem::path newPath = targetPath;

        if (std::filesystem::exists(newPath) && std::filesystem::is_directory(newPath))
        {
          std::filesystem::current_path(newPath);
        }
        else
        {
          std::cerr << "cd: " << targetPath << ": No such file or directory" << std::endl;
        }
      }
      catch (const std::filesystem::filesystem_error &e)
      {
        std::cerr << "cd: " << targetPath << ": " << e.what() << std::endl;
      }
    }
    else if (stackCommands[0] == "type")
    {

      std::string text = stackCommands[1];
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
      else if (text == "pwd")
      {
        std::cout << "pwd is a shell builtin\n";
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
          if (commandExists(stackCommands[0]))
          {
            executeCommandFunction(input); // Run the command with arguments
          }
          else
          {
            std::cout << stackCommands[1] << ": not found" << std::endl;
          }
        }
      }
    }
    else
    {
      if (commandExists(stackCommands[0]))
      {
        executeCommandFunction(input); // Run the command with arguments
      }
      else
      {
        std::cout << stackCommands[0] << ": not found" << std::endl;
      }
    }
  }
}