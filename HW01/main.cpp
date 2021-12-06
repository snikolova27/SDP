#include "interface.h"
#include "MyStore.h"

int main()
{
 MyStore* store = createMyStore();
readStore(store);
std::cout << store->clients[0]->arriveMinute;
 delete store;
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