#include <iostream>
#include <stdio.h>
#include <string>

using std::string;

#define SQL(...)  #__VA_ARGS__

int main (int argc, char const* argv[])
{
    string query_sql(SQL(
                insert into MusicCommonSettings.crew(
                    ID, NAME, ADDRESS, AGE, PHONE)
                values(%d, "%s", "%s", %d, "%s") 
                ));
    printf("%s\n", query_sql.c_str());
    return 0;
}
