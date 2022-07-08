//
//  main.cpp
//  termbookmark
//
//  Created by M. Alp Akbaba on 8.07.2022.
//

#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>


static void show_usage(bool err)
{
    if(err)
        std::cerr << "Invalid Operation! | try (add) or (list)" << std::endl;
    else
        std::cerr << "(add) -> Bookmark Add" << std::endl << "(list) -> List Bookmarks" << std::endl;
}

const std::vector<std::string> explode(const std::string& s, const char& c)
{
    std::string buff{""};
    std::vector<std::string> v;
    
    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);
    
    return v;
}

bool checkIsset(std::string name) {
    std::ifstream BookmarksFile("bookmarks.l");
    std::string line;
    
    while (getline(BookmarksFile, line)) {
      std::vector<std::string> lineV = explode(line, '@');
        
        if(lineV[0] == name)
            return true;
    }
    
    BookmarksFile.close();
    
    return false;
}

void list() {
    std::ifstream BookmarksFile("bookmarks.l");
    std::string line;
    
    while (getline(BookmarksFile, line)) {
      std::vector<std::string> lineV = explode(line, '@');
        std::cout << "\t" << lineV[0] << " -> " << lineV[1] << std::endl;
    }
    
    BookmarksFile.close();
}

int copytoclipboard(const char *str) {

    const char proto_cmd[] = "echo '%s' | /usr/bin/pbcopy";

    char cmd[strlen(str) + strlen(proto_cmd) - 1]; // -2 to remove the length of %s in proto cmd and + 1 for null terminator = -1
    sprintf(cmd ,proto_cmd, str);

    return system(cmd);
}

void getBookmark(std::string name) {
    std::ifstream BookmarksFile("bookmarks.l");
    std::string line;
    
    while (getline(BookmarksFile, line)) {
      std::vector<std::string> lineV = explode(line, '@');
        
        if(lineV[0] == name) {
            std::cout << "\t" << lineV[0] << " -> " << lineV[1] << std::endl;
                
        
            char tab2[1024];
            strcpy(tab2, lineV[1].c_str());
            copytoclipboard(tab2);
            
            return;
        }
    }
    
    BookmarksFile.close();
}

int main(int argc, const char * argv[]) {
    
    if (argc < 2) {
          show_usage(argv[0]);
          return 1;
      }
      std::vector <std::string> sources;
      std::string destination;
      for (int i = 1; i < argc; ++i) {
          std::string arg = argv[i];
          if ((arg == "h") || (arg == "help")) {
              show_usage(false);
              return 0;
          } else if ((arg == "l") || (arg == "list")) {
              list();
              
          } else if ((arg == "g") || (arg == "get")) {
              getBookmark(argv[++i]);
          } else if ((arg == "a") || (arg == "add")) {
              std::string bName;
              
              if (i + 1 < argc) {
                  bName = argv[++i];
              } else {
                  std::cerr << "add option requires one argument." << std::endl;
                  return 1;
              }
              
              if(checkIsset(bName)) {
                  std::cerr << "Name is already used." << std::endl;
                  return 1;
              }
              
              char tmp[256];
              getcwd(tmp, 256);
              
              std::ofstream MyFile("bookmarks.l", std::ios_base::app);
              MyFile << bName <<'@'<< tmp << std::endl;
              
              MyFile.close();
              
              std::cout << "Bookmark Added." << std::endl;
              
          } else {
              sources.push_back(argv[i]);
          }
      }
    
    return 0;
}
