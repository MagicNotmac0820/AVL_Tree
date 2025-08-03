#include <iostream>
#include <array>
#include <ctime>
#include "AVL_Tree.h"

int main(void){
    std::array<int,static_cast<int>(1e6)> arr ;
    for( size_t i = 0 ; i < arr.size() ; ++i )
        arr[i] = i+1 ;

    auto custom_comp = []( const int &num1 , const int &num2 ){
        return num1 < num2 ;
    } ;

    clock_t start_time = clock() ;
    AVL_Tree<int,decltype(custom_comp)> tree( arr.begin() , arr.end() , custom_comp ) ;
    clock_t end_time = clock() ;

    std::cout << "Cost time: " << static_cast<double>(end_time-start_time) / CLOCKS_PER_SEC ;
    std::cout << " secs" << std::endl ;

    tree.remove(1e6) ;
    
    return EXIT_SUCCESS ;
}