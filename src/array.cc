#include "array.h"
#include <iostream>
#include <stdexcept>

using size_t = std::size_t;


// check memory leak:    $ valgrind --leak-check=full --show-reachable=yes ./a.out

// -------------------------------------------------------------------------------


template<typename T>
array<T>::array(){
    m_reserved_size = 1;
    m_size = 0;
    //m_elements = nullptr;
    m_elements = (T*) malloc(sizeof(T)*m_reserved_size);
//    for (size_t i=0; i<m_reserved_size; i++) {
//        new (m_elements+i) T();
//    }
}

template<typename T>
array<T>::array(std::initializer_list<T> rhs){
    m_reserved_size = rhs.size();
    m_size = rhs.size();
    m_elements = (T*) malloc(sizeof(T)*m_reserved_size);
    size_t j = 0;
    for (auto i=rhs.begin(); i!=rhs.end(); ++i){
        new (m_elements+j) T(*i);
        j++;
    }

}

template<typename T>
array<T>::array(const array& ar){
    m_reserved_size = ar.m_reserved_size;
    m_size = ar.m_size;
    m_elements = (T*) malloc(sizeof(T)*m_reserved_size);
    for (size_t i=0; i<m_size; i++) {
        new (m_elements+i) T(ar[i]);
    }
}

//move constructor
template<typename T>
array<T>::array(array&& rhs) : m_elements(nullptr), m_size(0), m_reserved_size(0)
{
    m_elements = rhs.m_elements;
    m_size = rhs.m_size;
    m_reserved_size = rhs.m_reserved_size;


    rhs.m_elements = nullptr;
    rhs.m_size = 0;
    rhs.m_reserved_size = 0;
}

//construct with initial "reserved" size
template<typename T>
array<T>::array(size_t t){
    this->m_reserved_size = t;
    this->m_size = 0;
    m_elements = (T*) malloc(sizeof(T)*m_reserved_size);
}

//construct with n copies of t
template<typename T>
array<T>::array(size_t n, const T& t){
    m_reserved_size = n;
    m_size = n;
    m_elements = (T*)malloc(sizeof(T)*m_reserved_size);
    for (int i=0; i<n; i++){
        new (m_elements+i) T(t);
    }

}

//destructor
template<typename T>
array<T>::~array(){
    for(int i=0; i<m_size; i++){
       m_elements[i].~T();
    }
    if(m_elements!=nullptr)
    {
        free(m_elements);
    }
    m_size = 0;
    m_reserved_size = 0;
}

//ensure enough memory for n elements
template<typename T>
void array<T>::reserve(size_t n){


    T* tmp = m_elements;
    m_reserved_size = n;
    m_elements = (T*) malloc(sizeof(T)*m_reserved_size);
    size_t smaller;
    if (n<m_size){
        smaller = n;
    }
    else{
        smaller = m_size;
    }
//    for (int j=0; j<m_reserved_size; j++){
//        new (m_elements+j) T();
//    }

    for (int k=0; k<smaller; k++){
        new (m_elements+k) T(std::move(tmp[k]));
    }

    for (size_t i=0; i<m_size; i++){
        tmp[i].~T();
    }
    if (n<m_size)
        m_size = n;
    if (tmp!= nullptr){
        free(tmp);
        tmp = nullptr;
    }
}

//add to end of vector  -----------------
template<typename T>
void array<T>::push_back(const T& rhs){
    if (m_size >= m_reserved_size){
//        if (m_reserved_size==0){
//
//        }
        reserve(2*m_reserved_size);
    }
    new (m_elements+ (m_size++)) T(rhs);
}

//add to front of vector  ------------------
template<typename T>
void array<T>::push_front(const T& rhs){
    if (m_size >= m_reserved_size){
        reserve(2*m_reserved_size);
    }
    m_size++;
    //new (m_elements+m_size-1) T(std::move(m_elements[m_size-2]));
    for (size_t i=m_size-1; i>0; i--){
        new (m_elements+i) T(std::move(m_elements[i-1]));
        m_elements[i-1].~T();
          //m_elements[i] = std::move(m_elements[i-1]);
    }
//    m_elements[0].~T();
    new (m_elements) T(std::move(rhs));
//    m_elements[0] = std::move(rhs);
}

//remove last element
template<typename T>
void array<T>::pop_back(){
    m_elements[m_size-1].~T();
    m_size--;
}

//remove first element
template<typename T>
void array<T>:: pop_front(){
    m_elements[0].~T();
    for (int i=0; i<m_size-1; i++){
        m_elements[i] = std::move(m_elements[i+1]);
    }
    m_elements[m_size-1].~T();
    m_size--;
}

//return reference to first element
template<typename T>
T& array<T>:: front() const{
    return m_elements[0];

}

//return reference to last element
template<typename T>
T& array<T>::back() const{
    return m_elements[m_size-1];
}

//return reference to specified element
template<typename T>
const T& array<T>::operator[](size_t index) const{
    if (index>m_size-1 || index<0)
        throw std::out_of_range ("blah");
    else
        return m_elements[index];
}

//return reference to specified element
template<typename T>
T& array<T>::operator[](size_t index){
    if (index>m_size-1 || index<0)
        throw std::out_of_range ("blah");
    else
        return m_elements[index];
}

//return number of elements
template<typename T>
size_t array<T>::length() const{
    return m_size;
}

//returns true if empty
template<typename T>
bool array<T>::empty() const{
    return m_size==0;

}

//remove all elements
template<typename T>
void array<T>::clear(){
    for(int i=0; i<m_size; i++){
        m_elements[i].~T();
    }
    m_size = 0;
}

//obtain iterator to first element
template<typename T>
array_iterator<T> array<T>::begin() const{
    array_iterator<T> res(m_elements);
    return res;
}

//obtain iterator to one beyond element
template<typename T>
array_iterator<T> array<T>::end() const{
    array_iterator<T> res(m_elements+m_size-1);
    return res;
}

//remove specified element
template<typename T>
void array<T>::erase(const array_iterator<T>& rhs){
    std::cout<< *rhs << std::endl;
    int tag = 0;
    for (size_t i=0; i<m_size; i++){
        if (tag==1){
            m_elements[i-1] = std::move(m_elements[i]);
        }
        if (m_elements+i == rhs.m_current){
            m_elements[i].~T();
            tag = 1;
        }
        else if (i == m_size-1){
            m_elements[i].~T();
        }

    }
}

//insert element right before itr
template<typename T>
void array<T>::insert(const T& elem, const array_iterator<T>& rhs){
    size_t pos = rhs.m_current - m_elements;
    //std::cout<<*rhs<<std::endl;
    //T* tmp = new T(elem);
    if (m_size >= m_reserved_size){
        reserve(2*m_reserved_size);
    }
    //std::cout<<pos<<std::endl;
    //int tag = 0;
    m_size++;
    for (size_t i=m_size-1; i>pos; i--){
        new (m_elements+i) T(m_elements[i-1]);
        m_elements[i-1].~T();
//        this->print();
//        std::cout<<"----call once ----"<<std::endl;
//        size_t k = i-1;
//        if (m_elements+k == rhs.m_current){
//            //m_elements[k].~T();
//            new (m_elements+k) T(elem);
//            break;
//        }
//        else{
//            if (tag == 0){
//                tag = 1;
//                new (m_elements+k) T(m_elements[k-1]);
//                //m_elements[k-1].~T();
//            }
//            else{
//                new (m_elements+k) T(std::move(m_elements[k-1]));
//                m_elements[k-1].~T();
//            }
//        }
    }

    new (m_elements+pos) T(elem);
    //delete tmp;
}



template<typename T>
array_iterator<T>::array_iterator(){
    m_current = nullptr;
}

template<typename T>
array_iterator<T>::array_iterator(T* rhs){
    m_current = rhs;
}

template<typename T>
array_iterator<T>::array_iterator(const array_iterator& rhs){
    m_current = rhs.m_current;
}

template<typename T>
T& array_iterator<T>::operator*() const{
    return *m_current;
}

template<typename T>
array_iterator<T> array_iterator<T>::operator++(){
    m_current++;
    return *this;
}

template<typename T>
array_iterator<T> array_iterator<T>::operator++(int __unused){
    array_iterator<T> tmp(m_current);
    m_current++;
    return tmp;
}

template<typename T>
bool array_iterator<T>::operator != (const array_iterator& rhs) const{
    bool res = m_current != rhs.m_current;
    return res;
}

template<typename T>
bool array_iterator<T>::operator == (const array_iterator& rhs) const{
    bool res = m_current == rhs.m_current;
    return res;
}