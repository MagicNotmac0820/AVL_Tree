#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include <iostream>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <stdexcept>

template<typename T>
class TreeNode{
    private :
        T data ;
        TreeNode<T> *left , *right ;
        int height ;

        void update_height(void){
            int left_height = (!left) ? 0 : left->height ;
            int right_height = (!right) ? 0 : right->height ;

            height = std::max( left_height , right_height ) + 1 ;
            return ;
        }

        int get_balance_factor(void)const {
            int left_height = (!left) ? 0 : left->height ;
            int right_height = (!right) ? 0 : right->height ;

            return left_height - right_height ;
        }
    public :
        TreeNode( const T &_data = T() , TreeNode<T> *_left = nullptr , TreeNode<T> *_right = nullptr ) :
            data(_data) , left(_left) , right(_right) , height(1){}
        TreeNode( const TreeNode &other_node ) :
            data(other_node.data) , 
            left( (!other_node.left) ? nullptr : new TreeNode<T>(*other_node.left) ) ,
            right( (!other_node.right) ? nullptr : new TreeNode<T>(*other_node.right) ) ,
            height(other_node.height){}

        ~TreeNode(){
            delete left ;
            delete right ;
        }
    template<typename U,typename Compare>
    friend class AVL_Tree ;
} ;

template< typename T , typename Compare = std::less<T> >
class AVL_Tree{
    private :
        TreeNode<T> *root ;
        Compare _comp ;
        size_t _size ;

        template<typename _RandomAccessIterator>
        void generate_sorted_case( _RandomAccessIterator , _RandomAccessIterator , TreeNode<T> *& ) ;

        void rotate_clockwise( TreeNode<T> *& ) ;
        void rotate_counter_clockwise( TreeNode<T> *& ) ;

        void adjust_LL_case( TreeNode<T> *& ) ;
        void adjust_LR_case( TreeNode<T> *& ) ;
        void adjust_RL_case( TreeNode<T> *& ) ;
        void adjust_RR_case( TreeNode<T> *& ) ;
        void adjust( TreeNode<T> *& ) ;

        bool insert_recursive( TreeNode<T> *& , const T & ) ;
        bool remove_recursive( TreeNode<T> *& , const T & ) ;
    public :
        AVL_Tree( const Compare &comp = Compare() ) :
            root(nullptr) , _comp(comp) , _size(){}
            
        AVL_Tree( const AVL_Tree &other_tree ) :
            root( (!other_tree.root) ? nullptr : new TreeNode<T>(*other_tree.root) ) ,
            _comp(other_tree._comp) ,
            _size(other_tree._size){}
        template<typename _RandomAccessIterator>
        AVL_Tree( _RandomAccessIterator , _RandomAccessIterator ,
                  const Compare & = Compare() ) ;

        ~AVL_Tree()
        {   delete root ; }

        AVL_Tree &operator=( const AVL_Tree & ) ;

        size_t size(void)const ;
        void insert( const T & ) ;
        void remove( const T & ) ;
        bool query( const T & )const ;
} ;

template<typename T,typename Compare>
template<typename _RandomAccessIterator>
void AVL_Tree<T,Compare>::generate_sorted_case( 
    _RandomAccessIterator __left ,
    _RandomAccessIterator __right ,
    TreeNode<T> *&root
)
{
    if( __left > __right )
        return ;

    auto __mid = __left + (__right-__left) / 2 ;
    root = new TreeNode<T>(*__mid) ;
    generate_sorted_case( __left , __mid-1 , root->left ) ;
    generate_sorted_case( __mid+1 , __right , root->right ) ;
    root->update_height() ;
}

template<typename T,typename Compare>
template<typename _RandomAccessIterator>
AVL_Tree<T,Compare>::AVL_Tree( _RandomAccessIterator __first , _RandomAccessIterator __last ,
                               const Compare &comp ) :
    root(nullptr) , _comp(comp) , _size()
{
    using Value_Type = typename std::iterator_traits<_RandomAccessIterator>::value_type ;
    using Category = typename std::iterator_traits<_RandomAccessIterator>::iterator_category ;
    static_assert( std::is_same<Category,std::random_access_iterator_tag>::value ,
                   "Invalid iterator type: The iterator type must be equal to std::random_access_iterator" ) ;
    static_assert( std::is_same<Value_Type,T>::value ,
                   "Invalid value type: The value type of iterator must be equal to type T") ;

    if( std::distance( __first , __last ) < 0 )
        throw std::range_error("Invalid range: The first iterator must be less than or equal to the last iterator") ;

    if(std::is_sorted( __first , __last , _comp )){
        generate_sorted_case( __first , __last-1 , root ) ;
        _size = std::distance( __first , __last ) ;
        return ;
    } 
    
    for( auto it = __first ; it != __last ; ++it )
        insert(*it) ;
    return ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::rotate_clockwise( TreeNode<T> *&root ){
    TreeNode<T> *newRoot = root->left ;
    root->left = newRoot->right ;
    newRoot->right = root ;

    root->update_height() ;
    newRoot->update_height() ;
    root = newRoot ;
    return ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::rotate_counter_clockwise( TreeNode<T> *&root ){
    TreeNode<T> *newRoot = root->right ;
    root->right = newRoot->left ;
    newRoot->left = root ;

    root->update_height() ;
    newRoot->update_height() ;
    root = newRoot ;
    return ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::adjust_LL_case( TreeNode<T> *&root ){
    if( !root || !root->left )
        return ;
    
    rotate_clockwise(root) ;
    return ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::adjust_LR_case( TreeNode<T> *&root ){
    if( !root || !root->left || !root->left->right )
        return ;
    
    rotate_counter_clockwise(root->left) ;
    rotate_clockwise(root) ;
    return ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::adjust_RL_case( TreeNode<T> *&root ){
    if( !root || !root->right || !root->right->left )
        return ;
    
    rotate_clockwise(root->right) ;
    rotate_counter_clockwise(root) ;
    return ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::adjust_RR_case( TreeNode<T> *&root ){
    if( !root || !root->right )
        return ;
        
    rotate_counter_clockwise(root) ;
    return ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::adjust( TreeNode<T> *&root ){
    if(!root)
        return ;

    int root_balance_factor = root->get_balance_factor() ;
    if( abs(root_balance_factor) < 2 )
        return ;

    if( root_balance_factor > 0 ){
    // Switch to L* case
        int left_balance_factor = root->left->get_balance_factor() ;
        if( left_balance_factor >= 0 )
            adjust_LL_case(root) ;
        else
            adjust_LR_case(root) ;
    }else{
    // Switch to R* case
        int right_balance_factor = root->right->get_balance_factor() ;
        if( right_balance_factor > 0 )
            adjust_RL_case(root) ;
        else
            adjust_RR_case(root) ;
    }

    return ;
}

template<typename T,typename Compare>
bool AVL_Tree<T,Compare>::insert_recursive( TreeNode<T> *&root , const T &target_value ){
    if(!root){
        root = new TreeNode<T>(target_value) ;
        return true ;
    }

    if(_comp( target_value , root->data )){
        if(!insert_recursive( root->left , target_value ))
            return false ;
    }else if(_comp( root->data , target_value )){
        if(!insert_recursive( root->right , target_value ))
            return false ;
    }else
        return false ;

    root->update_height() ;
    adjust(root) ;
    return true ;
}

template<typename T,typename Compare>
bool AVL_Tree<T,Compare>::remove_recursive( TreeNode<T> *&root , const T &target_value ){
    // Condition: target node not found
    if(!root)
        return false ;

    if(_comp( target_value , root->data )){
        if(!remove_recursive( root->left , target_value ))
            return false ;
    }else if(_comp( root->data , target_value )){
        if(!remove_recursive( root->right , target_value ))
            return false ;
    }else{
        if( !root->left && !root->right ){
        // Condition: target node is a leaf node
            delete root ;
            root = nullptr ;
            return true ;
        }else if( root->left && !root->right ){
        // Condition: only exists the left subtree of target node
            TreeNode<T> *tmp = root ;
            root = root->left ;
            // Detach children to prevent them from being deleted along with this node
            tmp->left = tmp->right = nullptr ;
            delete tmp ;
            return true ;
        }else if( !root->left && root->right ){
        // Condition: only exists the right subtree of target node
            TreeNode<T> *tmp = root ;
            root = root->right ;
            // Detach children to prevent them from being deleted along with this node
            tmp->left = tmp->right = nullptr ;
            delete tmp ;
            return true ;
        }

        // Condition: both the left subtree and the right subtree of target node exist
        TreeNode<T> **indirect_candidate_node = &root->right ;
        while((*indirect_candidate_node)->left)
            indirect_candidate_node = &(*indirect_candidate_node)->left ;
        std::swap( root->data , (*indirect_candidate_node)->data ) ;

        remove_recursive( root->right , root->data ) ;
    }

    root->update_height() ;
    adjust(root) ;
    return true ;
}

template<typename T,typename Compare>
AVL_Tree<T,Compare> &AVL_Tree<T,Compare>::operator=( const AVL_Tree<T,Compare> &other_tree ){
    if( this == &other_tree )
        return *this ;

    delete root ;
    root = (!other_tree.root) ? nullptr : new TreeNode<T>(*other_tree.root) ;
    _comp = other_tree._comp ;
    _size = other_tree._size ;

    return *this ;
}

template<typename T,typename Compare>
size_t AVL_Tree<T,Compare>::size(void)const {
    return _size ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::insert( const T &data ){
    if(insert_recursive( root , data ))
        ++_size ;

    return ;
}

template<typename T,typename Compare>
void AVL_Tree<T,Compare>::remove( const T &data ){
    if(remove_recursive( root , data ))
        --_size ;

    return ;
}

template<typename T,typename Compare>
bool AVL_Tree<T,Compare>::query( const T &target_value )const {
    TreeNode<T> *curr = root ;

    while(curr){
        if(_comp( curr->data , target_value ))
            curr = curr->right ;
        else if(_comp( target_value , curr->data ))
            curr = curr->left ;
        else
            return true ;
    }

    return false ;
}

#endif  // __AVL_TREE_H__