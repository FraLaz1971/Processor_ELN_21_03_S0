#include "ELN2103S0Processor.h"
#include "ELN2103S0_FITS.h"


ELN2103S0Processor::ELN2103S0Processor()
{
    //IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    addOutputFileProcessor(new ELN2103S0_FITS());
    apid=0;
    century = 0;
}




bool ELN2103S0Processor::loadConfiguration(int argc, char** argv) throw(PacketException*)
{
    //don't forget to call this method
    bool ret = Processor::loadConfiguration(argc, argv);
    //add extra code here for the extra parameters
    return ret;
}




void ELN2103S0Processor::createMemoryStructure()
{
    //1) creare le strutture di memoria necessarie

    BLOCK* block = (BLOCK*) new BLOCK;

    //2) importante, da non dimenticare (prima dell'invocazione di setValue()
    arrayDataOutput = (void*) block;
}




bool ELN2103S0Processor::setValue()
{
    double   time_tag;

    word nblocks; // Numero reale di event data blocks

    BLOCK* block = (BLOCK*) arrayDataOutput;
    
    block->p = p;
    century = p->dataField->dataFieldHeader->getFieldValue(7);
    

    //important
    tot_nrows++;
    //IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    nrowsFITS = tot_nrows;
    
    return true;
}




char** ELN2103S0Processor::initCharValueForOutput_init()
{
    char** c;

    c = new char* [1];
    c[1] = 0;
    return c;
}




int* ELN2103S0Processor::initIntValueForOutput_init()
{
    int* i;
    i = new int[1];
    i[0] = 0;
    return i;
}




char** ELN2103S0Processor::initCharValueForOutput_close()
{
    char** c;

    c = new char* [1];
    c[2] = 0;
    return c;
}




int* ELN2103S0Processor::initIntValueForOutput_close()
{
    int* i;

    i = new int[4];
    i[0] = tot_nrows;
    i[1] = tot_nrows_bad;
    i[2] = century;
    i[3] = 0;
    return i;
}
