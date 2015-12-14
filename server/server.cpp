#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

using boost::asio::ip::tcp;

void session(tcp::socket socket)
{
  try
  {
    const boost::filesystem::path path = strcat(getenv("HOME"), "/Desktop/ServerFiles");
    boost::filesystem::recursive_directory_iterator itr(path);
    std::vector<std::string> paths;
    while (itr != boost::filesystem::recursive_directory_iterator())
    {
      if (!boost::filesystem::is_directory(itr->path().string()))
      {
        paths.push_back(itr->path().string());
      }
      ++itr;
    }

    std::streampos size;
    char* memblock;
    std::vector<std::string>::iterator itr2;

    for (itr2 = paths.begin(); itr2 != paths.end(); ++itr2)
    {
      try {
        std::ifstream file (*itr2, std::ios::in | std::ios::binary | std::ios::ate);
        size = file.tellg();
        memblock = new char[size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();
        std::stringstream ss;
        ss << *itr2 << "\n" << size << "\n";
        char extra[1024];
        std::memset(extra, 0, 1024);
        std::strcpy(extra, ss.str().substr(14, std::string::npos).c_str());
        boost::asio::write(socket, boost::asio::buffer(extra, 1024));
        boost::asio::write(socket, boost::asio::buffer(memblock, size));
        delete[] memblock;
      }
      catch (std::fstream::failure e)
      {
        std::cerr << "Exception opening/reading/closing file" << std::endl;
        return;
      }
      catch (std::exception& e)
      {
        std::cerr << "Exception: " << e.what() << std::endl;
        return;
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << std::endl;
  }
}

int main()
{
  try
  {
    boost::asio::io_service io_service;
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), 8080));
    for (;;)
    {
      tcp::socket socket(io_service);
      a.accept(socket);
      std::thread(session, std::move(socket)).detach();
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  return 0;
}