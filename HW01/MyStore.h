#pragma once
#include "interface.h"
#include "linkedQueue.h"

const int INCREMENT_STOCK = 100;

struct MyStore : Store 
{
  int time = 0;
  int bananas = 0;
  int schweppes = 0;
  int workers = 0;    //will decrease if a worker is sent to bring something, will increase when worker comes back
  int clientCnt = 0;

  /// Vector of clients, won't be deleting clients from here
  std::vector<const Client*> clients;

  /// Vector of waiting clients sorted by their departure time, will put -1 if a client has been catered to, so we don't have to erase from the vector
  std::vector <const Client*> waitingClientsByDeparture; 

  /// Will hold the index of the first client that needs to be catered to by departure, will change this index instead of erasing from the vector
  int currentFirstInLineDeparture = 0;

  /// Vector of waiting clients sorted by their arrival minute, will put -1 if a client has been catered to, so we don't have to erase from the vector
  std::vector< const Client*> waitingClientsByArrival;

  /// Will hold the index of the first client that needs to be catered to by arrival, will change this index instead of erasing from the vector
  int currentFirstInLineArrival = 0;

  /// Will hold when the sent workers will be back and with what resourse
  LinkedQueue <Worker> workersBackTimes; 
  
  /// Will hold all the generated events in the store, sorted by minute
  std::vector<StoreEvent> log;

  ActionHandler *actionHandler = nullptr;


  MyStore();
  MyStore(int workers, int banana, int schweppes);


  virtual void setActionHandler(ActionHandler *handler) override ;

  virtual void init(int workerCount, int startBanana, int startSchweppes) override;

  virtual void advanceTo(int minute) override;

  /// Adds a single client to the store
  void addClient(const Client* client);

  /// Adds more than one client to the store
  void addClients(const Client *clients, int count) override;

  /// Return the index of a client in the store, v - vector to search in
  int getClientId(const Client *client, std::vector<const Client*> v);

 /// Returns current banana quantity in the store
  virtual int getBanana() const override ;

/// Returns current schweppes quantity in the store
  virtual int getSchweppes() const override;


void printClients();
/// Fills the waitingClients vectors
  void fillWaiting();

/// Find minimum of waitingByArrival, waitingByDeparture, returningWorkers so we know what minute to advance to
  int findMin(const std::vector< const Client*> byArrival,const std::vector< const Client*> byDeparture, const std::vector<int> workers);

/// Sorts the list of waiting clients by their arrival minute
  void sortByArrivalMinute(std::vector<const Client*>& byArrival);

/// Sorts the list of waiting clients by their max departure minute
  void sortByDepartureMinute(std::vector< const Client*>& byDeparture);

/// Sort the waiting lists
  void sortWaiting();

/// Sorts the log by minute of event and i
/// Uses a lambda function for the sorting
  void sortByMin(std::vector<StoreEvent *> &log);

/// Prints the log
  void printLog();

/// Free the memory
void emptyClientsVectors();

/// Clear the vectors of client
void closeStore();


///Checks if there are enough bananas available for the request
  bool areBananasEnough(const int & bananas);

///Checks if there is enough schweppes available for the request
  bool isSchweppesEnough(const int & schweppes);

/// Increment bananas
  void incrementBananas();

/// Increment schweppes
  void incrementSchweppes();

/// Decrease bananas quantity with given amount
  void decreaseBananas(const int bananas);

/// Decrease schweppes quantity with given amount
  void decreaseSchweppes(const int schweppes);

/// Check how many bananas a client wants, returns -1 if they don't want any 
  int requestedBananas(const Client* client);

/// Check how much Schweppes a client wants, returns -1 if they don't want any 
  int requestedSchweppes(const Client* client);

/// Checks if there are any workers available
  bool isWorkerAvailable();

/// Check if the first by arival and the first by departute are the same
  bool areTheFirstsSame();

///Find which client arrived first
const Client* findFirstOfTwo(const Client* first, const Client* second);

///Checks if there is already a worker sent that can satisfy the request
  bool isWorkerSent(const ResourceType rt);

/// Returns which resource a client wants  - returns 0 for bananas, 1 for schweppes, 2 for both, -1 if not valid
int whichResource(const Client* client);

/// Send worker/s for a request
void doRequest(const Client* client);

/// Send a worker for the given resource
  void sendWorker(int minute, const ResourceType rt);

//------ TODO ------
  // MAKE SOME FUNCTIONS PRIVATE


/// Worker comes back
  void onReturn(int minute, const ResourceType rt);

/// Remove a client from waiting lists
  void popClient(int minute, const Client* client);

/// Generate all events for the store
  void generateEvents();
};

/// Creates a new MyStore
MyStore* createMyStore();

/// Reads information about the store
void readStore(MyStore* store);

/// Returns a string equal to the ResourceType
std::string getResourceToString(const ResourceType &rt);

/// Checks if input is valid, if not - throws an exception
void checkInput();

/// Reads information about a client and return a client with such information
Client* readClient();


 ///Copies vector of clients to another vector of clients
void copyClients(std::vector<const Client*>  source, std::vector< const Client*>& dest);


