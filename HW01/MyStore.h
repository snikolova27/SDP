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
  int clientInLog = 0;

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


  virtual void setActionHandler(ActionHandler *handler) override ;

  virtual void init(int workerCount, int startBanana, int startSchweppes) override;

  virtual void advanceTo(int minute) override;

  /// Adds a single client to the store
  void addClient( Client* client);

  /// Adds more than one client to the store
  void addClients(const Client *clients, int count) override;

  /// Return the index of a client in the store, v - vector to search in
  int getClientId(Client* client, std::vector<Client*> v);

///Return the index of the first non-popped client from a vector v
  int findFirstNotPopped(std::vector<Client*>& v);

///Returns if all clients in a vector v are popped == have left
  bool areAllPopped(std::vector <Client*> v);

 /// Returns current banana quantity in the store
  virtual int getBanana() const override ;

/// Returns current schweppes quantity in the store
  virtual int getSchweppes() const override;


void printClients();

/// Fills the waitingClients vectors
void fillWaiting();

/// Find minimum of waitingByArrival, waitingByDeparture, returningWorkers so we know what minute to advance to
  int findMin( std::vector< Client*>& byDeparture, LinkedQueue<Worker> &workers);

/// Sorts the list of waiting clients by their arrival minute
  void sortByArrivalMinute(std::vector<Client*>& byArrival);

/// Sorts the list of waiting clients by their max departure minute
  void sortByDepartureMinute(std::vector<Client*>& byDeparture);

/// Sort the waiting lists
  void sortWaiting();

/// Sorts the log by minute of event and i
/// Uses a lambda function for the sorting
  void sortByMin(std::vector<StoreEvent > &log);

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
  int requestedBananas( Client* client);

/// Check how much Schweppes a client wants, returns -1 if they don't want any 
  int requestedSchweppes( Client* client);

/// Checks if there are any workers available
  bool isWorkerAvailable();

/// Check if the first by arival and the first by departute are the same
  bool areTheFirstsSame();

///Find which client arrived first
 Client* findFirstOfTwo(Client* first,  Client* second);

/// Returns which resource a client wants  - returns 0 for bananas, 1 for schweppes, 2 for both, -1 if not valid
int whichResource( Client* client);

/// Send a worker for the given resource
  void sendWorker(int minute, const ResourceType rt);

/// Worker comes back
  void onReturn(int minute, const ResourceType rt);

/// Remove a client from waiting lists
  void popClient(int minute, Client* client);

 /// Resource with priotity
ResourceType higherPriotity(Client* client);

/// Return if there are any workers left in the store
bool workersLeft();

/// Returns the first client to be served
Client* firstToServe();

///Checks if there is already a worker sent that can satisfy the request of the first client to be served
  bool isWorkerSent(const ResourceType rt);

/// Send worker/s for a request,return minute when resource will be enough for the given client
/// @param client - first client to be served
int doRequest( Client* client);

/// Put client in log
void pushClientInLog(Client* client, int min, int bananas, int schweppes);

/// Serve bananas to a client, return end minute
int doRequestBananas( Client* client);

/// Serve schweppes to a client, return end minute
int doRequestSchweppes(Client* client);

/// Serves a client
void serve(Client* client);



// TODO
  //funcs
  //TODO fix close store function
//TODOMAKE SOME FUNCTIONS PRIVATE
//TODO overload opetaror == for StoreEvent

//TODO: sort the log




//TODO
/// Returns if event is already in log
bool isInLog(const StoreEvent ev);

//TODO
/// Return the minute the request for bananas will be satisfied when sending workers for both resources
int doBananas (Client* client, int& endBananas);

///
int doSchweppes(Client* client, int& endSchweppes);

/// Generate all events for the store up to a given minute
  void generateEvents( const int upToMin);
};


/// Done

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
void copyClients(std::vector<Client>  source, std::vector< Client>& dest);


