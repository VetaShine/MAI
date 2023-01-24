#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include <iostream>
#include <list>
#include <memory>
#include <utility>

class nodes {
  private:
    std::pair <long long, std::list <nodes>> IdNode;
    using iterator = typename std::list<nodes>::iterator;

  public:
    explicit nodes() noexcept : IdNode() {}

    nodes(const long long &element) {
      IdNode.first = element;
    }

    bool insert(const long long &parent, const long long &element) {
      for (iterator iterators = IdNode.second.begin(); iterators != IdNode.second.end(); ++iterators) { 
        if ((*iterators).GetId() == parent) { 
          nodes NewNode(element);
          (*iterators).IdNode.second.push_back(NewNode);
          return true;
        }
        else if(!(*iterators).EmptyNode()) {
            if((*iterators).insert(parent,element)) {
              return true;
            }
        }
      }

      return false;
    }

    bool insert(const long long &element) {
      this->IdNode.second.push_back(element);
      return true;
    }

    bool erase(const long long &element) {
      for (iterator iterators = this->IdNode.second.begin(); iterators != this->IdNode.second.end(); ++iterators) {
        if ((*iterators).GetId() == element) {
          if (!((*iterators).EmptyNode())) {
            long long id1 = (*iterators).LastElementDelete();
            (*iterators).SetId(id1);
            return true;
          }
          else {
            this->IdNode.second.erase(iterators);
            return true;
          }
        }
        else if(not((*iterators).EmptyNode())) { 
          if ((*iterators).erase(element)) {
            return true;
          }
        }
      }
      return false;
    }

    long long GetId() {
      return this->IdNode.first;
    }

    void SetId(const long long &element) {
      this->IdNode.first = element;
    }

    long long LastElementDelete() {
      if(this->IdNode.second.front().EmptyNode()) { 
        long long id1 = this->IdNode.second.front().GetId();
        this->IdNode.second.pop_front();
        return id1;
      }
      else {
        return this->IdNode.second.front().LastElementDelete();
      } 
    }

    long long find(const long long &element) {
      long long position = 0;
        
      for(iterator iterators = this->IdNode.second.begin(); iterators != this->IdNode.second.end(); ++iterators) {
        if ((*iterators).GetId() == element) {
          return position;
        }
        else if(!(*iterators).EmptyNode()) {
          if ((*iterators).find(element) == 0) {
            return position;
          } 
        }
      }

      return -1;
    }

    bool EmptyNode() {
      return this->IdNode.second.empty();
    }

    ~nodes(){}
};

template<class L, class N> 
class topology {
  private:
    std::shared_ptr <N> root;
    size_t ContainerSize;

  public:
    explicit topology() noexcept : root(nullptr), ContainerSize(0) {}
    ~topology() {}

    bool insert(const L &parent, const L &element) {
      if(this->root != nullptr) {
        if((*root).GetId() == parent) {
          return (*root).insert(element);
        }
        else {
          return (*root).insert(parent, element);
        }
      }
    
      return false;
    }

    void insert(const L &element) {
      if(root == nullptr) {
        N NewNode(element);
        root = std::make_shared<N>(NewNode);
      }
      else {
        std::cout << "ERROR: Root exists" << std::endl;
      }
    }

    long long find(const L &element) {
      long long position = 0;

      if (this->root != nullptr) {
        if((*root).GetId() == element) {
          return position;
        }
        else {
          return (*root).find(element);
        }
      }

      return -1; 
    }
        
    bool erase(const L &element) {
      if (this->root != nullptr) {
        if(((*root).GetId() == element) and (not((*root).EmptyNode()))) {
          long long id1 = (*root).LastElementDelete(); 
          (*root).SetId(id1);
          return true;
        }
        else if(((*root).GetId() == element) and ((*root).EmptyNode())) {
          this->root = nullptr;
          return true;
        }
        else if(((*root).GetId() != element) and (not((*root).EmptyNode()))) {
          return (*root).erase(element);
        }
      }
      return false;
    }

};

#endif
