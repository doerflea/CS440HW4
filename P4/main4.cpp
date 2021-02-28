//CS440 HW4 Part 4 External Memory Sorting
//Authors: Anneliese Doerfler, Carl Bohme

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

struct emp {
    int eid;
    std::string ename;
    int age;
    double salary;
};
/***************************************************************************************************************************
 Compare eid of employees, lower eid goes first
 ****************************************************************************************************************************/
bool comp(const struct emp &emp1, const struct emp &emp2){
    if(emp1.eid < emp2.eid){
        return true;
    }
    return false;
}
/***************************************************************************************************************************
 Generates runs of size 22 blocks
 ****************************************************************************************************************************/
int initialSort(){
    std::ifstream file_input;
    std::ofstream file_output;
    std::string file_name = "Emp.csv";
    
    file_input.open(file_name.c_str());
    
    if(file_input.fail()){
        std::cout << "Error opening Emp.csv" << std::endl;
        return 0;
    }
    
    int blocks_read = 0;
    std::vector<struct emp> employees;
    std::string str;
    int run = 1;
    
    while(getline(file_input, str)){
        std::istringstream ss(str);
        std::string eid, ename, age, salary;
        std::getline(ss, eid, ',');
        std::getline(ss, ename, ',');
        std::getline(ss, age, ',');
        std::getline(ss, salary, '\n');
        struct emp e = {stoi(eid), ename, stoi(age), stod(salary)};
        
        employees.push_back(e);
        blocks_read++;
        if(blocks_read == 22){
            std::sort(employees.begin(), employees.end(), comp);
            //read out to run
            std::string file_name = "run" + std::to_string(run) + ".txt";
            file_output.open(file_name);
            for(auto e : employees){
                file_output << e.eid << "," << e.ename << "," << e.age << "," << e.salary << std::endl;
            }
            run++;
            file_output.close();
            employees.clear();
            blocks_read = 0;
        }
    }
    if(blocks_read != 0){
        std::string file_name = "run" + std::to_string(run) + ".txt";
        file_output.open(file_name);
        for(auto e : employees){
            file_output << e.eid << "," << e.ename << "," << e.age << "," << e.salary << std::endl;
        }
    }
    file_input.close();
    return run;
    
}
/***************************************************************************************************************************
 Open run files that are currently being merged, up to 21 files
 ****************************************************************************************************************************/
void initalizeFilePtrs(std::vector< std::ifstream > &streams, int &run){
    for (int i = 0; i < 21; i++){
        std::string file_name = "run" + std::to_string(run) + ".txt";
        streams.emplace_back(std::ifstream{file_name});
        if(!streams[i].is_open()){
            streams.pop_back();
            break;
        }
        run++;
    }
}
/***************************************************************************************************************************
 Deletes all the old temporary run files from the last round so that the next round can start from run1.txt again
 ****************************************************************************************************************************/
void deleteOldRuns(int &to_delete_next, int &deletion_rounds, int passes){
    int end = passes;
    int start = to_delete_next;
    for (int i = start; i < end; i++){
        std::string file_name = "run" + std::to_string(i) + ".txt";
        remove(file_name.c_str());
    }
    std::string file_name = "run" + std::to_string(deletion_rounds) + ".txt";
    deletion_rounds++;
    if (std::rename("temp.txt", file_name.c_str()) != 0){
        perror("Error renaming file");
    }
    to_delete_next = end;
}
/***************************************************************************************************************************
 Merges and generates new runs after the intial sort
 ****************************************************************************************************************************/
void multisort(int total_runs){
    std::vector< std::ifstream > streams;
    std::string str;
    
    //Corresponeds to the index of file_ptrs
    std::vector<struct emp>employees(21);
    std::vector<int> eids(21);
    for(int i = 0; i < 21; i++){
        eids[i] = -1;
    }
    int passes = 1;
    int to_delete_next = passes;
    int deletion_rounds = 1;
    int saved_total_runs = total_runs;
    
    std::ofstream outfile;
    int finished_files = 0;
    int min_eid = INT_MAX;
    int index = -1;
    
    //Continue until only one merged file is left
    while(total_runs > 1){
        //Array of file ptrs, will store up to 21 file ptrs or the remaining number of runs, which ever is less
        initalizeFilePtrs(streams, passes);
        
        outfile.open("temp.txt", std::ios_base::app);
        while(finished_files < streams.size()){
            index = -1;
            //Read through array of file ptrs
            for(int i = 0; i < streams.size(); i++){
                //If this file's line has not yet been read, or has already been used, read a new emp into the files position
                if(streams[i].is_open() && eids[i] == -1){
                    if(std::getline(streams[i], str)){
                        std::istringstream ss(str);
                        std::string eid, ename, age, salary;
                        std::getline(ss, eid, ',');
                        std::getline(ss, ename, ',');
                        std::getline(ss, age, ',');
                        std::getline(ss, salary, '\n');
                        struct emp e = {stoi(eid), ename, stoi(age), stod(salary)};
                        employees[i] = e;
                        eids[i] = e.eid;
                    //If the file no longer has any employees left to read, close it
                    } else {
                        streams[i].close();
                        finished_files++;
                        eids[i] = -1;
                    }
                }
                //If this file still had employees left to read, check if the employee has the smallest eid
                if(eids[i] != -1){
                    if(eids[i] < min_eid){
                        min_eid = eids[i];
                        index = i;
                    }
                }
            }
            //Read the employee of the corresponding file index that has the smallest eid
            if(index != -1){
                outfile << employees[index].eid << "," << employees[index].ename << "," << employees[index].age << "," << employees[index].salary << std::endl;
                //Reset so this employee doesn't get used again
                eids[index] = -1;
                min_eid = INT_MAX;
                index = -1;
            }
            
        }
        outfile.close();
        //Reduce runs left to merge by the number of runs merged in theses passes, minus one for the new outfile created
        total_runs -= streams.size()-1;
        deleteOldRuns(to_delete_next, deletion_rounds, passes);
        streams.clear();
        finished_files = 0;
        //When all runs from current round have been merged, start a new round
        if(passes >= saved_total_runs){
            to_delete_next = 1;
            deletion_rounds = 1;
            saved_total_runs = total_runs;
            passes = 1;
        }
    }
}
/***************************************************************************************************************************
 Reads out the last merged run final to a new csv
 ****************************************************************************************************************************/
void readFinalRunToCSV(){
    std::ifstream file_input;
    std::ofstream file_output;
    std::string out_file_name = "EmpSorted.csv";
    std::string in_file_name = "run1.txt";
    
    file_input.open(in_file_name.c_str());
    if(file_input.fail()){
        std::cout << "Error opening last run." << std::endl;
        return;
    }
    file_output.open(out_file_name.c_str());
    if(file_input.fail()){
        std::cout << "Error opening new csv file." << std::endl;
        return;
    }
    std::string str;
    while(getline(file_input, str)){
        file_output << str << std::endl;
    }
    //Delete final run
    std::string file_name = "run1.txt";
    remove(file_name.c_str());
}

int main() {
    int total_runs = initialSort();
    if(total_runs > 0){
        multisort(total_runs);
    }
    readFinalRunToCSV();
    return 0;
}
