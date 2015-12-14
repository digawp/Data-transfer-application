#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

using boost::asio::ip::tcp;

int main()
{
  boost::asio::io_service io_service;
  tcp::socket socket(io_service);
  tcp::resolver resolver(io_service);
  try
  {
    boost::asio::connect(socket, resolver.resolve({"127.0.0.1", "8080"})); // TODO : change by server ip
    boost::asio::write(socket, boost::asio::buffer("a", 1));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

  std::string path;
  std::string size;
  size_t tsize;
  char* extra;
  char* data;
  bool switcher = true;
  boost::system::error_code error;

  for (;;)
  {
      if (switcher) {
        extra = new char[1024];
        std::memset(extra, 0, 1024);

        boost::asio::read(socket, boost::asio::buffer(extra, 1024), error);
        if (error == boost::asio::error::eof)
          break;
        else if (error)
          throw boost::system::system_error(error);

        std::stringstream ss;
        ss << extra;
        std::getline(ss, path);
        std::getline(ss, size);
        tsize = std::stoi(size, nullptr, 10);
        size.clear();
        delete[] extra;
        switcher = false;
      }
      else {
        data = new char[tsize];

        boost::asio::read(socket, boost::asio::buffer(data, tsize), error);
        if (error == boost::asio::error::eof)
          break;
        else if (error)
          throw boost::system::system_error(error);

        if (path.find_last_of("/") != std::string::npos) {
          boost::filesystem::path dir(strcat(getenv("HOME"), "/Desktop/ClientFiles") + path.substr(0, path.find_last_of("/")));
          boost::filesystem::create_directories(dir);
        }

        std::ofstream file2 ("../ClientFiles/" + path, std::ios::out | std::ios::binary | std::ios::ate);
        path.clear();
        file2.seekp (0, std::ios::beg);
        file2.write (data, tsize);
        file2.close();
        delete[] data;
        switcher = true;
      }
  }
  return 0;
}