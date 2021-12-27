#pragma once
#include "interface.h"
#include "linkedQueue.h"

const int INCREMENT_STOCK = 100;
const int RESTOCK_TIME = 60;

struct MyStore : Store 
{
  int time = 0;
  int bananas = 0;
  int schweppes = 0;
  int workers = 0;    //will decrease if a worker is sent to bring something, will increase when worker comes back
  int clientCnt = 0;
  int incomingB = 0;
  int incomingS = 0;
 
  /// Vector of clients, won't be deleting clients from here
  std::vector<Client*> clients;

  /// Vector of waiting clients sorted by their departure time, will put -1 if a client has been catered to, so we don't have to erase from the vector
  std::vector <Client*> waitingClientsByDeparture; 

  /// Will hold the index of the first client that needs to be catered to by departure, will change this index instead of erasing from the vector
  int currentFirstInLineDeparture = 0;

  /// Vector of waiting clients sorted by their arrival minute, will put -1 if a client has been catered to, so we don't have to erase from the vector
  std::vector< Client*> waitingClientsByArrival;

  /// Will hold the index of the first client that needs to be catered to by arrival, will change this index instead of erasing from the vector
  int currentFirstInLineArrival = 0;

  /// Will hold when the sent workers will be back and with what resourse
  LinkedQueue <Worker> workersBackTimes; 
  
  /// Will hold all the generated events in the store, sorted by minute
  std::vector<StoreEvent> log;

  ActionHandler *actionHandler = nullptr;


  MyStore();
  MyStore(int workers, int banana, int schweppes);
   ~MyStore()
  {
    this->closeStore();
  }

// ---------------- Log ---------------- 

  /// @brief Prints the log
  void printLog();


// ---------------- Store ---------------- 

  virtual void setActionHandler(ActionHandler *handler) override ;

  virtual void init(int workerCount, int startBanana, int startSchweppes) override;

  virtual void advanceTo(int minute) override;

  /// @brief Adds a single client to the store
  /// @param client - client to add
  void addClient( Client* client);

  /// @brief Adds more than one client to the store
  /// @param clients - vector/array of clients to add
  /// @param count - count of clients to add
  void addClients(const Client *clients, int count) override;

  /// @brief Generate all events for the store up to a given minute
  /// @param upToMin - minute
  void generate(const int upTo);

  /// Clear the vectors of client
  void closeStore();

  ///@brief Sort the waiting lists
  void sortWaiting();

  /// @brief Fills the waiting clients vectors
  void fillWaiting();

  /// @brief Returns current banana quantity in the store
  virtual int getBanana() const override ;

  /// @brief Returns current schweppes quantity in the store
  virtual int getSchweppes() const override;

 private:

// ---------------- Checks ---------------- 

  /// @brief Returns the type of event to happen in a given minute;
  /// @param jump - the minute of first event;
  /// 0 indicates that a client has arrived;
  /// 1 indicates that a client should leave;
  /// 2 indicates that a worker will come back;
  /// -1 otherwise
  int firstEventInMin(int& jump);

  /// @brief Returns if resources will be enough to satistfy client's request
  /// @param client - the client
  bool isStockComing(Client* client);

  /// @brief Return if worker will come back at the given minute
  /// @param minute - given minute
  bool willBeBack(const int minute);

  /// @brief Return if stock is enough for client's request
  /// @param client - client with request
  bool isStockEnough(Client* client);

  /// @brief Checks if there is already a worker sent that can satisfy the request of the first client to be served
  ///@param rt - resource which the worker is sent for
  bool isWorkerSent(const ResourceType rt);

  /// @brief Return if there are any workers left in the store
  bool workersLeft();

  /// @brief Returns if all clients in a vector are popped == have left
  /// @param v - vector to check in
  bool areAllPopped(std::vector <Client*> v);

   /// @brief Return resource with higher priority
   /// @param client - client with request
   ResourceType higherPriotity(Client* client);

   /// @brief Check how many bananas a client wants, returns -1 if they don't want any 
   /// @param client - client with request 
   int requestedBananas( Client* client);

   /// @brief Check how much Schweppes a client wants, returns -1 if they don't want any 
   /// @param client - client with request 
   int requestedSchweppes( Client* client);

   /// @brief Checks if there are enough bananas available for the request
   /// @param bananas - bananas in request
   bool areBananasEnough(const int & bananas);

  /// @brief Checks if there is enough schweppes available for the request
  /// @param schweppes - schweppes in request
  bool isSchweppesEnough(const int & schweppes);


// ---------------- Find ---------------- 

  /// @brief Find the first waiting client
  /// @param v - vector to search in
  /// @return index of client
  int findFirstWaiting(std::vector<Client*>& v);

  /// @brief Returns the first client to be served
  Client* firstToServe();

  ///@brief Return the index of a client in the store;
  ///@param v - vector to search in
  int getClientId(Client* client, std::vector<Client*> v);

  /// @brief Return the index of the first non-popped client from a vector
  /// @param v - vector to search in
  int findFirstNotPopped(std::vector <Client*>& v);

  /// @brief Return the index of the first client for whom no workers have been send to fulfill their request
  /// @param v - vector to search in
  int findFirstNotWaiting(std::vector<Client*>& v);

  /// @brief Return index of first client who is waiting and has not been 
  /// @param v - vector to search in 
  int findFirstNotPoppedAndWaiting(std::vector<Client *> &v);

  /// @brief Return index of first client who is not waiting and has not been popped
  /// @param v - vector to search in
  int findFirstNotPoppedAndNotWaiting(std::vector<Client *> &v);


// ---------------- Do requests ---------------- 

  /// @brief Serve clients by arrival up to given minute
  /// @param minute - given minute
  void fulfillRequestsByArrival(const int minute);

  /// @brief Serve client with what's available at given minute
  /// @param client - client to serve
  /// @param minute - guven minute
  void fulfillRequest(Client* client, const int minute);

  /// @brief Serve bananas to a client
  /// @param client - client to serve
  void doRequestBananas( Client* client);

  /// @brief Serve schweppes to a client
  /// @param client - client to serve
  void doRequestSchweppes(Client* client);


  // ---------------- Log ---------------- 

  /// @brief Push client into log
  /// @param client - client to push
  /// @param min - minute of departure of client
  /// @param bananas - amount of bananas the client is leaving with
  /// @param schweppes - amount of schweppes the client is leaving with
  void pushClientInLog(Client* client, int min, int bananas, int schweppes);

  /// @brief Send a worker for the given resource
  /// @param minute - minute to send the worker at
  /// @param rt - respurce to send the worker for
  void sendWorker(int minute, const ResourceType rt);

  /// @brief Worker comes back with resource
  /// @param minute - minute at which the worker returns
  /// @param rt - resource which the worker brings
  void onReturn(int minute, const ResourceType rt);

  /// @brief Client departs and is removed from waiting lists
  /// @param client - client to remove
  void popClient(Client* client);


  // ---------------- Store ---------------- 

  /// @brief Free the memory
  void emptyClientsVectors();

  /// @brief Increment bananas with 100
  void incrementBananas();

  /// @brief Increment schweppes with 100
  void incrementSchweppes();

  /// @brief Decrease bananas quantity with given amount
  /// @param bananas - amount to decrease with
  void decreaseBananas(const int bananas);

  /// Decrease schweppes quantity with given amount
  /// @param schweppes - amount to decrease with
  void decreaseSchweppes(const int schweppes);

  /// @brief Sorts the vector of waiting clients by their max departure minute
  /// @param byDeparture - vector to sort
  void sortByDepartureMinute(std::vector<Client*>& byDeparture);
};


/// @brief Creates a new MyStore
/// @return new MyStore
MyStore* createMyStore();

/// @brief Reads information about the store from the console
/// @param store - the store to read information about
void readStore(MyStore* store);

/// @brief  Returns a string equal to the ResourceType
/// @param rt - resource
std::string getResourceToString(const ResourceType &rt);

/// @brief Checks if input is valid, if not - throws an exception
void checkInput();

/// @brief Reads information about a client from the console
/// @return client with such information
Client* readClient();

 /// @brief Copies vector of clients to another vector of clients
 /// @param source - the source vector
 /// @param dest - the destination vector
void copyClients(std::vector<Client>  source, std::vector< Client>& dest);