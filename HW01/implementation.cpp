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
bool operator ==(const Client& current, const Client& other)
{
  return (current.arriveMinute == other.arriveMinute && 
          current.banana == other.banana &&
          current.schweppes == other.schweppes &&
          current.maxWaitTime == other.maxWaitTime &&
          current.maxDepartTime == other.maxDepartTime);
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

void MyStore::printLog() 
{
  int size = this->log.size();
  for (int i = 0; i < size; i++) 
  {
    if (this->log[i].type == StoreEvent::Type::WorkerSend) 
    {
      std::cout << "W " << this->log[i].minute << " " << getResourceToString(this->log[i].worker.resource)
                << std::endl;
    } 
    else if (this->log[i].type == StoreEvent::Type::WorkerBack)
    {
      std::cout << "D " << this->log[i].minute << " " <<  getResourceToString(this->log[i].worker.resource)
                << std::endl;
    }
    else 
    {
      std::cout << this->log[i].minute << " " << this->log[i].client.index << " " << this->log[i].client.banana << " "
                << this->log[i].client.schweppes << std::endl;
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

  Client* readClient() 
  {
    Client* res = new Client();
    int min, bananas, schweppes, wait, depart;
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
    depart = min + wait;
      
    res->arriveMinute = min;
    res->banana = bananas;
    res->schweppes = schweppes;
    res->maxWaitTime = wait;
    res->maxDepartTime = depart;
   
    std::cin.ignore();
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
  void MyStore:: addClient(const Client* client)
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

// Store *createStore()
//  { 
//    return new MyStore(); 
// }

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
      Client* current = readClient();
      store->clients.push_back(current);
      store->clientCnt++;
    }

    store->fillWaiting();
    store->sortWaiting();
  }

int MyStore::findMin(const std::vector< const Client*> byArrival,const std::vector<const Client*> byDeparture, const std::vector<int> workers)
{
  return std::min({byArrival[0]->arriveMinute, byDeparture[0]->maxDepartTime, workers[0]});
}

void copyClients(const std::vector< const Client*>source, std::vector< const Client*>& dest )
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

void MyStore:: printClients()
{
  int size = this->clients.size();
  for(int i=0;i<size;i++)
  {
    std::cout << this->clients[i]->arriveMinute << " " << this->clients[i]->banana << " " <<  this->clients[i]->schweppes << " "<< this->clients[i]->maxWaitTime << " "<<  this->clients[i]->maxDepartTime << std::endl;
  }
}

 void MyStore::fillWaiting()
 {
   copyClients(this->clients, this->waitingClientsByArrival);
   copyClients(this->clients, this->waitingClientsByDeparture);

 }

 void MyStore::sortWaiting()
 {
   int sizeA = this->waitingClientsByArrival.size();
   int sizeD = this->waitingClientsByDeparture.size();
   if(sizeA == sizeD && sizeA == 1)
   {
     return;
   }

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

int MyStore::requestedBananas(const Client* client)
{
  if( client->banana == 0)
  {
    return -1;
  }
  return client->banana;
}

int MyStore::requestedSchweppes(const Client* client)
{
  if( client->schweppes == 0)
  {
    return -1;
  }
  return client->schweppes;
}

bool MyStore::isWorkerAvailable()
{
  return this->workers != 0;
}

bool MyStore::areTheFirstsSame()
{
  const Client* arrival = this->waitingClientsByArrival[this->currentFirstInLineArrival];
  const Client* departure = this->waitingClientsByDeparture[this->currentFirstInLineDeparture];

  return arrival == departure;
}

const Client* MyStore::findFirstOfTwo(const Client* first, const Client* second)
{
  int arrivalFirst = first->arriveMinute;
  int arrivalSecond = second->arriveMinute;
  
  return (arrivalFirst < arrivalSecond) ? first : second;
}

bool MyStore::isWorkerSent(const ResourceType rt)
{
  if(this->workersBackTimes.isEmpty()) //no workers have been sent to restock
  {
    return false;
  }

  int nextInLineArrival = this->currentFirstInLineArrival + 1;
  int nextInLineDeparture = this->currentFirstInLineDeparture + 1;
  const Client* firstByArrival = this->waitingClientsByArrival[nextInLineArrival -1];
  const Client* firstByDeparture = this->waitingClientsByDeparture[nextInLineDeparture - 1];
  const Client* secondByArrival = this->waitingClientsByArrival[nextInLineArrival];
  const Client* secondByDeparture = this->waitingClientsByDeparture[nextInLineDeparture];

  int requestedB, requestedS, clientMaxDepart, workerBackTime, bananasToBeSold, schweppesToBeSold, timeFirstClient;
  ResourceType resource;
  
//  TODO: iznesi v otdelna funkciq if-ovete OPRAVI FUNKCIQTA

  //Looking at the first client to be served
    if(firstByArrival == firstByDeparture) //if they are the same client
    {
      bananasToBeSold = requestedBananas(firstByArrival);
      schweppesToBeSold = requestedSchweppes(firstByArrival);
      timeFirstClient = firstByArrival->maxDepartTime;
      
    }
    else
    {
      const Client* firstToBeServed = findFirstOfTwo(firstByArrival, firstByDeparture);
      bananasToBeSold = requestedBananas(firstToBeServed);
      schweppesToBeSold = requestedSchweppes(firstToBeServed);
      timeFirstClient = firstToBeServed->maxDepartTime;
  
    }
    //Looking at the next client to be served
    if(secondByArrival == secondByDeparture) // if they are the same client
    {
        requestedB = requestedBananas(secondByArrival);
        requestedS = requestedSchweppes(secondByArrival);
        clientMaxDepart = secondByArrival->maxDepartTime;
    }
    else  //find the one with lesser index == the one who arrived first
    {
      const Client* first = findFirstOfTwo(secondByArrival, secondByDeparture);
      requestedB = requestedBananas(first);
      requestedS = requestedSchweppes(first);
      clientMaxDepart = first->maxDepartTime;

    }
     resource = this->workersBackTimes.first().resource;
     workerBackTime = this->workersBackTimes.first().comeBackTime;

    // Left over stock after the first client has been served 
     int leftOverBananas = this->getBanana() - bananasToBeSold;
     int leftOverScheppes = this->getSchweppes() - schweppesToBeSold;
    
    if(clientMaxDepart < workerBackTime || timeFirstClient < workerBackTime) //this worker would't be able to come back in time
    {
      return false;
    }
    else if (resource == ResourceType::banana && requestedB <= leftOverBananas + INCREMENT_STOCK) // worker would have restocked
    {
      std::cout << leftOverBananas << " v isWorkerSend banani" << std::endl;
      return true;
      
    }
    else if ( resource == ResourceType::schweppes && requestedS <= leftOverScheppes + INCREMENT_STOCK)
    {
      return true;
    }

  return false;
}

void MyStore::sendWorker(int minute, const ResourceType rt)
{
  if(this->isWorkerAvailable() && !isWorkerSent(rt))
  {
    Worker worker = Worker(rt, (minute +60));
    StoreEvent ev;
    ev.type = StoreEvent::WorkerSend;
    ev.worker =worker;
    ev.minute = minute;

    this->log.push_back(ev);
    this->workers--;
    this->workersBackTimes.enqueue(worker);
  }
  else { return; }
}

void MyStore:: emptyClientsVectors()
{
  int size = this->clients.size();
  for(int i=0; i<size;i++)
  {
    /// Memory is allocated only when pushing clients in this vector, in the other two there are copies of the pointers
    delete this->clients[i];
  }
}

void MyStore::closeStore()
{
  this->emptyClientsVectors();
  this->clients.clear();
  this->waitingClientsByArrival.clear();
  this->waitingClientsByDeparture.clear();
}