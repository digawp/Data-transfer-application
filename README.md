# Data-transfer-application

### About the project

Please read the "Projet-statement" file

### Requirements

- C++ Boost library http://www.boost.org
- G++ compiler (or another c++11 compliant compiler)

### Building

```sh
- Build bin :
$ make build
- Debug bin :
$ make debug
- Clean all :
$ make clean
- Rebuild :
$ make rebuild
```

Note : the makefile use G++, if you want to use another compiler, please make this change :
```sh
CXX = Your_Compiler
```

### Usage

Just launch the server bin first, and then run the client bin.

### Specified fields

Due to the nature of this project, some informations are hard-coded to be faster and tiny as possible.
You can change the ip address, the port number, and the paths directly in the code.

If you want to run it under windows, please paid attention to also change the char "/" to "\\" at line 62-63 of client.cpp

### Notes

If you want to know more about the project research and history, chek the "Project_report" file.

### License

MIT (Please read the "LICENSE" file)