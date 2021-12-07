#pragma once

/// The type of resource, used when creating events
#include <ostream>
#include <string>
#include <vector>
#include <iostream>

enum class ResourceType {
  banana,
  schweppes,
};

/// These structs will be used for the log
 struct Worker {
    ResourceType resource;
    int comeBackTime;

    Worker(){}
    Worker(const ResourceType rt, const int min) : resource(rt), comeBackTime(min){};
  };

struct StoreEvent {
  enum Type { WorkerSend, WorkerBack, ClientDepart };

  Type type;
  int minute;
  Worker  worker;
  struct ClientEvent {
    int index;
    int banana;
    int schweppes;
  } client;
  
  //StoreEvent(){}
};

/// Interface used to gather all events your solution will generate
/// Instance of this class will be created by the tests and passed to your
/// implementation
struct ActionHandler {
  /// Callback for when store sends out a worker
  /// @param minute - the time at which the worker is sent out
  /// @param resource - the resource the worker will restock
  virtual void onWorkerSend(int minute, ResourceType resource) = 0;
  //само когато дойде клиент и няма продукти за него и не е изпратен друг
  //работник, който ще донесе достатъчно за него и добавяме в опашката за
  //работник-минута връщане

  /// Callback for when a previously sent out worker returns to the store
  /// @param minute - hte time at which the worker returns to the store
  /// @param resource - the type of resource the worker restocks
  virtual void onWorkerBack(int minute, ResourceType resource) = 0;
  //махаме работника от опашка работник-връщане, зареждаме банани и швепс,
  //обслужваме тези в структура чакащи клиенти, итерирайки по момент на
  //пристигане и махаме от структура за чакащи този клиент

  /// Callback for when client departs from the store for any reason
  /// @param index - the index of the client (0 - N, where N is the number of
  /// clients)
  /// @param minute - the time at which the client departs from the store
  /// @param banana - amount of bananas the client takes when departing
  /// @param schweppes - amount of schweppes the client takes when departing
  virtual void onClientDepart( int index, int minute, int banana,int schweppes) = 0; 
  //извеждаме съобщение + трием клиент от двете опашки
};

/// Descriptor of a client that arrives at the store
struct Client {
  int arriveMinute; ///< The time at which client arrives
  int banana;       ///< Amount of bananas the client wants to take
  int schweppes;    ///< Amount of schweppes the client wants to take
  int maxWaitTime;  ///< The max amount of time the client will wait before he departs
  int maxDepartTime; ///< The max minute at which the client will depart

  Client();
  Client(const int arrive, const int bananas, const int schweppes,const int time);
  friend bool operator ==(const Client& current, const Client& other);
};

/// The interface that your solution needs to implement
/// Your implementation must derive from this class, refer to the sample
/// implementation for example
struct Store {
  /// This method is called to initialize the store state
  /// For the console executable, starting resources are 0, but some tests will
  /// provide amounts more than 0
  /// @param workerCount - the number of workers this store has
  /// @parma startBanana - the amount of bananas the store starts with
  /// @parma startSchweppes - the amount of schweppes the store starts with
  virtual void init(int workerCount, int startBanana, int startSchweppes) = 0;

  /// This method will be called by the test to set the instance of the
  /// ActionHandler Your solution *MUST* call the methods of ActionHandler when
  /// event happens in the store Events in the store are: worker sent out,
  /// worker returns, client departs NOTE: For the console executable you can
  /// choose if you want to use ActionHandler at all
  /// @param handler - pointer to the instance of the handler, never null
  virtual void setActionHandler(ActionHandler *handler) = 0;

  /// Method used by the tests to add new clients to the store
  /// Indices of the clients must be incremented starting from 0
  /// NOTE: Tests may call this method multiple times, this must append new
  /// clients to existing clients in the store
  virtual void addClients(const Client *clients, int count) = 0;

  /// Advance the time to the given minute. All events up to and *including* the
  /// minute must be generated Events that happen at a later time must *not* be
  /// generated NOTE: Multiple calls to the function will always pass increasing
  /// values
  /// @param minute - the minute to advance the time to
  virtual void advanceTo(int minute) = 0;

  /// Get the amount of bananas currently in the store
  virtual int getBanana() const = 0;

  /// Get the amount of schweppes currently in the store
  virtual int getSchweppes() const = 0;

  virtual ~Store() {}
};

/// This function must create new instance of your store
/// This will be used by the tests to create a store to test
Store *createStore();

