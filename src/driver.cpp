#include "driver.h"
#include "types.h"

//TO DO

Driver::Driver()
{
    
}

Driver::~Driver()
{
}

void Driver::Activate()
{

}

int32_t Driver::Reset()
{
    return 0;
}

void Driver::Deactivate()
{
    
}


//Driver Manager
DriverManager::DriverManager()
{
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver* d)
{
    drivers[numDrivers] = d;
    numDrivers++;
}

void DriverManager::ActivateAll()
{
    for (int i = 0; i < numDrivers; i++)
    {
        drivers[i]->Activate();
    }
}

