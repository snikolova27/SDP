#include "interface.h"

struct MyStore : Store 
{
  int time = 0;
  int bananas = 0;
  int schweppes = 0;
  int workers = 0;
  int clientCnt = 0;
  std::vector<const Client *> clients;
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

  virtual int getBanana() const override ;

  virtual int getSchweppes() const override;
};

MyStore* createMyStore();

/// Reads information about the store
void readStore(MyStore* store);