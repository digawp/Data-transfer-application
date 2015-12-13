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

void session(tcp::socket sock)
{
  try
  {
    boost::filesystem::path path = "../ServerFiles/";
    boost::filesystem::recursive_directory_iterator itr(path);
    std::vector<std::string> paths;
    while (itr != boost::filesystem::recursive_directory_iterator())
    {
      if (!boost::filesystem::is_directory(itr->path().string()))
      {
        paths.push_back(itr->path().string());
        std::cout << itr->path().string() << std::endl;
      }
      ++itr;
    }

    std::streampos size;
    char* memblock;
    std::vector<std::string>::iterator itr2;

    for (itr2 = paths.begin(); itr2 != paths.end(); itr2++)
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
        char graaa[1024];
        std::memset(graaa, 0, sizeof graaa);
        std::strcpy(graaa, ss.str().substr(14, std::string::npos).c_str());
        size_t request_length = size;
        boost::asio::write(sock, boost::asio::buffer(graaa, 1024));
        boost::asio::write(sock, boost::asio::buffer(memblock, request_length));

        delete[] memblock;

        std::cout << "200 OK" << std::endl;
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
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    tcp::socket sock(io_service);
    a.accept(sock);
    std::thread(session, std::move(sock)).detach();
  }
}

int main()
{
  try
  {
    boost::asio::io_service io_service;
    server(io_service, 8080);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}