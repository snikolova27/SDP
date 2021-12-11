#include <iterator>
#define CATCH_CONFIG_MAIN

#include "interface.h"
#include "MyStore.h"
#include "catch.hpp"

#include <vector>

const int RESTOCK_AMOUNT = 100;

typedef std::vector<Client> ClientList;

struct TestStore : ActionHandler 
{
	Store *impl = nullptr;
	std::vector<StoreEvent> log;

	TestStore()
		: impl(createStore()) {
		impl->setActionHandler(this);
	}

	~TestStore()
	{
		delete impl;
	}

	TestStore(const TestStore &) = delete;
	TestStore &operator=(const TestStore &) = delete;

	void init(int workerCount, int startBanana, int startSchweppes)
	{
		impl->init(workerCount, startBanana, startSchweppes);
	}

	void advanceTo(int minute)
	{
		impl->advanceTo(minute);
	}

	int getBanana() const
	{
		return impl->getBanana();
	}

	int getSchweppes() const 
	{
		return impl->getSchweppes();
	}

	void addClients(const ClientList &clients)
	{
		impl->addClients(clients.data(), clients.size());
	}

	void addClients(const Client &single) 
	{
		impl->addClients(&single, 1);
	}

	void onWorkerSend(int minute, ResourceType resource) override 
	{
		StoreEvent ev;
		ev.type = StoreEvent::WorkerSend;
		ev.minute = minute;
		ev.worker.resource = resource;
		log.push_back(ev);
	}

	void onWorkerBack(int minute, ResourceType resource) override 
	{
		StoreEvent ev;
		ev.type = StoreEvent::WorkerBack;
		ev.minute = minute;
		ev.worker.resource = resource;
		log.push_back(ev);
	}

	void onClientDepart(int index, int minute, int banana, int schweppes) override 
	{
		StoreEvent ev;
		ev.type = StoreEvent::ClientDepart;
		ev.minute = minute;
		ev.client.banana = banana;
		ev.client.schweppes = schweppes;
		ev.client.index = index;
		log.push_back(ev);
	}
};

#define LastEvent() (store.log.back())

TEST_CASE("Worker shouldn't be sent for second customer")
{
	TestStore store;
	store.init(4, 0, 0);

    store.addClients(Client{1,10,10,75});
    store.addClients(Client{2,12,12,70});

	SECTION("Advance before depart time") 
	{
		store.advanceTo(0);
		INFO("Must not generate event before time is advanced to its time");
		REQUIRE(store.log.size() == 0);
	}

    SECTION("Send first workers")
    {
        store.advanceTo(1);
        INFO("Must send 2 workers - one for bananas, one for schweppes");
        REQUIRE(store.log.size() == 2);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
        REQUIRE(store.getBanana() == 0);
    }
	SECTION("Workers come back") 
	{
        INFO("Both workers should return and both clients should depart");
        store.advanceTo(61);
        REQUIRE(store.getBanana() == 78);
        REQUIRE(store.getSchweppes() == 78);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().minute == 61);
		REQUIRE(LastEvent().client.banana == 12);
		REQUIRE(LastEvent().client.schweppes == 12);
		REQUIRE(store.log.size() == 6);
	}

	SECTION("No workers should be sent for the request of the second client")
	{
		INFO("Incoming resources will be enough for both clients");
		store.advanceTo(2);
		REQUIRE(store.log.size() == 2);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
	}

    /*
     W 1 banana
     W 1 schweppes
     D 61 banana
     D 61 schweppes
     0 61 10 10
     1 61 12 12
    */
}

TEST_CASE("Should send worker if incoming resource won't be enough")
{
    TestStore store;
    store.init(2,0,0);

    store.addClients(Client{2,98,98,75});
    store.addClients(Client{3,25,25,100});

    SECTION("Send 2 workers for the request of the first client")
    {
        store.advanceTo(2);
        REQUIRE(store.log.size() == 2);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
        REQUIRE(store.getBanana() == 0);
        REQUIRE(store.getSchweppes() == 0);
    }
    SECTION("Cannot send 2 more workers for the request of the second client")
    {
        store.advanceTo(3);
        REQUIRE(store.log.size() == 2);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
        REQUIRE(store.getBanana() == 0);
        REQUIRE(store.getSchweppes() == 0);
    }
    SECTION("First 2 workers return and client one should be served and depart")
    {
        store.advanceTo(62);
        REQUIRE(store.log.size() == 5);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().minute == 62);
		REQUIRE(LastEvent().client.banana == 98);
		REQUIRE(LastEvent().client.schweppes == 98);
        REQUIRE(store.getBanana() == 2);
        REQUIRE(store.getSchweppes() == 2);
    }
    SECTION("Advance before second's client wait time has come ")
    {
        INFO("No new events should be generated.");
        store.advanceTo(100);
        REQUIRE(store.log.size() == 5);
        REQUIRE(store.getBanana() == 2);
        REQUIRE(store.getSchweppes() == 2);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);

    }
    SECTION("Give what's left in the store to second client")
    {
        store.advanceTo(105);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().minute == 103);
		REQUIRE(LastEvent().client.banana == 2);
		REQUIRE(LastEvent().client.schweppes == 2);
        REQUIRE(store.getBanana() == 0);
        REQUIRE(store.getSchweppes() == 0);
        
    }

    /*
     W 2 banana
     W 2 schweppes
     D 62 banana
     D 62 schweppes
     0 62 98 98
     1 103 2 2
    */
}

TEST_CASE("Client cannot wait for the worker to return")
{
    TestStore store;
    store.init(3,15,15);
    store.addClients(Client{0,20,25,60});
    store.addClients(Client{10,15,15,45});
    store.addClients(Client{60,80,80,2});

    SECTION("Send 2 workers for first client")
    {
        store.advanceTo(0);
        REQUIRE(store.log.size() == 2);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
    }
    SECTION("Don't send workers for the secind client")
    {
        store.advanceTo(10);
        REQUIRE(store.log.size() == 3);
        REQUIRE(store.getBanana() == 0);
        REQUIRE(store.getSchweppes() == 0);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
    }
    SECTION("Send worker for the request of the third client")
    {
        INFO("First client will leave");
        store.advanceTo(60);
        REQUIRE(store.log.size() == 6);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 0);
        REQUIRE(LastEvent().client.banana == 20);
        REQUIRE(LastEvent().client.schweppes == 25);
        REQUIRE(store.getBanana() == 80);
        REQUIRE(store.getSchweppes() == 75);

    }

    SECTION("Third client cannot wait for the worker to return")
    {
        store.advanceTo(62);
        REQUIRE(store.log.size() == 7);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 2);
        REQUIRE(LastEvent().client.banana == 80);
        REQUIRE(LastEvent().client.schweppes == 75);
        REQUIRE(store.getBanana() == 0);
        REQUIRE(store.getSchweppes() == 0);

    }
    SECTION("No more events should be generated")
    {
        store.advanceTo(122);
        REQUIRE(store.log.size() == 7);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(store.getSchweppes() == 0);
    }
    /*
    W 0 schweppes
    W 0 banana
    1 10 15 15
    D 60 schweppes
    D 60 banana
    0 60 20 25
    2 62 80 75
    */
}

TEST_CASE("Тест на Петър Колев - опит да счупим програмата")
{
    TestStore store;
    store.init(3,0,0);
    store.addClients(Client{0,120,30,100});
    store.addClients(Client{40,20,60,100});
    store.addClients(Client{80,70,45,70});
    
    SECTION("Send 3 workers when first client arrives")
    {
        store.advanceTo(0);
        REQUIRE(store.log.size() == 3);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
    }

    SECTION("Don't send workers for second client")
    {
        store.advanceTo(40);
        REQUIRE(store.log.size() == 3);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
    }

    SECTION("First and second client depart")
    {
        INFO("Workers have returned and brought back resources");
        store.advanceTo(60);
        REQUIRE(store.log.size() == 8);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 1);
        REQUIRE(LastEvent().client.banana == 20);
        REQUIRE(LastEvent().client.schweppes == 60);
        REQUIRE(store.getBanana() == 60);
        REQUIRE(store.getSchweppes() == 10);
        
    }

    SECTION("Send worker for third client")
    {
        store.advanceTo(80);
        REQUIRE(store.log.size() == 10);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
    }

    SECTION("Advance to final minute")
    {
        store.advanceTo(140);
        REQUIRE(store.log.size() == 13);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 2);
        REQUIRE(LastEvent().client.banana == 70);
        REQUIRE(LastEvent().client.schweppes == 45); 
        
    }
    /*
    W 0 banana
    W 0 schweppes
    W 0 banana
    D 60 banana
    D 60 schweppes
    D 60 banana
    0 60 120 30
    1 60 20 60
    W 80 schweppes
    W 80 banana
    D 140 schweppes
    D 140 banana
    2 140 70 45
    */
}

/*
TEST_CASE("Тест на Иван Стефанов от дискорд")
{
    TestStore store;
    store.init(4,0,0);
    store.addClients(Client{0,350,200,60});

    SECTION("Send 4 workers at minute 0")
    {
        store.advanceTo(0);
        REQUIRE(store.log.size() == 4);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
        REQUIRE(store.getBanana() == 0);
        REQUIRE(store.getSchweppes() == 0);    
    }

    SECTION("Workers return and serve client")
    {
        store.advanceTo(60);
        REQUIRE(store.log.size() == 9);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 0 );
        REQUIRE(LastEvent().client.banana == 300);
        REQUIRE(LastEvent().client.schweppes == 100);
    }

    
    W 0 banana
    W 0 banana
    W 0 schweppes
    W 0 banana
    D 60 banana
    D 60 banana
    D 60 schweppes
    D 60 banana
    0 60 300 100

    
}
*/


TEST_CASE("Тест на Мартин Костов от Дискорд")
{
    TestStore store;
    store.init(1,0,0);
    store.addClients(Client{0,1,0,0});
    store.addClients(Client(30,101,0,31));
    store.addClients(Client(59,2,0,2));

    SECTION("Send worker and first client departs")
    {
        store.advanceTo(0);
        REQUIRE(store.log.size() == 2);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 0);
        REQUIRE(LastEvent().client.banana == 0);
        REQUIRE(LastEvent().client.schweppes == 0);
    }
    SECTION("Worker comes back and third client leaves")
    {
        store.advanceTo(60);
        REQUIRE(store.log.size() == 4);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 2);
        REQUIRE(LastEvent().client.banana == 2);
        REQUIRE(LastEvent().client.schweppes == 0);
        REQUIRE(store.getBanana() == 98);
        REQUIRE(store.getSchweppes() == 0);
    }
    SECTION("Last client to leave is the second one")
    {
        store.advanceTo(61);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 1);
        REQUIRE(LastEvent().client.banana == 98);
        REQUIRE(LastEvent().client.schweppes == 0);
        REQUIRE(store.log.size() == 5);

    }
    /*
    
1  3
0 1 0 0
30 101 0 31
59 2 0 2

W 0 banana
0 0 0 0
D 60 banana
2 60 2 0
1 61 98 0
    */
}

TEST_CASE ("Priority check")
{
    TestStore store;
    store.init(4,50,50);
    store.addClients(Client{0,105,105,70});
    store.addClients(Client{10,100,100,60});
    store.addClients(Client{15,50,50,0});

    SECTION("Send 2 workers for first client")
    {
        store.advanceTo(0);
        REQUIRE(store.log.size() == 2);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
        REQUIRE(store.getBanana() == 50);
    }
    SECTION("Won't send 2 more workers for second client")
    {
        store.advanceTo(10);
        REQUIRE(store.log.size() == 2);
        REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
        REQUIRE(store.getBanana() == 50);
    }
    SECTION("Serve third client")
    {
        store.advanceTo(15);
        REQUIRE(store.log.size() == 3);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 2);
        REQUIRE(store.getBanana() == 0);
        REQUIRE(store.getSchweppes() == 0);
    }
    SECTION("First workers comes back and serve second client")
    {
        store.advanceTo(60);
        REQUIRE(store.log.size() == 6);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 1);
        REQUIRE(LastEvent().client.banana == 100);
        REQUIRE(LastEvent().client.schweppes == 100);
        REQUIRE(store.getBanana() == 0);
    }
    SECTION("Last workers come back and first client is served")
    {
        store.advanceTo(70);
        REQUIRE(store.log.size() == 7);
        REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
        REQUIRE(LastEvent().client.index == 0);
        REQUIRE(LastEvent().client.banana == 0);
        REQUIRE(LastEvent().client.schweppes == 0);
        REQUIRE(store.getBanana() == 0);  
    }
    /*
       start 50 50
    0 105 105 70
    10 100 100 60
    15 50 50  0

    W 0 bananas
    W 0 schewps  
    2 15 50 50    0 0
    D 60 banana 100
    D 60 schweps 100
    1 60 100 100 0 0
    0 70 0 0 
    */
}