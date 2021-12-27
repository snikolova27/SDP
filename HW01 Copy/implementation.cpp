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
bool operator ==(const StoreEvent& current, const StoreEvent& other)
{
  return (current.type == other.type &&
        current.minute == other.minute &&
        current.client.banana== other.client.banana &&
        current.client.schweppes == other.client.schweppes &&
        current.client.index == other.client.index &&
        current.worker.comeBackTime == other.worker.comeBackTime &&
        current.worker.resource == other.worker.resource);
}

void MyStore::sortByArrivalMinute(std::vector<Client*>& byArrival)
{
   std::sort(byArrival.begin(), byArrival.end(),
            [](const Client* a, const Client* b) { return a->arriveMinute <= b->arriveMinute; });
}

void MyStore::sortByDepartureMinute(std::vector< Client*>& byDeparture)
{
  std::sort(byDeparture.begin(), byDeparture.end(),
            [](const Client* a, const Client* b) { return a->maxDepartTime <= b->maxDepartTime; });
}

std::string getResourceToString(const ResourceType& rt)
{
  if(rt == ResourceType::banana)
	{
	return "banana";	
	}
	return "schweppes";
	
}

void MyStore::sortByMin(std::vector<StoreEvent > &log) 
{
  std::sort(log.begin(), log.end(),
            [](StoreEvent a, StoreEvent b) { return ((a.minute <= b.minute && a.type == StoreEvent::ClientDepart && a.type == b.type && a.client.index <= b.client.index)  //if both events are for clients departing
                                            || (a.minute <= b.minute && a.type == StoreEvent::WorkerSend && b.type!= StoreEvent::WorkerSend) //worker sent
                                            ||(a.minute <= b.minute && a.type == b.type && a.type == StoreEvent::WorkerSend && a.worker.resource == ResourceType::banana)  //both workers, but we first print the bananas
                                            ||(a.minute <= b.minute && a.type == b.type && a.type == StoreEvent::WorkerBack && a.worker.resource == ResourceType::banana)
                                            || (a.minute <= b.minute && a.type != StoreEvent::ClientDepart && b.type == StoreEvent::ClientDepart)); });
              
}

void MyStore::printLog() 
{
  this->sortByMin(this->log);
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

  Client* readClient() 
  {
    try 
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
      res->popped = false;
    
      std::cin.ignore();
      return res;
    } 
    catch (...)
    {
      throw;
    }
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
  void MyStore:: addClient(Client* client)
  {
    try 
    {
       Client* toAdd =  new Client(client->arriveMinute, client->banana, client->schweppes, client->maxWaitTime);
      int maxDepartTime = client->arriveMinute + client->maxWaitTime;
      toAdd->maxDepartTime = maxDepartTime;
      toAdd->popped = false;
      this->clients.push_back(toAdd);
      this->clientCnt++;
   // this->fillWaiting();
      this->waitingClientsByArrival.push_back(toAdd);
      this->waitingClientsByDeparture.push_back(toAdd);
      // std::cout << this->waitingClientsByArrival[0]->arriveMinute << " " << this->waitingClientsByArrival[0]->schweppes << std::endl;
      // if(this->waitingClientsByArrival.size() > 1)
      // {
      //   std::cout << this->waitingClientsByArrival[1]->arriveMinute << " " << this->waitingClientsByArrival[1]->schweppes << std::endl;
      //   }

    this->sortWaiting();
    } 
    catch (...) {
      throw;
    }
   
  }

  /// Adds more than one client to the store
  void MyStore::addClients(const Client *clients, int count) 
  {
      for (int i = 0; i < count; i++) 
      {
        try 
        {
            Client* toAdd =  new Client(clients[i]);
            int maxDepartTime = clients[i].arriveMinute + clients[i].maxWaitTime;
            toAdd->maxDepartTime = maxDepartTime;
            toAdd->popped = false;
            this->clients.push_back(toAdd);
            this->waitingClientsByArrival.push_back(toAdd);
            this->waitingClientsByDeparture.push_back(toAdd);
            this->clientCnt++;
        } 
        catch (...) 
        {
            throw;
        }    
      }
      this->sortWaiting();
  }

  void MyStore:: advanceTo(int minute)
  {
    this->time += minute;
    this->generate(minute);
    // actionHandler->onWorkerSend(0, ResourceType::banana);
    // actionHandler->onWorkerBack(0, ResourceType::schweppes);
    // actionHandler->onClientDepart(0, 0, 1, 2);
  }

  /// Return the index of a client in the store
int MyStore:: getClientId(Client* client, std::vector<Client*> v)
  {
    std::vector<Client* >::iterator it =
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
      Client* current = readClient();
      store->clients.push_back(current);
      store->clientCnt++;
    }

    store->fillWaiting();
    store->sortWaiting();
  }

int MyStore::findMin( std::vector<Client*>& byDeparture, LinkedQueue<Worker> &workers)
{
   int departMin = byDeparture[findFirstNotPopped(byDeparture)]->arriveMinute;
  if(!workers.isEmpty())
  {
      int worker = workers.first().comeBackTime;
     return std::min({departMin ,worker });
  }
  return departMin;

}

void copyClients(const std::vector< Client*>source, std::vector< Client*>& dest )
{
  if(!dest.empty())
  {
    return;
  }

  int size = source.size();
  for(int i=0; i<size;i++)
  {
    dest.push_back(source[i]); 
    std::cout << dest[i]->arriveMinute << " " << dest[i]->banana << std::endl;
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

int MyStore::requestedBananas(Client *client)
{
  return client->banana;
}

int MyStore::requestedSchweppes(Client* client)
{
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

Client* MyStore::findFirstOfTwo(Client* first,  Client* second)
{
  int arrivalFirst = first->arriveMinute;
  std::cout << " 347 arrive min first: " <<arrivalFirst << std::endl;
  int arrivalSecond = second->arriveMinute;
  
  return (arrivalFirst < arrivalSecond) ? first : second;
}
int MyStore::whichResource(Client* client)
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

Client* MyStore::firstToServe()
{
  int firstByD = findFirstNotPopped(this->waitingClientsByDeparture);
  int firstByA = findFirstNotPopped(this->waitingClientsByArrival);
  std::cout << this->waitingClientsByArrival[firstByA]->arriveMinute << " arrive minute v firstToServe" << std::endl;

  Client* firstByArrival = this->waitingClientsByArrival[firstByA];
  Client* firstByDeparture = this->waitingClientsByDeparture[firstByD];
  // Client* secondByArrival = this->waitingClientsByArrival[nextInLineArrival];
  // Client* secondByDeparture = this->waitingClientsByDeparture[nextInLineDeparture];

  if(firstByArrival == firstByDeparture)
  {
    std::cout << "vlizam na 379" << std::endl;
    std::cout<<firstByArrival->arriveMinute << std::endl;
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

    Client* firstClient = firstToServe();
    int  workerBackTime, bananasToBeSold, schweppesToBeSold, timeFirstClient;
    ResourceType resource;

     //Looking at the first client to be served
      bananasToBeSold = requestedBananas(firstClient);
      schweppesToBeSold = requestedSchweppes(firstClient);
      timeFirstClient = firstClient->maxDepartTime;
      
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
    //this->actionHandler->onWorkerSend(minute, rt);
  }
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

bool MyStore:: workersLeft()
{
  return this->workers > 0;
}

int MyStore:: doRequestBananas(Client* client)
{
  int endMin = client->arriveMinute;
  int min = client->arriveMinute;
  int maxDepartMin = client->maxDepartTime;
  int bananasFinal = this->getBanana();
  int requestedB = this->requestedBananas(client);
  std::cout << "shte pravq banani na 522" << std::endl;
   while(bananasFinal < requestedB)
      {
        if(!this->workersLeft() && !isWorkerSent(ResourceType::banana) ) //if there are no workers left and no workers have been sent - take what's available and client leaves
        {
           this->decreaseBananas(bananasFinal);
          this->pushClientInLog(client, maxDepartMin, 0, bananasFinal);
          return endMin;
        }

        // if(!isWorkerSent(ResourceType::banana) && this->workersLeft() && bananasFinal < requestedB && client.maxWaitTime < 60)
        // {
        //   this->pus
        // }
        if(!isWorkerSent(ResourceType::banana)  && this->workersLeft() && bananasFinal < requestedB && endMin <= maxDepartMin) //if there is a worker, send them for the resource needed
        {
          std::cout << "Vlizam vuv vtoriq if 532" << std::endl;
           this->sendWorker(min, ResourceType::banana);
           endMin+= RESTOCK_TIME;
 //          this->onReturn(min + RESTOCK_TIME, ResourceType::banana);
//           bananasFinal+= INCREMENT_STOCK;
  //           std::cout << " v do request banani imam " << bananasFinal << " banana" << std::endl;
        }    

      }

         if (bananasFinal > requestedB && maxDepartMin >= endMin)
        {
           std::cout << "shte pushvam v loga banani" << requestedB << std::endl;
          this->pushClientInLog(client, endMin, requestedB, 0);
          this->decreaseBananas(requestedB);
          return endMin;
        }   
      
      return endMin;

}

int MyStore::doBananas( Client* client, int& endBananas)
{
  int endMin = client->arriveMinute;
  int min = client->arriveMinute;
   int maxDepartMin = client->maxDepartTime;
  int bananasFinal = this->getBanana();
  int requestedB = this->requestedBananas(client);
  std::cout << "shte pravq banani na 563" << std::endl;

  while(bananasFinal < requestedB && maxDepartMin >= endMin)
      {
        if(requestedB > this->getBanana() && !this->workersLeft() && !isWorkerSent(ResourceType::banana)) //if there are no workers left and no workers have been sent - take what's available and client leaves
        {
           this->bananas = 0;
           return endMin;
        }

        if(!isWorkerSent(ResourceType::banana)  && this->workersLeft() && bananasFinal < requestedB && maxDepartMin >= endMin) //if there is a worker, send them for the resource needed
        {
          std::cout << "Vlizam vuv vtoriq if 575" << std::endl;
           this->sendWorker(min, ResourceType::banana);
           endMin+= RESTOCK_TIME;
           //this->onReturn(min + RESTOCK_TIME, ResourceType::banana);
         //  bananasFinal+= INCREMENT_STOCK;
         // std::cout << " v do request banani imam " << bananasFinal << " banana" << std::endl;
        }    
          
      }
      if (bananasFinal > requestedB &&  maxDepartMin >= endMin)
        {
           std::cout << "shte pushvam v loga banani" << requestedB << std::endl;
           endBananas  = requestedB;
         // this->pushClientInLog(client, endMin, requestedB, 0);
          this->decreaseBananas(requestedB);
          return endMin;
        }   
     
      return endMin;

}

///
int MyStore:: doSchweppes(Client* client, int& endSchweppes)
{
  int endMin = client->arriveMinute;
  int min = client->arriveMinute;
  int maxDepartMin = client->maxDepartTime;
  int schweppesFinal= this->getSchweppes();
  int requestedS = this->requestedSchweppes(client);
  std::cout <<"schweppes na 602" << std::endl;
   while(schweppesFinal < requestedS && maxDepartMin >= endMin)
      {
        if(requestedS > this->getSchweppes() && !this->workersLeft() && !isWorkerSent(ResourceType::schweppes)) //if there are no workers left and no worker has been sent for the resource- take what's available and client leaves
        {
          this->schweppes = 0;
          //this->pushClientInLog(client, client.arriveMinute, 0, schweppesFinal);
          return endMin;
        }

        if(!isWorkerSent(ResourceType::schweppes) && this->workersLeft())
        {
            this->sendWorker(min, ResourceType::schweppes);
            endMin+= RESTOCK_TIME;
           // this->onReturn(min + RESTOCK_TIME, ResourceType::schweppes);
           // schweppesFinal+=INCREMENT_STOCK;
           // std::cout << " v do request  2schweppes imam " << schweppesFinal << " schweppes" << std::endl;
        }
     
       
      }
       if (schweppesFinal > requestedS)
        {
          std::cout << "shte pushvam v loga schweppes" << requestedS << std::endl;
          endSchweppes = requestedS;
         // this->pushClientInLog(client, endMin, 0, requestedS);
         // this->printLog();
          this->decreaseSchweppes(requestedS);
          return endMin;
        }

     return endMin;
}

int MyStore:: doRequestSchweppes( Client* client)
{
  int endMin = client->arriveMinute;
  int min = client->arriveMinute;
   int maxDepartMin = client->maxDepartTime;
  int schweppesFinal= this->getSchweppes();
  int requestedS = this->requestedSchweppes(client);
   while(schweppesFinal < requestedS && maxDepartMin >= endMin)
      {
        if(requestedS > this->getSchweppes() && !this->workersLeft() && !isWorkerSent(ResourceType::schweppes)) //if there are no workers left and no worker has been sent for the resource- take what's available and client leaves
        {
          this->schweppes = 0;
          this->pushClientInLog(client, maxDepartMin, 0,schweppesFinal);
           return endMin;
        }

        if(!isWorkerSent(ResourceType::schweppes) && this->workersLeft())
        {
            this->sendWorker(min, ResourceType::schweppes);
            endMin+= RESTOCK_TIME;
           // this->onReturn(min + RESTOCK_TIME, ResourceType::schweppes);
            //schweppesFinal+=INCREMENT_STOCK;
           // std::cout << " v do request schweppes imam " << schweppesFinal << " schweppes" << std::endl;
        }
       
          
      }
 if (schweppesFinal > requestedS)
        {
          std::cout << "shte pushvam v loga schweppes" << requestedS << std::endl;
          this->pushClientInLog(client, endMin, 0, requestedS);
          //this->printLog();
          this->decreaseSchweppes(requestedS);
          return endMin;
        }
     return endMin;
}

//TODO: add case where there are no workers left to send and the client takes what is available

int MyStore::doRequest(Client* client)
{
  std::cout << "vlizam v do request" << std::endl;
   ResourceType priority = this->higherPriotity(client);
  int min = client->arriveMinute;
  int resources = this->whichResource(client);
  int endMin = min;

  int bananasFinal = 0;
  int schweppesFinal= 0;
  
  std::cout << resources << " resurs na 608" << std::endl;

    if(resources == 0)
    {
      std::cout << " na 604" << std::endl;
      return doRequestBananas(client);
    }
    else if (resources == 1)
    {
      std::cout << "shte pravq schweppes" << std::endl;
      return doRequestSchweppes(client);
     
    }
    else if (resources == 2)
    {
      //tuka se chupq
        int endB = this->requestedBananas(client);
        int endS = this->requestedSchweppes(client);
       
        if (priority == ResourceType::banana)
        {
  
          int minB = this->doBananas(client, endB);
          int minS = this->doSchweppes(client, endS);
          endMin = minB > minS ? minB : minS;
          // while(requestedB > this->getBanana())
          // {

          //   if(requestedB > this->getBanana() && !this->workersLeft() && !isWorkerSent(ResourceType::banana)) //if there are no workers left and no workers have been sent - take what's available and client leaves
          //     {
          //         this->bananas = 0;
          //     }
          //   if( bananasFinal < requestedB  && !isWorkerSent(ResourceType::banana)  && this->workersLeft()) //if there is a worker, send them for the resource needed
          //     {
          //       this->sendWorker(min, ResourceType::banana);
          //       this->onReturn(min + RESTOCK_TIME, ResourceType::banana);
          //       bananasFinal+=INCREMENT_STOCK;
          //       cntRestockTimesB++;
          //     }
          //     if(bananasFinal > requestedB)
          //     {
          //       bananasFinal = requestedB;
          //       this->decreaseBananas(bananasFinal - requestedB);
          //     }
       
          //   if(requestedS > this->getSchweppes() && !this->workersLeft() && !isWorkerSent(ResourceType::schweppes))
          //   {
          //       this->schweppes = 0;
          //   }
          //   if( schweppesFinal <requestedS && !isWorkerSent(ResourceType::schweppes)  && this->workersLeft())
          //   {
          //     this->sendWorker(min, ResourceType::schweppes);
          //     this->onReturn(min + RESTOCK_TIME, ResourceType::schweppes);
          //     cntRestockTimesS++;
          //     schweppesFinal+=INCREMENT_STOCK;
          //   }
          //     if(schweppesFinal > requestedS)
          //     {
          //       schweppesFinal = requestedS;
          //       this->decreaseSchweppes(schweppesFinal - requestedS);
          //     }
          // }
          // int restocks = (cntRestockTimesB > cntRestockTimesS) ? cntRestockTimesB : cntRestockTimesS;
          // endMin+= RESTOCK_TIME*restocks;
       
        }
        else 
        {
          int minS = this->doSchweppes(client, endS);
          int minB = this->doBananas(client, endB);
          
          endMin = minB > minS ? minB : minS;
            // while(requestedS >this->getSchweppes())
            // {
            //    if(requestedS > this->getSchweppes() && !this->workersLeft() && !this->isWorkerSent(ResourceType::schweppes)) //if there are no workers left and no worker has been sent for the resource- take what's available and client leaves
            //    {
            //       this->schweppes = 0;
               
            //    }
            //   if(schweppesFinal < requestedS  && !this->isWorkerSent(ResourceType::schweppes)  && this->workersLeft())
            //   {
            //     this->sendWorker(min, ResourceType::schweppes);
            //     this->onReturn(min + RESTOCK_TIME, ResourceType::schweppes);
            //     cntRestockTimesS++;
            //     schweppesFinal+=INCREMENT_STOCK;
            //   }
            //   if(schweppesFinal > requestedS)
            //   {
            //     schweppesFinal = requestedS;
            //     this->decreaseBananas(schweppesFinal - requestedS);
            //   }
              
            //   if(requestedB > this->getBanana() && !this->workersLeft() && !this->isWorkerSent(ResourceType::banana)) //if there are no workers left and no workers have been sent - take what's available and client leaves
            //   {
            //       this->bananas = 0;
            //   }
            //   if( bananasFinal< requestedB  && !this->isWorkerSent(ResourceType::banana)  && this->workersLeft()) //if there is a worker, send them for the resource needed
            //   {
            //     this->sendWorker(min, ResourceType::banana);
            //     this->onReturn(min + RESTOCK_TIME, ResourceType::banana);
            //     cntRestockTimesB++;
            //     bananasFinal+=INCREMENT_STOCK;
            //   }
            //    if(bananasFinal > requestedB)
            //   {
            //     bananasFinal = requestedB;
            //     this->decreaseBananas(bananasFinal - requestedB);
            //   }
            // }
            // int restocks = (cntRestockTimesB > cntRestockTimesS) ? cntRestockTimesB : cntRestockTimesS;
            // endMin+= RESTOCK_TIME*restocks;
        }
       this->pushClientInLog(client, endMin, endB,endS);
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

void MyStore:: pushClientInLog( Client* client,int minDepart, int bananas, int schweppes)
{
    int clientIdx = getClientId(client, this->clients);
     StoreEvent ev;
    ev.type = StoreEvent::ClientDepart;
    ev.minute = minDepart;
    ev.client.banana = bananas;
    ev.client.schweppes = schweppes;
    ev.client.index = clientIdx;
    this->log.push_back(ev);
    this->actionHandler->onClientDepart(clientIdx, minDepart, bananas,schweppes);
    this->clientInLog++;
    this->popClient(minDepart, client);
}

void MyStore:: popClient(int minute,  Client* client)
{
  int firstByA = getClientId(client,this->waitingClientsByArrival);
  int firstByD = getClientId(client,this->waitingClientsByDeparture);
 Client* firstByArrival = this->waitingClientsByArrival[firstByA];
 Client* firstByDeparture = this->waitingClientsByDeparture[firstByD];

  if(firstByArrival == firstByDeparture && firstByArrival == client )
  {
    this->waitingClientsByArrival[firstByA]->popped = true;
    this->waitingClientsByDeparture[firstByD]->popped = true;
    std::cout << " v pop client popva li se klienta " << this->waitingClientsByArrival[firstByA]->popped<<std::endl;
    this->currentFirstInLineArrival++;
    this->currentFirstInLineDeparture++;
  }
  else if ( client == firstByArrival)
  {
    this->waitingClientsByArrival[firstByA]->popped = true;
    this->currentFirstInLineArrival++;

    int idxInDeparture = getClientId(client, this->waitingClientsByDeparture);
    this->waitingClientsByDeparture[idxInDeparture]->popped = true; //indicates that this client has been removed from the waiting list
  }
  else if (client == firstByDeparture)
  {
    this->waitingClientsByDeparture[firstByD]->popped = true;
    this->currentFirstInLineDeparture++;

    int idxInArrival = getClientId(client,this->waitingClientsByArrival);
    this->waitingClientsByArrival[idxInArrival]->popped = true;

  } 
}
int MyStore:: findFirstNotPopped(std::vector<Client*>& v)
{
  int size = v.size();
  for(int i=0; i<size;i++)
  {
    if(!v[i]->popped)
    {
      return i;
    }
  }
  return -1; //if all are popped
}

bool MyStore::areAllPopped(std::vector <Client*> v)
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
  //this->actionHandler->onWorkerBack(minute, rt);
}

ResourceType MyStore::higherPriotity(Client* client)
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

void MyStore::serve(Client* client)
{
  std::cout << "v serve, client, pristignal v min " << client->arriveMinute << std::endl;
  int requestedB = this->requestedBananas(client);
  int requestedS = this->requestedSchweppes(client);
  std::cout << " v serve banani: " << requestedB << " schweppes: " << requestedS << std::endl;
  
  if(requestedB > this->getSchweppes() && requestedS == 0 && client->maxWaitTime < 60)
  {

  }
  if ( requestedB <= this->getBanana() && requestedB > 0 && requestedS == 0)
  {
      this->decreaseBananas(requestedB);
      this->pushClientInLog(client, client->arriveMinute, client->banana, client->schweppes);
      this->popClient(client->arriveMinute, client);
    
  }
  else if(requestedS<= this->getSchweppes() && requestedS > 0 && requestedB == 0)
  {
       this->decreaseSchweppes(requestedS);
      this->pushClientInLog(client, client->arriveMinute, client->banana, client->schweppes);
      this->popClient(client->arriveMinute, client);
  }
  else 
  {
    std::cout << "v serve v else" << std::endl;
    int endMinOfServe = this->doRequest(client);
 
    //this->popClient(endMinOfServe,client);
  }

}

void MyStore:: generateEvents(const int upToMin)
{
  if(this->workers == 0 && this->bananas >0 && this->schweppes > 0 ) //no workers, full store + clients
  {
    int i =0;
    int jumpTo = this->waitingClientsByDeparture[i]->arriveMinute;
        std::cout << jumpTo <<" v pulen magazin" << std::endl;
        while( i< this->clientCnt && jumpTo <= upToMin && jumpTo >= 0 && !areAllPopped(this->waitingClientsByArrival) && !areAllPopped(this->waitingClientsByDeparture))
        {
            std::cout << " vlizam na 962 put " << i << std::endl;
            std::cout << "popnat li e purviq v spisucite " << this->waitingClientsByArrival[0]->popped << std::endl;
             std::cout << "popnat li e purviq v spisucite " << this->waitingClientsByDeparture[0]->popped << std::endl;
            Client* first = this->waitingClientsByDeparture[i];
            std::cout << "to serve client with arrive min" << first->arriveMinute << std::endl;
            this->serve(first);
            std::cout << jumpTo << std::endl;
            i++;
            jumpTo =  this->waitingClientsByDeparture[i]->arriveMinute;
            std::cout << jumpTo << std::endl;
            
      }   
  }

  else if(this->workers == 0 && this->bananas == 0 && this->schweppes == 0)  //if there are no workers in the store at all
  {
    if(this->clientCnt == 1 && this->clients[0]->maxDepartTime <= upToMin)      //only one client
    {
      this->pushClientInLog(this->clients[0], clients[0]->maxDepartTime, 0, 0);
      std::cout <<" v generate bez rabotnici i edin klient" << std::endl; 
      this->printLog();
    }
    else 
    {
      for(int i=0; i<this->clientCnt;i++)
      {
        if (this->clients[i]->maxDepartTime <= upToMin)
        {
          this->pushClientInLog(this->clients[i], clients[i]->maxDepartTime, 0, 0);
          std::cout <<  " v generate bez rabotnici i mnogo klienti" << std::endl; 
        }        
      }
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
     int firstMin  = this->clients[0]->arriveMinute;
     assert(upToMin >= firstMin);

      if(this->clientCnt == 1 && this->clients[0]->maxDepartTime <= upToMin)
      {
          this->serve(this->clients[0]);
      }
      else
      {
        //opravi!
        int i=0;
        int jumpTo = this->findMin(this->waitingClientsByDeparture, this->workersBackTimes);
        std::cout << jumpTo << std::endl;
        while( i>= jumpTo && i <= upToMin && jumpTo <= upToMin && jumpTo >= 0 && !areAllPopped(this->waitingClientsByArrival) && !areAllPopped(this->waitingClientsByDeparture))
        {
        
            std::cout << " vlizam na 793 za put " << i << std::endl;
            std::cout << "popnat li e purviq v spisucite " << this->waitingClientsByArrival[0]->popped << std::endl;
             std::cout << "popnat li e purviq v spisucite " << this->waitingClientsByDeparture[0]->popped << std::endl;
            Client* first = this->firstToServe();
            std::cout << "to serve client with arrive min" << first->arriveMinute << std::endl;
            this->serve(first);
            std::cout << jumpTo << std::endl;
            jumpTo = this->findMin(this->waitingClientsByDeparture, this->workersBackTimes);
            std::cout << jumpTo << std::endl;
            i=jumpTo;
          
      }   
    }
    
    this->printLog();
   // this->closeStore();

  }
}

// int main()
// {
//  MyStore* store = createMyStore();
//  store->init(5,0,0);
// //readStore(store);
// // Client clients [2];
// //store->workers = 2;
// Client cl1 = Client (0,0,10,65);
//  Client cl2 =  Client (2,11,20,75);
// // clients[0] = cl1;
// // clients[1] = cl2;

// Client cl3 = Client(0,10,10,75);
// Client cl4 = Client(2,12,120,70);
// // store->fillWaiting();
// // store->sortWaiting();
// //store->addClients(clients, 2);
// //store->addClient(cl3);
// //store->addClient(cl4);
// //std::cout << store->clients[0].schweppes << std::endl;
// //std::cout << store->waitingClientsByDeparture[0].schweppes << std::endl;
// //std::cout << store->waitingClientsByDeparture[1].arriveMinute << " " << store->waitingClientsByDeparture[1].schweppes << std::endl;

// Client cl [] = {Client(0, 10, 0, 10),
// 		Client(45, 35, 0, 30),
// 		Client(46, 30, 20, 100),
// 		Client(200, 10, 10, 1)};
// store->addClients(cl, 4);
// //std::cout << store->waitingClientsByArrival.size() << std::endl;
// //std::cout << store->requestedBananas(store->waitingClientsByDeparture[0]) << std::endl;
// //store->printClients();
// store->generateEvents(200);//
// }

int MyStore::firstEventMinute()
{
  int workerReturn = -1;
  int departMin = -1;
  int arriveMin = -1;
  if(! this->clients.empty() && !this->waitingClientsByDeparture.empty() 
      && !this->areAllPopped(this->waitingClientsByDeparture) && !areAllPopped(this->clients))
  {
    arriveMin = this->clients[findFirstNotPopped(this->clients)]->arriveMinute;
    departMin = this->waitingClientsByDeparture[findFirstNotPopped(this->waitingClientsByDeparture)]->maxDepartTime;
  }
  
  if(!this->workersBackTimes.isEmpty())
  {
      workerReturn = this->workersBackTimes.first().comeBackTime;
  }

  if(arriveMin >= 0 && departMin >= 0)
  {
    if(workerReturn >= 0)
    {
       return std::min({arriveMin, departMin, workerReturn});
    }
    return std::min(arriveMin, departMin);
  }

  return -1;
 
}

int MyStore::firstEventInMin(const int minute)
{
  int workerReturn = -1;
  int arriveMin = this->clients[findFirstNotPopped(this->clients)]->arriveMinute;
  int departMin = this->waitingClientsByDeparture[findFirstNotPopped(this->waitingClientsByDeparture)]->maxDepartTime;
  if(!this->workersBackTimes.isEmpty())
  {
      workerReturn = this->workersBackTimes.first().comeBackTime;
  }

  if(minute == arriveMin)
  {
    return 0; //indicates that a client has arrived
  }
  else if (minute == departMin)
  {
    return 1; //indicates that a client should leave
  }
  else if (minute == workerReturn && workerReturn)
  {
     return 2; //indicates that a worker has returned
  }
  return -1;
}

void MyStore:: generate(const int upTo)
{  
  if(this->workers == 0 && this->bananas >= 0 && this->schweppes >= 0 && this->clientCnt > 0  && !this->clients.empty())  //no workers, full store + clients
  {
    for(int i =0 ; i< this->clientCnt;i++)
    {
      int arriveMin = this->clients[i]->arriveMinute;
      int departMin = this->clients[i]->arriveMinute + this->clients[i]->maxWaitTime;
      if(arriveMin <= upTo)
      {
        int reqB = this->clients[i]->banana;
        int reqS = this->clients[i]->schweppes;
        int finalB = 0;
        int finalS = 0;
        if(areBananasEnough(reqB))
        {
          finalB = reqB;
          this->decreaseBananas(reqB);
        }
        else //stock is not enough, take what they can
        {
          finalB = this->getBanana();
          this->bananas = 0;
        }
        if(isSchweppesEnough(reqS))
        {
          finalS = reqS;
          this->decreaseSchweppes(reqS);
        }
        else 
        {
          finalS = this->getSchweppes();
          this->schweppes = 0;
        }
        if(departMin <= upTo)
        {
          std::cout << departMin << std::endl;
          this->pushClientInLog(this->clients[i], departMin, finalB, finalS);
          this->clients[i].hasWorkerSent`
          this->popClient(departMin,this->clients[i]);
        }
      }
      else { break;}
    }
  }   
  

}


  // for(int i=0; i<upTo;i++)
  // {
  //   int jump =  this->firstEventMinute();
  //   if(jump != -1)
  //   { 
  //       int eventType = this->firstEventInMin(jump);
  //       if( eventType == 0) //client has arrived
  //       {
  //           //get client
  //           if(this->areAllPopped(this->clients) || this->areAllPopped(this->waitingClientsByArrival) || this->areAllPopped(this->waitingClientsByDeparture))
  //           {
  //             return; //no more clients to serve
  //           }
  //           Client* toServe = this->waitingClientsByArrival[findFirstNotPopped(this->waitingClientsByArrival)];
  //           int requestedBananas = requestedBananas(this->client)
  //       }
  //   }
  

  // }
