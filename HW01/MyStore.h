#include "interface.h"

/// Will be used for the log
struct StoreEvent {
  enum Type { WorkerSend, WorkerBack, ClientDepart };

  Type type;
  int minute;

  struct Worker {
    ResourceType resource;
  } worker;

  struct ClientEvent {
    int index;
    int banana;
    int schweppes;
  } client;
};

struct MyStore : Store 
{
  int time = 0;
  int bananas = 0;
  int schweppes = 0;
  int workers = 0;
  int clientCnt = 0;
  std::vector<const Client *> clients;

  /// Vector of waiting clients sorted by their departure time, will put -1 if a client has been catered to, so we don't have to erase from the vector
  std::vector <const Client*> waitingClientsByDeparture; 
  /// Will hold the index of the first client that needs to be catered to by departure, will change this index instead of erasing from the vector
  int currentFirstInLineDeparture = 0;
  /// Vector of waiting clients sorted by their arrival minute, will put -1 if a client has been catered to, so we don't have to erase from the vector
  std::vector< const Client*> waitingClientsByArrival;
  /// Will hold the index of the first client that needs to be catered to by arrival, will change this index instead of erasing from the vector
  int currentFirstInLineArrival = 0;


  std::vector<StoreEvent> log;

  ///структура за чакащи

  ActionHandler *actionHandler = nullptr;


  MyStore();
  MyStore(int workers, int banana, int schweppes);


  void setActionHandler(ActionHandler *handler) override ;

  void init(int workerCount, int startBanana, int startSchweppes) override;

  /// Adds a single client to the store
  void addClient(const Client *client);

  /// Adds more than one client to the store
  void addClients(const Client *clients, int count) override;

  void advanceTo(int minute) override;

  /// Return the index of a client in the store
  int getClientId(const Client *client);

 /// Returns current banana quantity in the store
  virtual int getBanana() const override ;

/// Returns current schweppes quantity in the store
  virtual int getSchweppes() const override;

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
void printLog(const std::vector<StoreEvent *> &log);
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
Client readClient();

 ///Copies vector of clients to another vector of clients
void copyClients(std::vector<const Client*>  source, std::vector< const Client*>& dest);
