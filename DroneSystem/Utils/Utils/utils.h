#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <vector>
#include <utility>
#include <string>

//typedef there

typedef std::vector<std::pair<int, int> > Iteration;
//structurs there
struct Address
{
    int ID;
    std::string IP;
    int PORT;
};
//

//functions there
std::string DataToStr(std::map<int, Address> & data);
std::map<int, Address> StrToData(std::string &str);
void delay(int m);
//

#endif // UTILS_H

