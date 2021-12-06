#include "interface.h"
#include "MyStore.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>


Client::Client(){};
Client:: Client(const int arrive, const int bananas, const int schweppes, const int time) 
{
    this->arriveMinute = arrive;
    this->banana = bananas;
    this->schweppes = schweppes;
    this->maxWaitTime = time;
}

std::string getResourceToString(const ResourceType& rt)
{
  if(rt == ResourceType::banana)
	{
	return "banana";	
	}
	return "schweppes";
	
}

void sortByMin(std::vector<StoreEvent *> &log) 
{
  std::sort(log.begin(), log.end(),
            [](StoreEvent *a, StoreEvent *b) { return a->minute <= b->minute; });
            //add index client sort
}

void printLog(const std::vector<StoreEvent *> &log) 
{
  int size = log.size();
  for (int i = 0; i < size; i++) 
  {
    if (log[i]->type == StoreEvent::Type::WorkerSend) 
    {
      std::cout << "W " << log[i]->minute << " " << getResourceToString(log[i]->worker.resource)
                << std::endl;
    } 
    else if (log[i]->type == StoreEvent::Type::WorkerBack)
    {
      std::cout << "D " << log[i]->minute << " " <<  getResourceToString(log[i]->worker.resource)
                << std::endl;
    }
    else 
    {
      std::cout << log[i]->client.index << " " << log[i]->client.banana << " "
                << log[i]->client.schweppes << std::endl;
    }
  }
}

  void checkInput()
  {
    if (!std::cin)
    {
      throw std::invalid_argument(
          "Wrong input. Input should be only whole numbers.");
    }
  }

  Client readClient() 
  {
    Client res;
    int min, bananas, schweppes, wait;
    std::cin >> min;
    checkInput();
    std::cin >> bananas;
    checkInput();
    std::cin >> schweppes;
    checkInput();
    std::cin >> wait;
    checkInput();

    res.arriveMinute = min;
    res.banana = bananas;
    res.schweppes = schweppes;
    res.maxWaitTime = wait;

    return res;
  }

  
/// This is sample empty implementation you can place your solution here or
/// delete this and include tests to your solution


 MyStore:: MyStore() { }
  MyStore::MyStore(int workers, int banana, int schweppes)
  {
    this->init(workers, banana, schweppes);
  }


  void MyStore::setActionHandler(ActionHandler *handler)
  {
    this->actionHandler = handler;
  }

  void MyStore::init(int workerCount, int startBanana, int startSchweppes) 
  {
    this->workers = workerCount;
    this->bananas = startBanana;
    this->schweppes = startSchweppes;
  }

  /// Adds a single client to the store
  void MyStore:: addClient(const Client *client)
  {
    this->clients.push_back(client);
    this->clientCnt++;
  }

  /// Adds more than one client to the store
  void MyStore::addClients(const Client *clients, int count) 
  {
      for (int i = 0; i < count; i++) 
      {
      this->clients.push_back(&clients[i]);
      this->clientCnt++;
      }
  }

  void MyStore:: advanceTo(int minute)
  {
    this->time += minute;
    // actionHandler->onWorkerSend(0, ResourceType::banana);
    // actionHandler->onWorkerBack(0, ResourceType::schweppes);
    // actionHandler->onClientDepart(0, 0, 1, 2);
  }

  /// Return the index of a client in the store
  int MyStore:: getClientId(const Client *client)
  {
    std::vector<const Client *>::iterator it =
        std::find(this->clients.begin(), this->clients.end(), client);
    if (it == this->clients.end()) // if client was not found
    {
      return -1;
    }
    return it - this->clients.begin();
  }

  int MyStore::getBanana() const 
  {
     return this->bananas; 
  }

  int MyStore::getSchweppes() const
   {
      return this->schweppes;
   }




Store *createStore()
 { 
   return new MyStore(); 
}

MyStore* createMyStore()
{
  return new MyStore();
}

/// Reads information about the store
  void readStore(MyStore* store)
   {
    int clientCnt, workers;

    std::cin >> clientCnt;
    checkInput();

    std::cin >> workers;
    checkInput();
    store->workers = workers;

    for (int i = 0; i < clientCnt; i++) {
      Client current = readClient();
      store->clients.push_back(&current);
      store->clientCnt++;
    }
  }
