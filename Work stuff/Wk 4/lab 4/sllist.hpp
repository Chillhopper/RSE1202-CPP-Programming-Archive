#ifndef SLLIST_HPP
#define SLLIST_HPP

#include <iostream>
#include <cstddef>

namespace hlp2{
    struct node;
    struct sllist;

    // interface to individual elements of singly-linked list
    int data(node const *p); // accessor to node's data
    void data(node *p, int newval); // mutator to node's data
    node* next(node *p); // pointer to successor node
    node const* next(node const *p); // pointer to successor node
    // interface declarations for singly-linked list...

    /*
    \brief      1) construct a empty linked list
    \return     pointer to created empty linked list
    */
    sllist* construct();

    /*
    \brief      2) delete the created linked list
    \param[in]  ptr_sll -> pointer to a singly linked list
    */
    void destruct(sllist *ptr_sll);

    /*
    \brief      3) add a node with a specified value to start of linked list
    \param[in]  ptr_sll -> pointer to linked list
    \param[in]  value   -> value to be added
    */
    void push_front(sllist *ptr_sll, int value);

    /*
    \brief      4) add a node with a specified value to end of linked list
    \param[in]  ptr_sll -> pointer to linked list
    \param[in]  value   -> value to be added
    */
    //void push_back(sllist *ptr_sll, int value);

    /*
    \brief      5) add a node with a specified value to specified index of linked list
    \param[in]  ptr_sll -> pointer to linked list
    \param[in]  value   -> value to be added
    \param[in]  index   -> index of the linked list to be added at
    */
    //void insert(sllist *ptr_sll, int value, size_t index);

    /*
    \brief      6) remove first instance of node with specified value
    \param[in]  ptr_sll -> pointer to linked list
    \param[in]  value   -> value of node to be deleted
    */
    //void remove_first(sllist *ptr_sll, int value);

    /*
    \brief      7) retrieve front of linked list
    \param[in]  ptr_sll -> pointer to linked list
    \return     node at the front of the linked list
    */
    node* front(sllist *ptr_sll);

    /*
    \brief      8) retrieve front of linked list
    \param[in]  ptr_sll -> pointer to linked list
    \return     node at the front of the linked list
    */
    node const* front(sllist const *ptr_sll);

    /*
    \brief      9) Find node which contains specified value
    \param[in]  ptr_sll -> pointer to linked list
    \param[in]  value   -> value to find in linked list
    \return     node which contains specified value
    */
   // node* find(sllist const *ptr_sll, int value);

    /*
    \brief      10) Check is a linked list is empty
    \param[in]  ptr_sll -> pointer to linked list
    \returns    boolean value representing is linked list is empty
    */
   bool empty(sllist const *ptr_sll);

    /*
    \brief      11) Find the size of a linked list
    \param[in]  ptr_sll -> pointer to linked list
    \return     size of linked list
    */
    size_t size(sllist const *ptr_sll);

}


#endif