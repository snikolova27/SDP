#include "interface.h"
#include "MyStore.h"
#include <algorithm>
#include <cstddef>
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
    this->waiting = false;
}
Client::Client(const Client& other)
{
  this->arriveMinute = other.arriveMinute;
  this->banana = other.banana;
  this->schweppes = other.schweppes;
  this->maxWaitTime = other.maxWaitTime;
  this->maxDepartTime = other.arriveMinute + other.maxWaitTime;
  this->popped = false;
  this->waiting = false;
}

bool operator ==(const Client& current, const Client& other)
{
  return (current.arriveMinute == other.arriveMinute && 
          current.banana == other.banana &&
          current.schweppes == other.schweppes &&
          current.maxWaitTime == other.maxWaitTime &&
          current.maxDepartTime == other.maxDepartTime &&
          current.popped == other.popped &&
          current.waiting == other.waiting);
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
      throw std::invalid_argument("Wrong input. Input should be only whole numbers.");
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
      res->waiting = false;
    
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

void MyStore:: addClient(Client* client)
{
    try 
    {
       Client* toAdd =  new Client(client->arriveMinute, client->banana, client->schweppes, client->maxWaitTime);
      int maxDepartTime = client->arriveMinute + client->maxWaitTime;
      toAdd->maxDepartTime = maxDepartTime;
      toAdd->popped = false;
      toAdd->waiting = false;
      this->clients.push_back(toAdd);
      this->clientCnt++;
      this->waitingClientsByArrival.push_back(toAdd);
      this->waitingClientsByDeparture.push_back(toAdd);
      this->sortWaiting();
    } 
    catch (...) {
      throw;
    }
   
}

void MyStore::addClients( const Client *clients, int count) 
{
  for (int i = 0; i < count; i++) 
  {
    try 
      {
        Client* toAdd =  new Client(clients[i]);
        int maxDepartTime = clients[i].arriveMinute + clients[i].maxWaitTime;
        toAdd->maxDepartTime = maxDepartTime;
        toAdd->popped = false;
        toAdd->waiting = false;
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
    this->generate(minute);
}

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

void readStore(MyStore* store)
{
    int clientCnt, workers;
    std::cout << "Enter client count: ";
    std::cin >> clientCnt;
    checkInput();

    std::cout << "Enter workers count: ";
    std::cin >> workers;
    checkInput();
    store->workers = workers;

    std::cout << std::endl;

    for (int i = 0; i < clientCnt; i++) 
    {
      std::cout << "Enter information about client №" << i + 1 << std::endl;
      Client* current = readClient();
      store->clients.push_back(current);
      store->clientCnt++;
    }

    store->fillWaiting();
    store->sortWaiting();
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

Client* MyStore::firstToServe()
{
  if(this->areAllPopped(this->clients))
  {
    return nullptr;
  }
   int first = findFirstNotWaiting(this->waitingClientsByArrival);
   Client* firstByArrival = this->waitingClientsByArrival[first];
   return firstByArrival;
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

      //Left over stock after the first client has been served  and 
      int leftOverBananas =  this->getBanana() - bananasToBeSold +INCREMENT_STOCK;
      int leftOverScheppes = this->getSchweppes() - schweppesToBeSold + INCREMENT_STOCK;

    if( timeFirstClient < workerBackTime) //this worker would't be able to come back in time
    {
      return false;
    }
    else if (bananasToBeSold > 0 && resource == ResourceType::banana && leftOverBananas >=0 ) // worker would have restocked and returned in time
    {
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
  if(this->workersLeft())
  {
    Worker worker = Worker(rt, (minute + RESTOCK_TIME));
    StoreEvent ev;
    ev.type = StoreEvent::WorkerSend;
    ev.worker =worker;
    ev.minute = minute;

    this->log.push_back(ev);
    this->workers--;
    this->workersBackTimes.enqueue(worker);
    this->actionHandler->onWorkerSend(minute, rt);
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

void MyStore:: doRequestBananas(Client* client)
{
  int min = client->arriveMinute;
  int requestedB = this->requestedBananas(client);

        //if there is a worker, send them for the resource needed
        if(this->workersLeft())
        {
           this->sendWorker(min, ResourceType::banana);
           this->incomingB += INCREMENT_STOCK;
        }  
        //otherwise we cannot send a worker            
  }

void MyStore:: doRequestSchweppes(Client* client)
{
  int min = client->arriveMinute;
  int requestedS = this->requestedSchweppes(client);
  if(workersLeft())
        {
           this->sendWorker(min, ResourceType::schweppes);
           this->incomingS += INCREMENT_STOCK;
        }  
        //otherwise we cannot send a worker    
}

void MyStore::closeStore()
{
  this->emptyClientsVectors();
  this->clients.clear();
  this->waitingClientsByArrival.clear();
  this->waitingClientsByDeparture.clear();
}

void MyStore:: pushClientInLog(Client* client,int minDepart, int bananas, int schweppes)
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
    this->popClient(client);
}

void MyStore:: popClient(Client* client)
{
  client->popped = true;
  client->waiting = false;
} 

int MyStore:: findFirstNotWaiting(std::vector<Client*>& v)
{
  int size = v.size();
  for(int i=0; i<size;i++)
  {
    if(!v[i]->waiting)
    {
      return i;
    }
  }
  return -1; 
}

int MyStore:: findFirstNotPopped(std::vector <Client*>& v)
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
  return this->findFirstNotWaiting(v) == -1;
}

void MyStore::onReturn(int minute, const ResourceType rt)
{
  // Increment resource
  if(rt == ResourceType::banana)
  {
    this->incrementBananas();
    this->incomingB-=INCREMENT_STOCK;
  }
  else 
  {
    this->incrementSchweppes();
    this->incomingS-=INCREMENT_STOCK;
  }

  // Worker is back
  this->workersBackTimes.dequeue();
  this->workers++;

  StoreEvent ev;
  ev.minute = minute;
  ev.type = StoreEvent::WorkerBack;
  ev.worker.resource = rt;

  this->log.push_back(ev);
  this->actionHandler->onWorkerBack(minute, rt);
}

ResourceType MyStore::higherPriotity(Client* client)
{
  int requestedB = this->requestedBananas(client);
  int requestedS = this->requestedSchweppes(client);
  int notEnoughB = requestedB - (this->getBanana() + this->incomingB);
  int notEnoughS = requestedS - (this->getSchweppes() + this->incomingS);

  if( notEnoughB >= notEnoughS)
  {
    return ResourceType::banana;
  }
  else 
  {
    return ResourceType::schweppes;
  }
}

int MyStore::firstEventInMin(int& jump)
{
  int workerReturn = -1;
  int departMin = -1;
  int arriveMin = -1;

  if(this->findFirstNotPoppedAndNotWaiting(this->clients)!= -1)
  {
     arriveMin = this->clients[this->findFirstNotPoppedAndNotWaiting(this->clients)]->arriveMinute;
  }
 
  if(this->findFirstNotPoppedAndWaiting(this->waitingClientsByDeparture) != -1)
  {
    departMin = this->waitingClientsByDeparture[this->findFirstNotPoppedAndWaiting(this->waitingClientsByDeparture)]->maxDepartTime;
  }

  if(!this->workersBackTimes.isEmpty())
  {
      workerReturn = this->workersBackTimes.first().comeBackTime;
  }
  
  // arriveMin is valid
  if(arriveMin >= 0)
  {
    if(departMin >= 0 && workerReturn < 0 )
    {
        jump = std::min(arriveMin, departMin);
        return (jump == arriveMin) ? 0 : 1;
    }

    if(workerReturn >=0 && departMin >= 0)
    {
      jump = std::min({arriveMin, departMin, workerReturn});
      if (jump == arriveMin)
      {
        return 0; //arrive
      }
      else if (jump == departMin)
      {
        return 1; //depart
      }
      return 2; // worker has returned
    }

    if(departMin < 0 && workerReturn >=0)
    {
      jump = std::min(arriveMin, workerReturn);
      return (jump == arriveMin) ? 0 : 2;
    }
    jump = arriveMin;  
    return 0;
  }

//arriveMin is not valid
  else 
 {
   if(departMin >= 0 && workerReturn < 0 )
    {
      jump = departMin;
      return 1;
    }

    if(workerReturn >=0 && departMin < 0)
    {
      jump = workerReturn;
       return 2;
    }
    if(departMin >= 0 && workerReturn >= 0)
    {
      jump = std::min(departMin, workerReturn);
      return (jump == departMin) ? 1 : 2;
    }
 }
  jump = -1;
  return -1;
}

void MyStore:: generate(const int upTo)
{  
   //no workers, full store + clients
  if(this->workers == 0 && this->bananas >= 0 && this->schweppes >= 0 && this->clientCnt >= 0  && !this->clients.empty()) 
  {
    for(int i =0 ; i< this->clientCnt;i++)
    {
      int arriveMin = this->clients[i]->arriveMinute;
      int departMin;
      if(arriveMin <= upTo)
      {
        int reqB = this->clients[i]->banana;
        int reqS = this->clients[i]->schweppes;
        int finalB = 0;
        int finalS = 0;
        if(areBananasEnough(reqB) && isSchweppesEnough(reqS))
        {
          finalB = reqB;
          finalS = reqS;
          this->decreaseBananas(reqB);
          this->decreaseSchweppes(reqS);
          departMin = arriveMin;
        }
        else //stock is not enough, take what they can
        {
          finalB = this->getBanana();
          this->bananas = 0;
          finalS = this->getSchweppes();
          this->schweppes = 0;
          departMin = this->clients[i]->maxDepartTime;
        }
        if(departMin <= upTo)
        { 
          this->pushClientInLog(this->clients[i], departMin, finalB, finalS);
          this->popClient(this->clients[i]);

        }        
      }
      else { break;}
    }
  }   

  else 
{
  bool vlizam = false;
    while(this->time <= upTo)
      {
        int jump;
        int eventType =  this->firstEventInMin(jump);
        this->time = jump;

        if(jump > upTo || jump == -1)
        {
          this->time = upTo;
          break;
        }

        else  
        { 
            //client has arrived
            if(eventType == 0)
            {
                //get client
                if(this->areAllPopped(this->clients) || this->areAllPopped(this->waitingClientsByArrival) || this->areAllPopped(this->waitingClientsByDeparture))
                {
                  return; //no more clients to serve
                }
                Client* toServe = this->clients[findFirstNotPoppedAndNotWaiting(this->clients)];

                  while(this->workersLeft() && !isStockComing(toServe))
                  {
                     //priority check
                      ResourceType priority = higherPriotity(toServe);
                      if( ResourceType::banana == priority)
                      {
                        doRequestBananas(toServe);
                      }
                      else 
                      {
                        doRequestSchweppes(toServe);
                      }    
                  }
              
              //if a worker comes back
              while(willBeBack(jump))
              {
                vlizam = true;
                ResourceType rt = this->workersBackTimes.first().resource;
                this->onReturn(jump, rt);
              }
              if(vlizam)
              {
                this->fulfillRequestsByArrival(jump);
              }
              if(isStockEnough(toServe) || (toServe->maxDepartTime == jump && toServe->waiting))
              {
                this->fulfillRequest(toServe,jump);
              }
              else 
              {
                toServe->waiting = true;
              }
            }

            //worker has returned
            else if (eventType == 2 && !this->workersBackTimes.isEmpty())
            {
               while(willBeBack(jump))
              {
                vlizam = true;
                ResourceType rt = this->workersBackTimes.first().resource;
                this->onReturn(jump, rt);
              }
              if(vlizam)
              {
                this->fulfillRequestsByArrival(jump);
              }
            }

            // client has to depart
            else if(eventType == 1 && !this->waitingClientsByDeparture.empty() && !this->areAllPopped(this->waitingClientsByDeparture))
            {
               Client* toServe = this->waitingClientsByDeparture[findFirstNotPoppedAndWaiting(this->waitingClientsByDeparture)];
              //get client
              while (toServe->maxDepartTime == jump) 
              {
                  this->fulfillRequest(toServe,jump);
                  if(findFirstNotPoppedAndWaiting(this->waitingClientsByDeparture) == -1)
                  {
                    break;
                  }
                  toServe = this->waitingClientsByDeparture[findFirstNotPoppedAndWaiting(this->waitingClientsByDeparture)];
              }
            } 
        }
    }
  }
}

void MyStore:: fulfillRequest(Client* client, const int minute)
{
  int reqB = this->requestedBananas(client);
  int reqS = this->requestedSchweppes(client);
  int maxDepart = client->maxDepartTime;
  int finalS, finalB;

  if(reqB < this->getBanana())
  {
    finalB = reqB;
  }
  else 
  {
    finalB = this->bananas;
  }
  this->decreaseBananas(finalB);

  if(reqS < this->getSchweppes())
  {
    finalS = reqS;
  }
  else 
  {
    finalS = this->schweppes;
  }
  this->decreaseSchweppes(finalS);
  this->pushClientInLog(client, minute, finalB, finalS);
  this->popClient(client);

}

bool MyStore:: isStockComing(Client* client)
{
  int reqB =  this->requestedBananas(client);
  int reqS = this->requestedSchweppes(client);

  int currentB = this->getBanana();
  int currentS = this->getSchweppes();

  int incomingB = this->incomingB;
  int incomingS = this->incomingS;

  return ((reqB <= currentB + incomingB) && (reqS <= currentS + incomingS));
}

bool MyStore:: willBeBack(const int minute)
{
  if(!this->workersBackTimes.isEmpty())
  {
    int firstBackMinute = this->workersBackTimes.first().comeBackTime;
    return firstBackMinute == minute;
  }
  return false;
}

bool MyStore:: isStockEnough(Client* client)
{
  return this->areBananasEnough(client->banana) && this->isSchweppesEnough(client->schweppes);
}

void MyStore:: fulfillRequestsByArrival(const int minute)
{
  for(int i=0; i<this->clientCnt; i++)
  {
     if(!this->waitingClientsByArrival[i]->popped && this->waitingClientsByArrival[i]->waiting)
     {
       //serve a client who leaves at the given minute
       if(isStockEnough(this->waitingClientsByArrival[i]) || this->waitingClientsByArrival[i]->maxDepartTime == minute)
       {
         this->fulfillRequest(this->waitingClientsByArrival[i], minute);
       }
     }
  }
}

int MyStore::findFirstWaiting(std::vector<Client*>& v)
{
  int size = v.size();
  for(int i=0; i<size;i++)
  {
    if(v[i]->waiting)
    {
      return i;
    }
  }
  return -1; 
}

int MyStore::findFirstNotPoppedAndNotWaiting(std::vector<Client *> &v)
{
  int size = v.size();
  for(int i=0; i<size;i++)
  {
    if(!v[i]->popped && !v[i]->waiting)
    {
      return i;
    }
  }
  return -1;
}

int MyStore::findFirstNotPoppedAndWaiting(std::vector<Client *> &v)
{
  int size = v.size();
  for(int i = 0; i < size; i++)
  {
    if(!v[i]->popped && v[i]->waiting)
    {
      return i;
    }
  }
  return -1;
}