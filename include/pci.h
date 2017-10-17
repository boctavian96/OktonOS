#ifndef __PCI_H
#define __PCI_H

#include <port.h>
#include <driver.h>
#include <types.h>
#include <interrupts.h>

class PeripheralComponentInterconnectDeviceDescriptor
{
public:
    
    uint32_t portBase;
    uint32_t interrupt;
    
    uint16_t bus;
    uint16_t device;
    uint16_t function;
    
    unit16_t vendor_id;
    uint16_t device_id;
    
    uint8_t class_id; 
    uint8_t subclass_id;
    uint8_t interface_id;
    
    uint8_t revision;
    
    //Constructor
    PeripheralComponentInterconnectDeviceDescriptor();
    //Destructor
    ~PeripheralComponentInterconnectDeviceDescriptor();
    
};

class PeripheralComponentInterconnectController
{
    Port32Bit dataPort;
    Port32Bit commandPort;
    
public:
    //Ctor
    PeripheralComponentInterconnectController();
    //Dtor
    ~PeripheralComponentInterconnectController();
    
    uint32_t Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
    void Write(uint16_t bus, unit16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);
    bool DeviceHasFunctions(uint16_t bus, uint16_t device);
    
    void SelectDrivers(DriverManager* DriverManager);
    PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
    
};