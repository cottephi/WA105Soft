#ifndef _PROCESS_DIR_H_
#define _PROCESS_DIR_H_

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <vector>
#include <string>


class ProcessDir
{
 private:
    
 public:
  ProcessDir(){;};
  ~ProcessDir(){;};
  
  int GetRootFiles(const char* dirname, std::vector<std::string> &files);
  void Print(std::vector<std::string> files);
    
};

#endif
