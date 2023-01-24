#include <pthread.h>
#include <queue>
#include <tuple>
#include <list>
#include <unistd.h>
#include "search.hpp"
#include "zmq_std.hpp"

const std::string SENTINEL_STR = "$";

long long NodeId;
pthread_mutex_t mutex;
pthread_cond_t cond;
std::queue <std::pair <std::string, std::string>> CalculationQueue;
std::queue <std::list <unsigned int>> DoneQueue;

void* ThreadFunction(void*) {
  while(1) {
    pthread_mutex_lock(&mutex);
    
    while(CalculationQueue.empty()) {
      pthread_cond_wait(&cond, &mutex);
    }

    std::pair <std::string, std::string> current = CalculationQueue.front();
    CalculationQueue.pop();
    pthread_mutex_unlock(&mutex);

    if(current.first == SENTINEL_STR and current.second == SENTINEL_STR) {
      break;
    } 
    else {
      std::vector <unsigned int> result = KMPFunction(current.first, current.second);
      std::list <unsigned int> ResultList;

      for (const unsigned int &element : result) {
        ResultList.push_back(element);
      }

      pthread_mutex_lock(&mutex);
      DoneQueue.push(ResultList);
      pthread_mutex_unlock(&mutex);
    }
  }

  return NULL;
}

int main(int ArgumentCount, char** ArgumentVector) {
  int parameter;
  assert(ArgumentCount == 2);
  NodeId = std::stoll(std::string(ArgumentVector[1]));
  
  void* NodeParentContext = zmq_ctx_new();
  void* NodeParentSocket = zmq_socket(NodeParentContext, ZMQ_PAIR);
  parameter = zmq_connect(NodeParentSocket, ("tcp://localhost:" + std::to_string(PORT_BASE + NodeId)).c_str());
  assert(parameter == 0);
  long long ChildId = -1;
  void* NodeContext = NULL;
  void* NodeSocket = NULL;
    
  pthread_t CalculationThread;
  parameter = pthread_mutex_init(&mutex, NULL);
  assert(parameter == 0);
  parameter = pthread_cond_init(&cond, NULL);
  assert(parameter == 0);
  parameter = pthread_create(&CalculationThread, NULL, ThreadFunction, NULL);
  assert(parameter == 0);

  std::string pattern, text;
  bool FlagSentinel = true;

  NodeToken* InformationToken = new NodeToken({info, getpid(), getpid()});
  ZmqStandard::send_msg_dontwait(InformationToken, NodeParentSocket);

  std::list<unsigned int> CurrentCalculated;

  bool HavingChild = false;
  bool awake = true;
  bool calculate = true;

  while(awake) {
    NodeToken token;
    ZmqStandard::RecieveMessage(token, NodeParentSocket);
    NodeToken* reply = new NodeToken({fail, NodeId, NodeId});

    if(token.action == back) {
      if(token.id == NodeId) {
        if(calculate) {
          pthread_mutex_lock(&mutex);
          
          if(DoneQueue.empty()) {
            reply->action = exec;
          } 
          else {
            CurrentCalculated = DoneQueue.front();
            DoneQueue.pop();
            reply->action = success;
            reply->id = getpid();
          }
          
          pthread_mutex_unlock(&mutex);
          calculate = false;
        } 
        else {
          if(CurrentCalculated.size() > 0) {
            reply->action = success;
            reply->id = CurrentCalculated.front();
            CurrentCalculated.pop_front();
          } 
          else {
            reply->action = exec;
            calculate = true;
          }
        }
      } 
      else {
        NodeToken* TokenDown = new NodeToken(token);
        NodeToken ReplyDown(token);
        ReplyDown.action = fail;
      
        if(ZmqStandard::SendRecieveWait(TokenDown, ReplyDown, NodeSocket) and ReplyDown.action == success) {
          *reply = ReplyDown;
        }
      }
    } 
    else if(token.action == bind and token.ParentId == NodeId) {
      ZmqStandard::InitPairSocket(NodeContext, NodeSocket);
      parameter = zmq_bind(NodeSocket, ("tcp://*:" + std::to_string(PORT_BASE + token.id)).c_str());
      assert(parameter == 0);
      HavingChild = true;
      ChildId = token.id;
      NodeToken* TokenPing = new NodeToken({ping, ChildId, ChildId});
      NodeToken ReplayPing({fail, ChildId, ChildId});
    
      if(ZmqStandard::SendRecieveWait(TokenPing, ReplayPing, NodeSocket) and ReplayPing.action == success) {
        reply->action = success;
      }
    } 
    else if(token.action == create) {
      if(token.ParentId == NodeId) {
        if(HavingChild) {
          parameter = zmq_close(NodeSocket);
          assert(parameter == 0);
          parameter = zmq_ctx_term(NodeContext);
          assert(parameter == 0);
        }

        ZmqStandard::InitPairSocket(NodeContext, NodeSocket);
        parameter = zmq_bind(NodeSocket, ("tcp://*:" + std::to_string(PORT_BASE + token.id)).c_str());
        assert(parameter == 0);
        int ForkId = fork();

        if(ForkId == 0) {
          parameter = execl(NODE_EXECUTABLE_NAME, NODE_EXECUTABLE_NAME, std::to_string(token.id).c_str(), NULL);
          assert(parameter != -1);
          return 0;
        } 
        else {
          bool state = true;
          NodeToken ReplyInfo({fail, token.id, token.id});
          state = ZmqStandard::RecieveMessageWait(ReplyInfo, NodeSocket);
          
          if(ReplyInfo.action != fail) {
            reply->id = ReplyInfo.id;
            reply->ParentId = ReplyInfo.ParentId;
          }
          
          if(HavingChild) {
            NodeToken* TokenBind = new NodeToken({bind, token.id, ChildId});
            NodeToken ReplayBind({fail, token.id, token.id});
            state = ZmqStandard::SendRecieveWait(TokenBind, ReplayBind, NodeSocket);
            state = state and (ReplayBind.action == success);
          }
          
          if(state) {
            NodeToken* TokenPing = new NodeToken({ping, token.id, token.id});
            NodeToken ReplayPing({fail, token.id, token.id});
            state = ZmqStandard::SendRecieveWait(TokenPing, ReplayPing, NodeSocket);
            state = state and (ReplayPing.action == success);
            
            if(state) {
              reply->action = success;
              ChildId = token.id;
              HavingChild = true;
            } 
            else {
              parameter = zmq_close(NodeSocket);
              assert(parameter == 0);
              parameter = zmq_ctx_term(NodeContext);
              assert(parameter == 0);
            }
          }
        }
      } 
      else if(HavingChild) {
        NodeToken* TokenDown = new NodeToken(token);
        NodeToken ReplyDown(token);
        ReplyDown.action = fail;
        
        if (ZmqStandard::SendRecieveWait(TokenDown, ReplyDown, NodeSocket) and ReplyDown.action == success) {
          *reply = ReplyDown;
        }
      }
    } 
    else if(token.action == ping) {
      if(token.id == NodeId) {
        reply->action = success;
      } 
      else if(HavingChild) {
        NodeToken* TokenDown = new NodeToken(token);
        NodeToken ReplyDown(token);
        ReplyDown.action = fail;

        if (ZmqStandard::SendRecieveWait(TokenDown, ReplyDown, NodeSocket) and ReplyDown.action == success) {
          *reply = ReplyDown;
        }
      }
    } 
    else if(token.action == destroy) {
      if(HavingChild) {
        if(token.id == ChildId) {
          bool state = true;
          NodeToken* TokenDown = new NodeToken({destroy, NodeId, ChildId});
          NodeToken ReplyDown({fail, ChildId, ChildId});
          state = ZmqStandard::SendRecieveWait(TokenDown, ReplyDown, NodeSocket);
                
          if(ReplyDown.action == destroy and ReplyDown.ParentId == ChildId) {
            parameter = zmq_close(NodeSocket);
            assert(parameter == 0);
            parameter = zmq_ctx_term(NodeContext);
            assert(parameter == 0);
            HavingChild = false;
            ChildId = -1;
          } 
          else if(ReplyDown.action == bind and ReplyDown.ParentId == NodeId) {
            parameter = zmq_close(NodeSocket);
            assert(parameter == 0);
            parameter = zmq_ctx_term(NodeContext);
            assert(parameter == 0);
            ZmqStandard::InitPairSocket(NodeContext, NodeSocket);
            parameter = zmq_bind(NodeSocket, ("tcp://*:" + std::to_string(PORT_BASE + ReplyDown.id)).c_str());
            assert(parameter == 0);
            ChildId = ReplyDown.id;
            NodeToken* TokenPing = new NodeToken({ping, ChildId, ChildId});
            NodeToken ReplayPing({fail, ChildId, ChildId});
            
            if(ZmqStandard::SendRecieveWait(TokenPing, ReplayPing, NodeSocket) and ReplayPing.action == success) {
              state = true;
            }
          }
          if(state) {
            reply->action = success;
          }
        } 
        else if(token.id == NodeId) {
          parameter = zmq_close(NodeSocket);
          assert(parameter == 0);
          parameter = zmq_ctx_term(NodeContext);
          assert(parameter == 0);
          HavingChild = false;
          reply->action = bind;
          reply->id = ChildId;
          reply->ParentId = token.ParentId;
          awake = false;
        } 
        else {
          NodeToken* TokenDown = new NodeToken(token);
          NodeToken ReplyDown(token);
          ReplyDown.action = fail;
          
          if(ZmqStandard::SendRecieveWait(TokenDown, ReplyDown, NodeSocket) and ReplyDown.action == success) {
            *reply = ReplyDown;
          }
        }
      } 
      else if(token.id == NodeId) {
        reply->action = destroy;
        reply->ParentId = NodeId;
        reply->id = NodeId;
        awake = false;
      }
    } 
    else if(token.action == exec) {
      if(token.id == NodeId) {
        char symbol = token.ParentId;
        
        if(symbol == SENTINEL) {
          if(FlagSentinel) {
            std::swap(text, pattern);
          } 
          else {
            pthread_mutex_lock(&mutex);
            
            if(CalculationQueue.empty()) {
              pthread_cond_signal(&cond);
            }
            
            CalculationQueue.push({pattern, text});
            pthread_mutex_unlock(&mutex);
            text.clear();
            pattern.clear();
          }
          
          FlagSentinel = FlagSentinel ^ 1;
        } 
        else {
          text = text + symbol;
        }
        
        reply->action = success;
      } 
      else if(HavingChild) {
        NodeToken* TokenDown = new NodeToken(token);
        NodeToken ReplyDown(token);
        ReplyDown.action = fail;
        
        if(ZmqStandard::SendRecieveWait(TokenDown, ReplyDown, NodeSocket) and ReplyDown.action == success) {
          *reply = ReplyDown;
        }
      }
    }

    ZmqStandard::send_msg_dontwait(reply, NodeParentSocket);
  }

  if(HavingChild) {
    parameter = zmq_close(NodeSocket);
    assert(parameter == 0);
    parameter = zmq_ctx_term(NodeContext);
    assert(parameter == 0);
  }

  parameter = zmq_close(NodeParentSocket);
  assert(parameter == 0);
  parameter = zmq_ctx_term(NodeParentContext);
  assert(parameter == 0);
  pthread_mutex_lock(&mutex);
  
  if(CalculationQueue.empty()) {
    pthread_cond_signal(&cond);
  }

  CalculationQueue.push({SENTINEL_STR, SENTINEL_STR});
  pthread_mutex_unlock(&mutex);

  parameter = pthread_join(CalculationThread, NULL);
  assert(parameter == 0);

  parameter = pthread_cond_destroy(&cond);
  assert(parameter == 0);
  parameter = pthread_mutex_destroy(&mutex);
  assert(parameter == 0);
}
