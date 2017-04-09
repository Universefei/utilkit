#include <iostream>
#include "file_readline.hpp"

using std::string;
using std::cout;
using std::endl;

int main (int argc, char const* argv[])
{
  if (argc < 2) {
    std::cout << "parameters failed!" << std::endl;
    exit(-1);
  }
  string t_file_path = argv[1];
  CsvParser t_parser;
  t_parser.load_file(t_file_path);
  boost::unordered_map<string, string> t_kvp;
  /* while (!t_parser.fetch_row(&t_kvp)) { */
  /*   boost::unordered_map<string, string>::iterator t_itr = t_kvp.begin(); */
  /*   for (; t_itr != t_kvp.end(); ++t_itr) { */
  /*     std::cout << t_itr->first << ":" << t_itr->second << "\t"; */
  /*   } */
  /*   std::cout << std::endl; */
  /* } */
  int t_ecode = 0;
  do {
    t_kvp.clear();
    t_ecode = t_parser.fetch_row(&t_kvp);
    /* std::cout << "ret:" << t_ecode << std::endl; */
    boost::unordered_map<string, string>::iterator t_itr = t_kvp.begin();
    for (; t_itr != t_kvp.end(); ++t_itr) {
      std::cout << t_itr->first << ":" << t_itr->second << "\t";
    }
    std::cout << std::endl;
  } while (!t_ecode);
  return 0;
}
