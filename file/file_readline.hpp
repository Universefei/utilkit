#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <boost/unordered_map.hpp>

using std::vector;
using std::map;
using std::string;


int parse_csv(const string& file_path, map<string, string>* row);
int string2array(const string& line, const string& sep, vector<string>* v) {
  if (!v) return 0;
  int t_ret = 0;
  string::size_type t_head = 0;
  string::size_type t_sep = 0;
  size_t t_len = line.size();
  do {
    t_sep = line.find(sep, t_head);
    if (string::npos == t_sep) {
      /* std::cout << "push_back->" << string(line, t_head, line.size() - t_head) */
        /* << std::endl; */
      v->push_back(string(line, t_head, line.size() - t_head));
      break;
    } else {
      /* std::cout << "push_back->" << string(line, t_head, t_sep - t_head) */
        /* << std::endl; */
      v->push_back(string(line, t_head, t_sep - t_head));
      if (t_len > t_sep) {
        t_head = t_sep + 1;
      } else {
        break;
      }
    }
  } while (t_sep != string::npos);
  return t_ret;
}

string int2string(const int& nu) {
  std::stringstream t_buf;
  t_buf << nu;
  return t_buf.str();
  
}

/******************************************************************************
 *                                  FileParser
 *****************************************************************************/

class FileParser {
  public:
    FileParser();
    FileParser(const string& file_path);
    virtual ~FileParser();
    int load_file(const string& file_path);
    int reset();
    virtual int fetch_row(map<string, string>* p_row);
    virtual int fetch_row(boost::unordered_map<string, string>* p_row);
    int define_fields(const vector<string>& fields);
    int get_fields_define(vector<string>* p_fields);

  protected:
    int get_line(string* p_line);
    virtual int format2kvp(const string& line, map<string, string>* p_kvp) = 0;
    virtual int format2kvp_hashmap(const string& line,
        boost::unordered_map<string, string>* p_kvp) = 0;
    std::ifstream if_;
    vector<string> keyname_;
};

/******************************************************************************
 *                                  CsvParser
 *****************************************************************************/

class CsvParser : public FileParser {
  public:
    CsvParser();
    virtual ~CsvParser();
  private:
    virtual int format2kvp(const string& line, map<string, string>* p_kvp);
    virtual int format2kvp_hashmap(const string& line,
        boost::unordered_map<string, string>* p_kvp);
};


/******************************************************************************
 *                                  TsvParser
 *****************************************************************************/

class TsvParser : public FileParser {
  public:
    TsvParser();
    virtual ~TsvParser();
  private:
    virtual int format2kvp(const string& line, map<string, string>* p_kvp);
    virtual int format2kvp_hashmap(const string& line,
        boost::unordered_map<string, string>* p_kvp);
};

/*---------------------------------------------------------------------------*/
FileParser::FileParser() {
}

FileParser::FileParser(const string& file_path): if_(file_path.c_str()) { }

FileParser::~FileParser() {
  if (if_.good()) {
    if_.close();
  }
}

int FileParser::load_file(const string& file_path) {
  int t_ret = 0;
  if (if_.good()) {
    if_.close();
  }
  keyname_.clear();
  if_.open(file_path.c_str());
  return if_.good() ? 0 : -1;
}

int FileParser::reset() {
  int t_ret = 0;
  if (if_.good()) { if_.close(); }
  return t_ret;
}

int FileParser::define_fields(const vector<string>& fields) {
  keyname_.clear();
  keyname_ = fields;
  return 0;
}

int FileParser::get_fields_define(vector<string>* p_fields) {
  *p_fields = keyname_;
  return 0;
}

// return == 1 : reach the end
// return == 0 : fetch one row successfully
int FileParser::get_line(string* o_line) {
  if (!o_line) return -1;
  return getline(if_, *o_line, '\n') ? 0 : 1;
}

int FileParser::fetch_row(map<string, string>* p_row) {
  if (!p_row) return 0;
  string t_line = "";
  int t_ret = get_line(&t_line);
  if ( 0 == t_ret) {
    format2kvp(t_line, p_row);
  }
  return t_ret;
}

int FileParser::fetch_row(boost::unordered_map<string, string>* p_row) {
  if (!p_row) return 0;
  string t_line = "";
  int t_ret = get_line(&t_line);
  /* std::cout << "get_line: " << t_line << std::endl; */
  if ( 0 == t_ret) {
    format2kvp_hashmap(t_line, p_row);
  }
  return t_ret;
}

/*---------------------------------------------------------------------------*/

CsvParser::CsvParser() { }
CsvParser::~CsvParser() { }
int CsvParser::format2kvp(const string& line, map<string, string>* p_kvp) {
  if (!p_kvp) return 0;
  vector<string> t_v_field;
  string2array(line, ",", &t_v_field);
  size_t t_key_size = keyname_.size();
  for (size_t i = 0, size = t_v_field.size(); i < size; ++i) {
    if (i < t_key_size) {
      p_kvp->insert(make_pair(keyname_[i], t_v_field[i]));
    } else {
      p_kvp->insert(make_pair(int2string(i), t_v_field[i]));
    }
  }
  return 0;
}

int CsvParser::format2kvp_hashmap(const string& line,
    boost::unordered_map<string, string>* p_kvp) {
  if (!p_kvp) return 0;
  vector<string> t_v_field;
  string2array(line, ",", &t_v_field);
  size_t t_key_size = keyname_.size();
  for (size_t i = 0, size = t_v_field.size(); i < size; ++i) {
    if (i < t_key_size) {
      /* std::cout<< "will insert\t" << keyname_[i] << ":" << t_v_field[i] << std::endl; */
      p_kvp->insert(make_pair(keyname_[i], t_v_field[i]));
    } else {
      p_kvp->insert(make_pair(int2string(i), t_v_field[i]));
    }
  }
  return 0;
}

/*---------------------------------------------------------------------------*/

TsvParser::TsvParser() { }
TsvParser::~TsvParser() { }
int TsvParser::format2kvp(const string& line, map<string, string>* p_kvp) {
  if (!p_kvp) return 0;
  vector<string> t_v_field;
  string2array(line, "\t", &t_v_field);
  for (size_t i = 0, size = t_v_field.size(), keysize = keyname_.size();
      i < size && i < keysize; ++i) {
    p_kvp->insert(make_pair(keyname_[i], keyname_[i]));
  }
  return 0;
}

int TsvParser::format2kvp_hashmap(const string& line,
    boost::unordered_map<string, string>* p_kvp) {
  if (!p_kvp) return 0;
  vector<string> t_v_field;
  string2array(line, "\t", &t_v_field);
  for (size_t i = 0, size = t_v_field.size(), keysize = keyname_.size();
      i < size && i < keysize; ++i) {
    p_kvp->insert(make_pair(keyname_[i], t_v_field[i]));
  }
  return 0;
}




