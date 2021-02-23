#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

struct emp {
   int eid;
   std::string ename;
   int age;
   double salary;
}

/*Compare eid of employees, lower eid goes first*/
bool comp(const struct &emp1, const struct &emp2){
   if(emp1.eid < emp2.eid){
      return true;
   }
   return false;
}

int main() {
   return 0;
}
