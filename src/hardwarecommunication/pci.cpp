#include <hardwarecommunication/pci.h>
#include <drivers/amd_am79c973.h>
#include <kprintf.h>

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;




PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor()
{
}

PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor()
{
}







PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
: dataPort(0xCFC),
  commandPort(0xCF8)
{
}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController()
{
}

uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.Write(id);
    uint32_t result = dataPort.Read();
    return result >> (8* (registeroffset % 4));
}

void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.Write(id);
    dataPort.Write(value); 
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(common::uint16_t bus, common::uint16_t device)
{
    return Read(bus, device, 0, 0x0E) & (1<<7);
}

void PeripheralComponentInterconnectController::SelectDrivers(DriverManager* driverManager, myos::hardwarecommunication::InterruptManager* interrupts)
{
    for(int bus = 0; bus < 8; bus++)
    {
        for(int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for(int function = 0; function < numFunctions; function++)
            {
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);
                
                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;
                
                
                for(int barNum = 0; barNum < 6; barNum++)
                {
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if(bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address;
                }
                
                Driver* driver = GetDriver(dev, interrupts);
                if(driver != 0)
                    driverManager->AddDriver(driver);

                
                kprintf("PCI BUS ");
                kprintfHex(bus & 0xFF);
                
                kprintf(", DEVICE ");
                kprintfHex(device & 0xFF);

                kprintf(", FUNCTION ");
                kprintfHex(function & 0xFF);
                
                kprintf(" = VENDOR ");
                kprintfHex((dev.vendor_id & 0xFF00) >> 8);
                kprintfHex(dev.vendor_id & 0xFF);
                kprintf(", DEVICE ");
                kprintfHex((dev.device_id & 0xFF00) >> 8);
                kprintfHex(dev.device_id & 0xFF);
                kprintf("\n");
            }
        }
    }
}


BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
    BaseAddressRegister result;
    
    
    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4*headertype);
    if(bar >= maxBARs)
        return result;
    
    
    uint32_t bar_value = Read(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;
    
    
    
    if(result.type == MemoryMapping)
    {
        
        switch((bar_value >> 1) & 0x3)
        {
            
            case 0: // 32 Bit Mode
            case 1: // 20 Bit Mode
            case 2: // 64 Bit Mode
                break;
        }
        
    }
    else // InputOutput
    {
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }
    
    
    return result;
}



Driver* PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, InterruptManager* interrupts)
{
    Driver* driver = 0;
    switch(dev.vendor_id)
    {
        case 0x1022: // AMD
            switch(dev.device_id)
            {
                case 0x2000: // am79c973
                    kprintf("AMD am79c973 ");
                    driver = (amd_am79c973*)MemoryManager::activeMemoryManager->malloc(sizeof(amd_am79c973));
                    if(driver != 0)
                        new (driver) amd_am79c973(&dev, interrupts);
                    else
                        kprintf("instantiation failed");
                    return driver;
                    break;
            }
            break;

        case 0x8086: // Intel
            break;
    }
    
    
    switch(dev.class_id)
    {
        case 0x03: // graphics
            switch(dev.subclass_id)
            {
                case 0x00: // VGA
                    kprintf("VGA ");
                    break;
            }
            break;
    }
    
    
    return driver;
}



PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PeripheralComponentInterconnectDeviceDescriptor result;
    
    result.bus = bus;
    result.device = device;
    result.function = function;
    
    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0b);
    result.subclass_id = Read(bus, device, function, 0x0a);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3c);
    
    return result;
}







