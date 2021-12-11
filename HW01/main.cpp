#include "MyStore.h"
#include "interface.h"
#include <iostream>

int main()
{
    MyStore* store = nullptr;
    try 
    {
     store = createMyStore();
    }
     catch (...)
    {
    throw;
    }

    store->init(3,15,15);
    readStore(store);

    store->advanceTo(INT32_MAX);
    store->printLog();
    store->closeStore();
    delete store;

}