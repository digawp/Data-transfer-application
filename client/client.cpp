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

const std::string server_ip = "127.0.0.1"; // "192.168.10.2" in actual run
const std::string parent_path = strcat(getenv("HOME"), "/Desktop/ClientFiles");

int main()
{
  boost::asio::io_service io_service;
  tcp::socket socket(io_service);
  tcp::resolver resolver(io_service);
  try
  {
    boost::asio::connect(socket, resolver.resolve({server_ip, "8080"}));
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
<<<<<<< HEAD
  boost::system::error_code error;
=======
>>>>>>> 4ed34401b47008ba7aa354003be40fbf74c1e27c

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
<<<<<<< HEAD
          boost::filesystem::path dir("../ClientFiles/" + path.substr(0, path.find_last_of("/")));
          boost::filesystem::create_directories(dir);
        }

        std::ofstream file2 ("../ClientFiles/" + path, std::ios::out | std::ios::binary | std::ios::ate);
=======
          boost::filesystem::path sent_path(path);
          boost::filesystem::path dir(parent_path + sent_path.parent_path().string());
          boost::filesystem::create_directories(dir);
        }

        std::ofstream file2 (parent_path + path, std::ios::out | std::ios::binary | std::ios::ate);
>>>>>>> 4ed34401b47008ba7aa354003be40fbf74c1e27c
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