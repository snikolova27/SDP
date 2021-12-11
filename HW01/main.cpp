#include "MyStore.h"
#include "interface.h"
#include <iostream>

int main()
{
 MyStore* store = createMyStore();
 store->init(5,10,0);
 std::cout << store->workers << std::endl;
 Client cl [] = {Client(0, 10, 0, 10),
		Client(45, 35, 0, 30),
		Client(46, 30, 20, 100),
		Client(200, 10, 10, 1)};
//store->addClients(cl, 4);
//std::cout << store->waitingClientsByArrival.size() << std::endl;
//std::cout << store->requestedBananas(store->waitingClientsByDeparture[0]) << std::endl;


Client* cl1 =  new Client (0,0,10,0);
 Client* cl2 =   new Client (1,20,0,5);
 store->addClient(cl1);
 store->addClient(cl2);

//store->printClients();
store->advanceTo(6);//
store->printLog();
//readStore(store);
// Client clients [2];
//store->workers = 2;

// clients[0] = cl1;
// clients[1] = cl2;

Client cl3 = Client(0,10,10,75);
Client cl4 = Client(2,12,120,70);
// store->fillWaiting();
// store->sortWaiting();
//store->addClients(clients, 2);
//store->addClient(cl3);
//store->addClient(cl4);
//std::cout << store->clients[0].schweppes << std::endl;
//std::cout << store->waitingClientsByDeparture[0].schweppes << std::endl;
//std::cout << store->waitingClientsByDeparture[1].arriveMinute << " " << store->waitingClientsByDeparture[1].schweppes << std::endl;



//  std::cout << store->waitingClientsByArrival[store->currentFirstInLineArrival]->maxDepartTime << std::endl;
//  std::cout << store->waitingClientsByDeparture[0]->arriveMinute << std::endl;
//  std::cout << store->waitingClientsByDeparture[store->currentFirstInLineDeparture]->maxDepartTime << std::endl;

//store->sendWorker(0, ResourceType::banana);
//store->sendWorker(2 ResourceType::banana); //worker would not be send if the clients are
                                            /* 0 10 10 75
                                               2 12 12 70 ->trugna  за бананите

                                               should send worker if
                                               2 98 98 75
                                               3 25 25 100 ->trugna за бананите

                                               test za onReturn, pak samo za bananite -> 2 rabotnika za banani, ama ne sum pusnala da hodi za schweppes
                                               0 10 10 75
                                               2 98 98 75   -> BACHKAME ZA DVA BANANA
                                                            -> BACHKAME ZA BANAN + SCHWEPPES


                                            */
// store->sendWorker(0, ResourceType::schweppes);
// store->onReturn(60, ResourceType::banana); 
// store->onReturn(62, ResourceType:: schweppes);
// store->popClient(60, &cl1);

                                            
//std::cout << std::boolalpha<< store->areTheFirstsSame() << std::endl;
//store->printLog();
store->emptyClientsVectors();


//store->incrementBananas();
//std::cout << store->bananas;
 delete store;
 delete cl1;
 delete cl2;


}
 
/* start 15 15
    0 20 25 60
    10 15 15 45
    60 80 80 

    start 0 0 
    0 10 10 65
    15 10 10 45
    D 60 banana
    D 60 schweps

    start 50 50
    0 105 105 70
    10 100 100 60
    15 50 50  0

    W 0 bananas
    W 0 schewps
    W 10 ban
    W 10 sch    
    2 15 50 50 
    D 60 banana
    D 60 schweps
    1 60 100 100 
    D 70 banana
    D 70 schweps
    0 70 100 100 

    приоритет продажби ?? чакащ или идващ

    start 0 0
    0 50 50 70
    10 50 50 50
    60 50 50 0
    60 50 50 50
                KAK???? LYOBO???
    W 0 bananas
    W 0 scheps
    D 60 banans
    D 60 schewps


*/
/*
5
350 100 
banana
250 100
banana
150 100
banana
50 100
schwepppes
50 0
bananna


*/