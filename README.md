# AVL_Tree

## Quick reference
- [Constructor]()
- [Insertion]()
- [Deletion]()
- [Search]()
- [Operator overloading]()
- [Getter function]()

## Introduction
`AVL_Tree` is a generic, self-balancing binary search tree implementation in C++.  
It supports custom comparison functions and efficient insertion, deletion, and search operations.  
For more details about AVL Tree, see:
[AVL Tree](https://en.wikipedia.org/wiki/AVL_tree)

### Constructor
- **AVL_Tree( const Compare &comp = Compare() )** :  
    Constructs an empty AVL tree with an optional custom comparison function.
    
- **AVL_Tree( const AVL_Tree<T,Compare> &other_tree )** :  
    Copy constructor. Creates a deep copy of another AVL tree.

- **AVL_Tree( _RandomAccessIterator __first , _RandomAccessIterator __last ,  
    const Compare &comp = Compare() )** :  
    Constructs an AVL tree from a range `[__first,__last)` of elements.  
    ***Note**: Only `std::random_access_iterator` is supported currently. And the `__first` iterator should be less than or equal to the `__last` iterator; otherwise an range_erro exception will be thrown.*

### Insertion
- **AVL_Tree::insert( const T &data )** :  
    Inserts `data` into the tree. If the data already exists, the tree remains unchanged.

### Deletion
- **AVL_Tree::remove( const T &data )** :  
    Removes `data` from the tree. If the data does not exist, the operation has no effect.

### Searching
- **AVL_Tree::query( const T &data )const** :  
    Returns `true` if `data` exists in the tree; otherwise, returns `false`.

### Operator overloading
- **AVL_Tree::operator=( const AVL_Tree<T,Compare> &other_tree )** :  
    An copy assignment operator overloading of AVL Tree. It also assigns a deep copy of another AVL tree.

### Getter function
- **AVL_Tree::size()const** :  
    Returns the size of tree.