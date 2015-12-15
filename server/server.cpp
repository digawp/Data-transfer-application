#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

using boost::asio::ip::tcp;

int main()
{
  try
  {
    const boost::filesystem::path path = "../ServerFiles/";
    boost::filesystem::recursive_directory_iterator itr(path);
    std::vector<std::string> paths;
    while (itr != boost::filesystem::recursive_directory_iterator())
    {
      if (!boost::filesystem::is_directory(itr->path()))
      {
        paths.push_back(itr->path().string());
      }
      ++itr;
    }

    boost::asio::io_service io_service;
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), 8080));
    tcp::socket socket(io_service);
    a.accept(socket);

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
        char extra[512];
        std::memset(extra, 0, 512);
        std::strcpy(extra, ss.str().substr(15, std::string::npos).c_str());
        boost::asio::write(socket, boost::asio::buffer(extra, 512));
        boost::asio::write(socket, boost::asio::buffer(memblock, size));
        delete[] memblock;
      }
      catch (std::fstream::failure e)
      {
        std::cerr << "Exception opening/reading/closing file" << std::endl;
        return 1;
      }
      catch (std::exception& e)
      {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  return 0;
}