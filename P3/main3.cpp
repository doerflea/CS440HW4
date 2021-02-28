/* 
Title: CS 440 HW4 Programming Part
Description: Merge-Sort Algorithm
Authors: Anneliese Doerfler, Carl Bohme
Date: 3/2/2021
*/
   
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>


using namespace std;



// ####################################################################################
// Strcut for emp blocks (56 Bytes)
// ####################################################################################
struct emp {
   int eid;
   string ename;
   int age;
   double salary;
};



// ####################################################################################
// Struct for dept blocks (56 Bytes)
// ####################################################################################
struct dept {
   int did;
   string dname;
   double budget;
   int managerid;
};



// ####################################################################################
// Gets One EMP Block From File
// ####################################################################################
emp grabEmp(fstream &empin) {
    string line, word;
    emp tempEmp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        tempEmp.eid = stoi(word);
        getline(s, word, ',');
        tempEmp.ename = word;
        getline(s, word, ',');
        tempEmp.age = stoi(word);
        getline(s, word, ',');
        tempEmp.salary = stod(word);
        return tempEmp;
    } else {
        tempEmp.eid = -1;
        return tempEmp;
    }
}



// ####################################################################################
// Gets One EMP Block From File
// ####################################################################################
dept grabDept(fstream &deptin) {
    string line, word;
    dept tempDept;
    if (getline(deptin, line, '\n')) {
        stringstream s(line);
        getline(s, word,',');
        tempDept.did = stoi(word);
        getline(s, word, ',');
        tempDept.dname = word;
        getline(s, word, ',');
        tempDept.budget = stod(word);
        getline(s, word, ',');
        tempDept.managerid = stoi(word);
        return tempDept;
    } else {
        tempDept.did = -1;
        return tempDept;
    }
}



// ####################################################################################
// Sorts EMP File
// ####################################################################################
void sort(){
   fstream empin;
   fstream deptin;
   fstream empout;
   fstream deptout;
   empin.open("Emp.csv", ios::in);
   deptin.open("Dept.csv", ios::in);
   empout.open("SortedEmp.csv", ios::out | ios::app);
   deptout.open("SortedDept.csv", ios::out | ios::app);



   // Emp Sorting Algorithm
   emp e = grabEmp(empin);
   int min;
   int prevMin = -1;
   int count = 1;
   int j = 1;
   for(int i = 0; i < j; i++){

      min = -1;
      count = 0;

      while(!empin.eof()){
         if((e.eid < min && e.eid > prevMin) || (min == -1 && e.eid > prevMin))
            min = e.eid;
         e = grabEmp(empin);
         count++;
      }

      empin.close();
      empin.open("Emp.csv", ios::in);
      e = grabEmp(empin);

      while(!empin.eof()){
         if(e.eid == min)
            empout << e.eid << ',' << e.ename << ',' << e.age << ',' << e.salary << "\n";
         e = grabEmp(empin);
      }

      prevMin = min;
      j = count - 1;
      e = grabEmp(empin);
      empin.close();
      empin.open("Emp.csv", ios::in);
   }



   // Dest Sorting Algorithm
   dept d = grabDept(deptin);
   prevMin = -1;
   count = 1;
   j = 1;
   for(int i = 0; i < j; i++){

      min = -1;
      count = 0;

      while(!deptin.eof()){
         if((d.managerid < min && d.managerid > prevMin) || (min == -1 && d.managerid > prevMin))
            min = d.managerid;
         d = grabDept(deptin);
         count++;
      }

      deptin.close();
      deptin.open("Dept.csv", ios::in);
      d = grabDept(deptin);

      while(!deptin.eof()){
         if(d.managerid == min)
            deptout << d.did << ',' << d.dname << ',' << d.budget << ',' << d.managerid << "\n";
         d = grabDept(deptin);
      }

      prevMin = min;
      j = count - 2;
      d = grabDept(deptin);
      deptin.close();
      deptin.open("Dept.csv", ios::in);
   }


   // Close Open Filestreams
   empin.close();
   deptin.close();
   empout.close();
   deptout.close();

   return;
}



// ####################################################################################
// Merge algorithm
// ####################################################################################
void merge(){
   fstream empin;
   fstream deptin;
   fstream joinout;
   empin.open("SortedEmp.csv", ios::in);
   deptin.open("SortedDept.csv", ios::in);
   joinout.open("Join.csv", ios::out | ios::app);


   int mark = -1;
   emp e = grabEmp(empin);
   dept d = grabDept(deptin);


   while(!deptin.eof()){
      if(mark == -1){
         while(e.eid < d.managerid){
            // Advance e
            e = grabEmp(empin);
         }
         while(d.managerid < e.eid){
            // Advance d
            d = grabDept(deptin);
         }
         mark = e.eid;
      }

      if(e.eid == d.managerid){
         // Create Combined Tuple
         joinout << d.did << ',' << d.dname << ',' << d.budget << ',' << d.managerid << 
         ',' << e.eid << ',' << e.ename << ',' << e.age << ','<< e.salary << "\n";
         
         // Advance e
         e = grabEmp(empin);

      } else {
         // Reset to mark
         empin.seekg(0);
         e = grabEmp(empin);
         while(e.eid != mark){
            e = grabEmp(empin);
         }

         // Advance d
         d = grabDept(deptin);

         // Restel mark
         mark = -1;
      }
   }

   empin.close();
   deptin.close();
   joinout.close();

}


// ####################################################################################
// Main
// ####################################################################################
int main() {
   remove("Join.csv");
   sort();
   merge();
   remove("SortedEmp.csv");
   remove("SortedDept.csv");
   return 0;
}
