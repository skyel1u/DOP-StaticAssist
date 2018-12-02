#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<int, std::string> mapStudent;
    mapStudent.insert(std::pair<int, std::string>(1, "student_one"));
    mapStudent.insert(std::pair<int, std::string>(1, "student_two"));
    mapStudent.insert(std::pair<int, std::string>(1, "student_three"));
    mapStudent.insert(std::pair<int, std::string>(1, "student_four"));
    std::map<int, std::string>::reverse_iterator iter;
    
    for(iter = mapStudent.rbegin(); iter != mapStudent.rend(); iter++)
    {
        std::cout << iter->first << "  " << iter->second << std::endl;
        
    }

    std::cout << "2333" << std::endl;
    // for(auto x : mapStudent.size())
    // {
    //     std::cout << x << std::endl;
    // }
    

    return 0;
}