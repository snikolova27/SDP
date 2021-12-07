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
    this->maxDepartTime = arrive + time;
}

void MyStore::sortByArrivalMinute(std::vector<const Client*>& byArrival)
{
   std::sort(byArrival.begin(), byArrival.end(),
            [](const Client *a, const Client *b) { return a->arriveMinute <= b->arriveMinute; });
}

void MyStore::sortByDepartureMinute(std::vector< const Client*>& byDeparture)
{
  std::sort(byDeparture.begin(), byDeparture.end(),
            [](const Client *a, const Client *b) { return a->maxDepartTime <= b->maxDepartTime; });
}

std::string getResourceToString(const ResourceType& rt)
{
  if(rt == ResourceType::banana)
	{
	return "banana";	
	}
	return "schweppes";
	
}

void MyStore::sortByMin(std::vector<StoreEvent *> &log) 
{
  std::sort(log.begin(), log.end(),
            [](StoreEvent *a, StoreEvent *b) { return a->minute <= b->minute && a->client.index <= b->client.index; });
                //added index client sort
}

void MyStore::printLog(const std::vector<StoreEvent *> &log) 
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
      std::cout << log[i]->minute << " " << log[i]->client.index << " " << log[i]->client.banana << " "
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
    std::cout << "Enter arrive min: ";
    std::cin >> min;
    checkInput();
    std::cout << "Enter bananas quantity: ";
    std::cin >> bananas;
    checkInput();
    std::cout << "Enter schweppes quantity: ";
    std::cin >> schweppes;
    checkInput();
    std::cout << "Enter max wait time: ";
    std::cin >> wait;
    checkInput();
    std::cout << std::endl;

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
    std::cout << "Enter client count: ";
    std::cin >> clientCnt;
    checkInput();

    std::cout << "Enter workers count:";
    std::cin >> workers;
    checkInput();
    store->workers = workers;

    for (int i = 0; i < clientCnt; i++) {
      std::cout << "Enter information about client №" << i + 1 << std::endl;
      Client current = readClient();
      store->clients.push_back(&current);
      store->clientCnt++;
    }

    store->fillWaiting();
    store->sortWaiting();
  }

int MyStore::findMin(const std::vector< const Client*> byArrival,const std::vector<const Client*> byDeparture, const std::vector<int> workers)
{
  return std::min({byArrival[0]->arriveMinute, byDeparture[0]->maxDepartTime, workers[0]});
}

void copyClients(std::vector< const Client*>source, std::vector< const Client*>& dest )
{
  if(!dest.empty())
  {
    return;
  }

  int size = source.size();
  for(int i=0; i<size;i++)
  {
    dest.push_back(source[i]);
  }
}

 void MyStore::fillWaiting()
 {
   copyClients(this->clients, this->waitingClientsByArrival);

 }

 void MyStore::sortWaiting()
 {
   this->sortByArrivalMinute(this->waitingClientsByArrival);
   this->sortByDepartureMinute(this->waitingClientsByDeparture);
 }

bool MyStore::areBananasEnough(const int & bananas)
{
  return this->bananas >= bananas;
}


bool MyStore::isSchweppesEnough(const int & schweppes)
{
  return this->schweppes >= schweppes;
}

void MyStore::decreaseBananas(const int bananas)
{
  this->bananas-=bananas;
}

void MyStore::decreaseSchweppes(const int schweppes)
{
  this->schweppes-= schweppes;
}

void MyStore:: incrementBananas()
{
  this->bananas+=INCREMENT_STOCK;
}

void MyStore::incrementSchweppes()
{
  this->schweppes+=INCREMENT_STOCK;
}