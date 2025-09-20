#include <iostream>
#include <array>
#include <ctime>
#include "AVL_Tree.h"
#define TREE_SIZE (int)1e6

int main(void){
    auto custom_comp = []( const int &num1 , const int &num2 ){
        return num1 > num2 ;
    } ;

    AVL_Tree<int,decltype(custom_comp)> tree(custom_comp) ;
    clock_t start_time = clock() ;
    for( int i = 0 ; i < TREE_SIZE ; ++i )
        tree.insert(i+1) ;
    clock_t end_time = clock() ;

    std::cout << "Cost time: " << static_cast<double>(end_time-start_time) / CLOCKS_PER_SEC ;
    std::cout << " secs" << std::endl ;

    for( int i = 0 ; i < TREE_SIZE ; ++i )
        tree.remove(i) ;
    
    return EXIT_SUCCESS ;
}