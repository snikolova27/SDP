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
    this->popped = false;
}
Client::Client(const Client& other)
{
  this->arriveMinute = other.arriveMinute;
  this->banana = other.banana;
  this->schweppes = other.schweppes;
  this->maxWaitTime = other.maxWaitTime;
  this->maxDepartTime = other.arriveMinute + other.maxWaitTime;
  this->popped = false;
}

bool operator ==(const Client& current, const Client& other)
{
  return (current.arriveMinute == other.arriveMinute && 
          current.banana == other.banana &&
          current.schweppes == other.schweppes &&
          current.maxWaitTime == other.maxWaitTime &&
          current.maxDepartTime == other.maxDepartTime &&
          current.popped == other.popped);
}

void MyStore::sortByArrivalMinute(std::vector<Client>& byArrival)
{
   std::sort(byArrival.begin(), byArrival.end(),
            [](const Client a, const Client b) { return a.arriveMinute <= b.arriveMinute; });
}

void MyStore::sortByDepartureMinute(std::vector< Client>& byDeparture)
{
  std::sort(byDeparture.begin(), byDeparture.end(),
            [](const Client a, const Client b) { return a.maxDepartTime <= b.maxDepartTime; });
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
      std::cout <<  this->log[i].client.index << " " << this->log[i].minute << " " << this->log[i].client.banana << " "
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

  Client readClient() 
  {
    Client res = Client();
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
      
    res.arriveMinute = min;
    res.banana = bananas;
    res.schweppes = schweppes;
    res.maxWaitTime = wait;
    res.maxDepartTime = depart;
    res.popped = false;
   
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
  void MyStore:: addClient(Client client)
  {
    Client toAdd = Client(client.arriveMinute, client.banana, client.schweppes, client.maxWaitTime);
    int maxDepartTime = client.arriveMinute + client.maxWaitTime;
    toAdd.maxDepartTime = maxDepartTime;
    toAdd.popped = false;
    this->clients.push_back(toAdd);
    this->clientCnt++;
   // this->fillWaiting();
    this->waitingClientsByArrival.push_back(toAdd);
   this->waitingClientsByDeparture.push_back(toAdd);
   std::cout << this->waitingClientsByArrival[0].arriveMinute << " " << this->waitingClientsByArrival[0].schweppes << std::endl;
   if(this->waitingClientsByArrival.size() > 1)
   {
   std::cout << this->waitingClientsByArrival[1].arriveMinute << " " << this->waitingClientsByArrival[1].schweppes << std::endl;

   }

    this->sortWaiting();
  }

  /// Adds more than one client to the store
  void MyStore::addClients(const Client *clients, int count) 
  {
      for (int i = 0; i < count; i++) 
      {
        try 
        {
            Client toAdd = Client(clients[i]);
            int maxDepartTime = clients[i].arriveMinute + clients[i].maxWaitTime;
            toAdd.maxDepartTime = maxDepartTime;
            toAdd.popped = false;
            this->clients.push_back(toAdd);
        } 
        catch (...) 
        {
            throw;
        }
 
      this->clientCnt++;
      }
      this->fillWaiting();
      this->sortWaiting();
  }

  void MyStore:: advanceTo(int minute)
  {
    this->time += minute;
    this->generateEvents(minute);
    // actionHandler->onWorkerSend(0, ResourceType::banana);
    // actionHandler->onWorkerBack(0, ResourceType::schweppes);
    // actionHandler->onClientDepart(0, 0, 1, 2);
  }

  /// Return the index of a client in the store
  int MyStore:: getClientId(Client client, std::vector<Client> v)
  {
    std::vector<Client >::iterator it =
        std::find(v.begin(), v.end(), client);
    if (it == v.end()) // if client was not found
    {
      return -1;
    }
    return it - v.begin();
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
      store->clients.push_back(current);
      store->clientCnt++;
    }

    store->fillWaiting();
    store->sortWaiting();
  }

int MyStore::findMin(const std::vector< Client> byArrival,const std::vector<Client> byDeparture, const LinkedQueue<Worker> workers)
{
  int arriveMin = byArrival[0].arriveMinute;
  int departMin = byDeparture[0].maxDepartTime;
  if(!workers.isEmpty())
  {
      int worker = workers.first().comeBackTime;
     return std::min({arriveMin,departMin ,worker });
  }
  return std::min(arriveMin, departMin);

}

void copyClients(const std::vector< Client>source, std::vector< Client>& dest )
{
  if(!dest.empty())
  {
    return;
  }

  int size = source.size();
  for(int i=0; i<size;i++)
  {
    dest.push_back(source[i]); 
    std::cout << dest[i].arriveMinute << " " << dest[i].banana << std::endl;
  }
}

void MyStore:: printClients()
{
  int size = this->clients.size();
  for(int i=0;i<size;i++)
  {
    std::cout << this->clients[i].arriveMinute << " " << this->clients[i].banana << " " <<  this->clients[i].schweppes << " "<< this->clients[i].maxWaitTime << " "<<  this->clients[i].maxDepartTime << std::endl;
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

int MyStore::requestedBananas(Client client)
{
  return client.banana;
}

int MyStore::requestedSchweppes(Client client)
{
  return client.schweppes;
}

bool MyStore::isWorkerAvailable()
{
  return this->workers != 0;
}

bool MyStore::areTheFirstsSame()
{
  const Client arrival = this->waitingClientsByArrival[this->currentFirstInLineArrival];
  const Client departure = this->waitingClientsByDeparture[this->currentFirstInLineDeparture];

  return arrival == departure;
}

Client MyStore::findFirstOfTwo(Client first,  Client second)
{
  int arrivalFirst = first.arriveMinute;
  std::cout << " 347 arrive min first: " <<arrivalFirst << std::endl;
  int arrivalSecond = second.arriveMinute;
  
  return (arrivalFirst < arrivalSecond) ? first : second;
}
int MyStore::whichResource(Client client)
{
  int requestedB = requestedBananas(client);
  int requestedS = requestedSchweppes(client);
  if( requestedB > 0 && requestedS == 0 )
  {
    return 0; //would mean that the client only wants bananas
  }
  else if (requestedB == 0 && requestedS > 0)
  {
    return 1; //would mean that the client only wants schweppes
  }
  else if (requestedB > 0 && requestedS > 0)
  {
    return 2; //would mean that the client wants both
  }  
  else {return -1;}
}

Client MyStore::firstToServe()
{
  int firstByD = findFirstNotPopped(this->waitingClientsByDeparture);
  int firstByA = findFirstNotPopped(this->waitingClientsByArrival);

  Client firstByArrival = this->waitingClientsByArrival[firstByA];
  Client firstByDeparture = this->waitingClientsByDeparture[firstByD];
  // Client* secondByArrival = this->waitingClientsByArrival[nextInLineArrival];
  // Client* secondByDeparture = this->waitingClientsByDeparture[nextInLineDeparture];

  if(firstByArrival == firstByDeparture)
  {
    std::cout << "vlizam na 379" << std::endl;
    std::cout<<firstByArrival.arriveMinute << std::endl;
    return firstByArrival;
  }
  else if (!areAllPopped(this->waitingClientsByArrival) || !areAllPopped(this->waitingClientsByDeparture))
  {
    std::cout << "vlizam na 385" << std::endl;
      return findFirstOfTwo(firstByArrival, firstByDeparture);
  }
  // else if (firstByDeparture->popped)
  // {
  //   int newFirstD = this->findFirstNotPopped(this->waitingClientsByDeparture);

  // }

}

bool MyStore::isWorkerSent(const ResourceType rt)
{
  if(this->workersBackTimes.isEmpty()) //no workers have been sent to restock anything
  {
    return false;
  }

    Client firstClient = firstToServe();
    int  workerBackTime, bananasToBeSold, schweppesToBeSold, timeFirstClient;
    ResourceType resource;

     //Looking at the first client to be served
      bananasToBeSold = requestedBananas(firstClient);
      schweppesToBeSold = requestedSchweppes(firstClient);
      timeFirstClient = firstClient.maxDepartTime;
      
      resource = this->workersBackTimes.first().resource;
      workerBackTime = this->workersBackTimes.first().comeBackTime;

    // Left over stock after the first client has been served  and 
     int leftOverBananas =  this->getBanana() - bananasToBeSold +INCREMENT_STOCK;
     int leftOverScheppes = this->getSchweppes() - schweppesToBeSold + INCREMENT_STOCK;
    
    if( timeFirstClient < workerBackTime) //this worker would't be able to come back in time
    {
      return false;
    }
    else if (bananasToBeSold > 0 && resource == ResourceType::banana && leftOverBananas >=0 ) // worker would have restocked and returned in time
    {
      std::cout << leftOverBananas << " v isWorkerSend banani" << std::endl;
      return true;
      
    }
    else if (schweppesToBeSold > 0 && resource == ResourceType::schweppes &&  leftOverScheppes >= 0)
    {
      return true;
    }

  return false;
}

void MyStore::sendWorker(int minute, const ResourceType rt)
{

  if(this->isWorkerAvailable() && !isWorkerSent(rt))
  {
    Worker worker = Worker(rt, (minute + RESTOCK_TIME));
    StoreEvent ev;
    ev.type = StoreEvent::WorkerSend;
    ev.worker =worker;
    ev.minute = minute;

    this->log.push_back(ev);
    this->workers--;
    this->workersBackTimes.enqueue(worker);
   // this->actionHandler->onWorkerSend(minute, rt);
  }
}

// void MyStore:: emptyClientsVectors()
// {
//   int size = this->clients.size();
//   for(int i=0; i<size;i++)
//   {
//     /// Memory is allocated only when pushing clients in this vector, in the other two there are copies of the pointers
//     delete this->clients[i];
//   }
// }

bool MyStore:: workersLeft()
{
  return this->workers > 0;
}

//TODO: add case where there are no workers left to send and the client takes what is available

int MyStore::doRequest(Client client)
{
  std::cout << "vlizam v do request" << std::endl;
   ResourceType priority = this->higherPriotity(client);
  int min = client.arriveMinute;
  int resources = this->whichResource(client);
  int endMin = min;
  int endMinStart = min;
  int cntRestrockTimesB = 0;
  int cntRestrockTimesS = 0;
  int bananasFinal = 0;
  int schweppesFinal = 0;
  

    if(resources == 0)
    {
      std::cout << " na 474" << std::endl;
      int requestedB = this->requestedBananas(client);

      while(requestedB > this->getBanana())
      {

        if(requestedB > this->getBanana() && !this->workersLeft() && !isWorkerSent(ResourceType::banana)) //if there are no workers left and no workers have been sent - take what's available and client leaves
        {
          client.banana = this->getBanana();
          this->bananas = 0;
          return endMin;
        }

        if(!isWorkerSent(ResourceType::banana)  && this->workersLeft() && bananasFinal <= requestedB) //if there is a worker, send them for the resource needed
        {
          std::cout << "Vlizam vuv vtoriq if 486" << std::endl;
           this->sendWorker(min, ResourceType::banana);
           endMin+= RESTOCK_TIME;
           this->onReturn(min + RESTOCK_TIME, ResourceType::banana);
           bananasFinal+= INCREMENT_STOCK;
        }
       
      }
    
    }
    else if (resources == 1)
    {
      int requestedS = this->requestedSchweppes(client);
      while(requestedS > this->getSchweppes())
      {
        if(requestedS > this->getBanana() && !this->workersLeft() && !isWorkerSent(ResourceType::schweppes)) //if there are no workers left and no worker has been sent for the resource- take what's available and client leaves
        {
          client.schweppes = this->getSchweppes();
          this->schweppes = 0;
          return endMin;
        }

        if(!isWorkerSent(ResourceType::schweppes) && this->workersLeft())
        {
            this->sendWorker(min, ResourceType::schweppes);
            endMin+= RESTOCK_TIME;
            this->onReturn(min + RESTOCK_TIME, ResourceType::schweppes);
        }
     
      }
    }
    else if (resources == 2)
    {
        int requestedB = this->requestedBananas(client);
        int requestedS = this->requestedSchweppes(client);
        if (priority == ResourceType::banana)
        {
          while(requestedB > this->getBanana())
          {

            if(requestedB > this->getBanana() && !this->workersLeft() && !isWorkerSent(ResourceType::banana)) //if there are no workers left and no workers have been sent - take what's available and client leaves
              {
                  client.banana = this->getBanana();
                  this->bananas = 0;
              }
            if( requestedB > this-> getBanana() && !isWorkerSent(ResourceType::banana)  && this->workersLeft()) //if there is a worker, send them for the resource needed
              {
                this->sendWorker(min, ResourceType::banana);
                this->onReturn(min + RESTOCK_TIME, ResourceType::banana);
                cntRestrockTimesB++;
              }
       
            this->sendWorker(min, ResourceType::banana);
          
            if(requestedS > this->getSchweppes() && !this->workersLeft() && !isWorkerSent(ResourceType::schweppes))
            {
                   client.schweppes = this->getSchweppes();
                   this->schweppes = 0;
            }
            if( requestedS > this->getSchweppes() && !isWorkerSent(ResourceType::schweppes)  && this->workersLeft())
            {
              this->sendWorker(min, ResourceType::schweppes);
              this->onReturn(min + RESTOCK_TIME, ResourceType::schweppes);
              cntRestrockTimesS++;
            }
          }
          int restocks = (cntRestrockTimesB > cntRestrockTimesS) ? cntRestrockTimesB : cntRestrockTimesS;
          endMin+= RESTOCK_TIME*restocks;
       
        }
        else 
        {
            while(requestedS >this->getSchweppes())
            {
               if(requestedS > this->getSchweppes() && !this->workersLeft() && !this->isWorkerSent(ResourceType::schweppes)) //if there are no workers left and no worker has been sent for the resource- take what's available and client leaves
               {
                    client.schweppes = this->getSchweppes();
                    this->schweppes = 0;
               
               }
              if( requestedS > this->getSchweppes() && !this->isWorkerSent(ResourceType::schweppes)  && this->workersLeft())
              {
                this->sendWorker(min, ResourceType::schweppes);
                this->onReturn(min + RESTOCK_TIME, ResourceType::schweppes);
                cntRestrockTimesS++;
              }
              
              if(requestedB > this->getBanana() && !this->workersLeft() && !this->isWorkerSent(ResourceType::banana)) //if there are no workers left and no workers have been sent - take what's available and client leaves
              {
                  client.banana = this->getBanana();
                  this->bananas = 0;
              }
              if( requestedB > this-> getBanana() && !this->isWorkerSent(ResourceType::banana)  && this->workersLeft()) //if there is a worker, send them for the resource needed
              {
                this->sendWorker(min, ResourceType::banana);
                this->onReturn(min + RESTOCK_TIME, ResourceType::banana);
                cntRestrockTimesB++;
              }
            }
            int restocks = (cntRestrockTimesB > cntRestrockTimesS) ? cntRestrockTimesB : cntRestrockTimesS;
            endMin+= RESTOCK_TIME*restocks;
        }
      
    }
      return endMin;   
}

void MyStore::closeStore()
{
  //this->emptyClientsVectors();
  this->clients.clear();
  this->waitingClientsByArrival.clear();
  this->waitingClientsByDeparture.clear();
}

void MyStore:: pushClientInLog(int minDepart, int bananas, int schweppes)
{
   StoreEvent ev;
    ev.type = StoreEvent::ClientDepart;
    ev.minute = minDepart;
    ev.client.banana = bananas;
    ev.client.schweppes = schweppes;
    ev.client.index = this->clientInLog;
    this->log.push_back(ev);
      // this->actionHandler->onClientDepart(this->clientInLog, minute, client->banana, client->schweppes);
    this->clientInLog++;
}

void MyStore:: popClient(int minute,  Client client)
{
  int firstByA = findFirstNotPopped(this->waitingClientsByArrival);
  int firstByD = findFirstNotPopped(this->waitingClientsByDeparture);
 Client firstByArrival = this->waitingClientsByArrival[firstByA];
 Client firstByDeparture = this->waitingClientsByDeparture[firstByD];

  if(firstByArrival == firstByDeparture && firstByArrival == client )
  {
    this->waitingClientsByArrival[this->currentFirstInLineArrival].popped = true;
    this->waitingClientsByDeparture[this->currentFirstInLineDeparture].popped = true;
    this->currentFirstInLineArrival++;
    this->currentFirstInLineDeparture++;
  }
  else if ( client == firstByArrival)
  {
    firstByArrival.popped = true;
    this->currentFirstInLineArrival++;

    int idxInDeparture = getClientId(client, this->waitingClientsByDeparture);
    this->waitingClientsByDeparture[idxInDeparture].popped = true; //indicates that this client has been removed from the waiting list
  }
  else if (client == firstByDeparture)
  {
    firstByDeparture.popped = true;
    this->currentFirstInLineDeparture++;

    int idxInArrival = getClientId(client,this->waitingClientsByArrival);
    this->waitingClientsByArrival[idxInArrival].popped = true;

  } 
}
int MyStore:: findFirstNotPopped(std::vector<Client> v)
{
  int size = v.size();
  for(int i=0; i<size;i++)
  {
    if(!v[i].popped)
    {
      return i;
    }
  }
  return -1; //if all are popped
}

bool MyStore::areAllPopped(std::vector <Client> v)
{
  return this->findFirstNotPopped(v) == -1;
}

void MyStore::onReturn(int minute, const ResourceType rt)
{
  //check what resource +
  //increment in the store +
  //increment workers +
  // dequeue worker +
  // push into log +
  //  -the worker +
  // 

  // Increment resource
  if(rt == ResourceType::banana)
  {
    this->incrementBananas();
  }
  else 
  {
    this->incrementSchweppes();
  }

  // Worker is back
  this->workersBackTimes.dequeue();
  this->workers++;

  StoreEvent ev;
  ev.minute = minute;
  ev.type = StoreEvent::WorkerBack;
  ev.worker.resource = rt;

  this->log.push_back(ev);
 // this->actionHandler->onWorkerBack(minute, rt);
}

ResourceType MyStore::higherPriotity(Client client)
{
  int requestedB = this->requestedBananas(client);
  int requestedS = this->requestedSchweppes(client);

  if( requestedB >= requestedS)
  {
    return ResourceType::banana;
  }
  else 
  {
    return ResourceType::schweppes;
  }
}

void MyStore::serve(Client client)
{
  std::cout << "v serve, client, pristignal v min " << client.arriveMinute << std::endl;
  int requestedB = this->requestedBananas(client);
  int requestedS = this->requestedBananas(client);
  std::cout << " v serve banani: " << requestedB << " schweppes: " << requestedS << std::endl;
  
  if ( requestedB <= this->getBanana() && requestedS<= this->getSchweppes())
  {
      this->decreaseBananas(requestedB);
      this->decreaseSchweppes(requestedS);
      this->popClient(client.arriveMinute, client);
      this->pushClientInLog(client.arriveMinute, client.banana, client.schweppes);
  }
  else 
  {
    std::cout << "v serve v else" << std::endl;
      int endMinOfServe = this->doRequest(client);
      
      this->popClient(endMinOfServe,client);
  }

}

void MyStore:: generateEvents(const int upToMin)
{
  if(this->workers == 0 && this->bananas == 0 && this->schweppes==0)  //if there are no workers in the store at all
  {
    if(this->clientCnt == 1)      //only one client
    {StoreEvent ev;
      ev.type = StoreEvent::ClientDepart;
      ev.client.index =0;
      ev.client.banana = 0;
      ev.client.schweppes = 0;
      ev.minute = this->clients[0].maxWaitTime;
      std::cout << ev.minute << " v generate bez rabotnici i edin klient" << std::endl; 
      this->log.push_back(ev);
      this->printLog();
    }
    for(int i=0; i<this->clientCnt;i++)
    {
      StoreEvent ev;
      ev.type = StoreEvent::ClientDepart;
      ev.client.index =i;
      ev.client.banana = 0;
      ev.client.schweppes = 0;
      ev.minute = this->clients[i].maxWaitTime;
      std::cout << ev.minute << " v generate bez rabotnici i mnogo klienti" << std::endl; 
      this->log.push_back(ev);
      this->printLog();
    }
  }
  else if(!this->clientCnt)
  {
    std::cout << "No clients in the store";
    this->closeStore();
  }
  else
  {
     int firstMin  = this->clients[0].arriveMinute;
     assert(upToMin >= firstMin);

      if(this->clientCnt == 1)
      {
          this->serve(this->clients[0]);
      }
      else
      {
        int i=0;
        int jumpTo = this->findMin(this->waitingClientsByArrival, this->waitingClientsByDeparture, this->workersBackTimes);
        while( i <= upToMin && jumpTo <= upToMin)
        {
          if(!areAllPopped(this->waitingClientsByArrival) || !areAllPopped(this->waitingClientsByDeparture))
          {
            std::cout << " vlizam na 793 za put " << i << std::endl;
            Client first = this->firstToServe();
            this->serve(first);
            std::cout << jumpTo << std::endl;
            jumpTo = this->findMin(this->waitingClientsByArrival, this->waitingClientsByDeparture, this->workersBackTimes);
            std::cout << jumpTo << std::endl;
            i++;
          }
          else 
          {
           return;
          }
          
      }   
    }
    
    this->printLog();
   // this->closeStore();

  }
}