#include <iostream>
#include <string>
using namespace std;

int main()
{
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true)
  {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit 0")
    {
      exit(0); // Exit the program if "exit 0" is entered
    }
    else if (input.find("echo ") == 0)
    {
      const int ECHO_LEN = 5; // Including space
      std::string text = input.substr(ECHO_LEN);
      std::cout << text << std::endl;
    }
    else
    {
      std::cout << input << ": command not found" << std::endl;
    }
  }

  return 0;
}
