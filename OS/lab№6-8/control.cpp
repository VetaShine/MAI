#include <unistd.h>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include "topology.hpp"
#include "zmq_std.hpp"

using NodeIdType = long long;

int main() {
  int parameter;
  topology <NodeIdType, nodes> ControlNode;
  std::vector <std::pair<void*, void*> > childs;
  std::vector <NodeIdType> ids;
  std::string string;
  NodeIdType id;

  while(std::cin >> string >> id) {
    if(string == "create") {
      NodeIdType ParentId;
      std::cin >> ParentId;

      if (ParentId == -1) {
        void* NewContext = NULL;
        void* NewSocket = NULL;
        ZmqStandard::InitPairSocket(NewContext, NewSocket);
        parameter = zmq_bind(NewSocket, ("tcp://*:" + std::to_string(PORT_BASE + id)).c_str());
        assert(parameter == 0);
        int ForkId = fork();

        if(ForkId == 0) {
          parameter = execl(NODE_EXECUTABLE_NAME, NODE_EXECUTABLE_NAME, std::to_string(id).c_str(), NULL);
          assert(parameter != -1);
          return 0;
        } 
        else {
          bool state = true;
          NodeToken ReplyInfo({fail, id, id});
          state = ZmqStandard::RecieveMessageWait(ReplyInfo, NewSocket);
          NodeToken* token = new NodeToken({ping, id, id});
          NodeToken reply({fail, id, id});
          state = ZmqStandard::SendRecieveWait(token, reply, NewSocket);
          
          if(state and reply.action == success) {
            childs.push_back(std::make_pair(NewContext, NewSocket));
            ControlNode.insert(id);
            std::cout << "OK: " << ReplyInfo.id << std::endl;
            ids.push_back(id);
          } 
          else {
            parameter = zmq_close(NewSocket);
            assert(parameter == 0);
            parameter = zmq_ctx_term(NewContext);
            assert(parameter == 0);
          }
        }
      } 
      else if(ControlNode.find(ParentId) == -1) {
        std::cout << "Error: Not found" << std::endl;
      } 
      else {
        if(ControlNode.find(id) != -1) {
          std::cout << "Error: Already exists" << std::endl;
        } 
        else {
          int position = ControlNode.find(ParentId);
          NodeToken* token = new NodeToken({create, ParentId, id});
          NodeToken reply({fail, id, id});
          
          if(ZmqStandard::SendRecieveWait(token, reply, childs[position].second) and reply.action == success) {
            std::cout << "OK: " << reply.id << std::endl;
            ids.push_back(id);
            ControlNode.insert(ParentId, id);
          } 
          else {
            std::cout << "Error: Parent is unavailable" << std::endl;
          }
        }
      } 
    } 
    else if(string == "remove") {
      int position = ControlNode.find(id);
      
      if(position != -1) {
        NodeToken* token = new NodeToken({destroy, id, id});
        NodeToken reply({fail, id, id});
        bool state = ZmqStandard::SendRecieveWait(token, reply, childs[position].second);
        
        if(reply.action == destroy and reply.ParentId == id) {
          parameter = zmq_close(childs[position].second);
          assert(parameter == 0);
          parameter = zmq_ctx_term(childs[position].first);
          assert(parameter == 0);
          std::vector <std::pair <void*, void*>>::iterator IteratorChild = childs.begin();
          
          while(position--) {
            ++IteratorChild;
          }
          childs.erase(IteratorChild);
        } 
        else if(reply.action == bind and reply.ParentId == id) {
          parameter = zmq_close(childs[position].second);
          assert(parameter == 0);
          parameter = zmq_ctx_term(childs[position].first);
          assert(parameter == 0);
          ZmqStandard::InitPairSocket(childs[position].first, childs[position].second);
          parameter = zmq_bind(childs[position].second, ("tcp://*:" + std::to_string(PORT_BASE + reply.id)).c_str());
          assert(parameter == 0);
        }
        if(state) {
          ControlNode.erase(id);
          
          for(auto iterator = ids.begin(); iterator != ids.end(); ++iterator) {
            if (*iterator == id){
              ids.erase(iterator);
              break;
            }
          }
          
          std::cout << "OK" << std::endl;
        } 
        else {
          std::cout << "Error: Node is unavailable" << std::endl;
        }
      } 
      else {
        std::cout << "Error: Not found" << std::endl;
      }
    } 
    else if(string == "heartbit") {
      int position = 0;
      int count = 0;
      double start = 0, end = 0;
      start = clock();

      for(;;) {
        for(auto iterator = ids.begin(); iterator != ids.end(); ++iterator) {
          position = ControlNode.find(*iterator);
          
          if (position == -1) {
            std::cout << "Error: Not found" << *iterator << std::endl;
          }
          else {
            NodeToken* token = new NodeToken({ping, *iterator, *iterator});
            NodeToken reply({fail, *iterator, *iterator});
            
            if(ZmqStandard::SendRecieveWait(token, reply, childs[position].second) and reply.action == success) {
              std::cout << "OK:" << *iterator << std::endl;
            } 
            else {
              std::cout << "Heartbit: node" << *iterator << "is unavailable now" << std::endl;
            }
          }
          
          ++count;
        }
        
        end = clock();

        if(start > 4*id or end > 4*id) {
          break;
        }
      }     
    } 
    else if(string == "back") {
      int position = ControlNode.find(id);
      
      if(position != -1) {
        NodeToken* token = new NodeToken({back, id, id});
        NodeToken reply({fail, id, id});
        
        if(ZmqStandard::SendRecieveWait(token, reply, childs[position].second)) {
          if(reply.action == success) {
            NodeToken* TokenBack = new NodeToken({back, id, id});
            NodeToken ReplyBack({fail, id, id});
            std::vector <unsigned int> calculated;
            
            while(ZmqStandard::SendRecieveWait(TokenBack, ReplyBack, childs[position].second) and ReplyBack.action == success) {
              calculated.push_back(ReplyBack.id);
            }
            
            if(calculated.empty()) {
              std::cout << "OK: " << reply.id << " : -1" << std::endl;
            } 
            else {
              std::cout << "OK: " << reply.id << " : ";
              
              for(size_t index = 0; index < calculated.size() - 1; ++index) {
                std::cout << calculated[index] << ", ";
              }

              std::cout << calculated.back() << std::endl;
            }
          } 
          else {
            std::cout << "Error: No calculations to back" << std::endl;
          }
        } 
        else {
          std::cout << "Error: Node is unavailable" << std::endl;
        }
      } 
      else {
        std::cout << "Error: Not found" << std::endl;
      }
    } 
    else if(string == "exec") {
      std::string pattern, text;
      std::cin >> pattern >> text;
      int position = ControlNode.find(id);
      
      if(position != -1) {
        bool state = true;
        std::string TextPattern = pattern + SENTINEL + text + SENTINEL;
        
        for(size_t index = 0; index < TextPattern.size(); ++index) {
          NodeToken* token = new NodeToken({exec, TextPattern[index], id});
          NodeToken reply({fail, id, id});
          
          if(!ZmqStandard::SendRecieveWait(token, reply, childs[position].second) or reply.action != success) {
            state = false;
            break;
          }
        }
        
        if(state) {
          std::cout << "OK" << std::endl;
        } 
        else {
          std::cout << "Error: Node is unavailable" << std::endl;
        }
      } 
      else {
        std::cout << "Error: Not found" << std::endl;
      }
    }
  }

  for(size_t index = 0; index < childs.size(); ++index) {
    parameter = zmq_close(childs[index].second);
    assert(parameter == 0);
    parameter = zmq_ctx_term(childs[index].first);
    assert(parameter == 0);
  }
}
