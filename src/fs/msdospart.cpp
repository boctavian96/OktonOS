#include <fs/msdospart.h>
#include <fs/fat.h>
#include <kprintf.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::fs;

void MSDOSPartitionTable::ReadPartitions(myos::drivers::AdvancedTechnologyAttachment *hd)
{
    
    MasterBootRecord mbr;
    
    kprintf("MBR: ");
    
    hd->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));

    /*
    for(int i = 0x1BE; i <= 0x01FF; i++)
    {
        kprintfHex(((uint8_t*)&mbr)[i]);
        kprintf(" ");
    }
    kprintf("\n");
    */
    
    if(mbr.magicnumber != 0xAA55)
    {
        kprintf("illegal MBR");
        return;
    }
    
    
    for(int i = 0; i < 4; i++)
    {
        if(mbr.primaryPartition[i].partition_id == 0x00)
            continue;
        
        kprintf(" Partition ");
        kprintfHex(i & 0xFF);
        
        
        if(mbr.primaryPartition[i].bootable == 0x80)
            kprintf(" bootable. Type");
        else
            kprintf(" not bootable. Type ");
        
        
        kprintfHex(mbr.primaryPartition[i].partition_id);
        
     
        ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
    }
    
}
