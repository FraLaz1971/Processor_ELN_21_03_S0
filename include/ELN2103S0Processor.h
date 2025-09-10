#ifndef ELN2103S0Processor_H_HEADER_INCLUDED_C3772E73
#define ELN2103S0Processor_H_HEADER_INCLUDED_C3772E73
#include "Processor.h"
#include "ProcessorDefinition.h"

typedef  struct
{
  Packet* p;
} BLOCK;



//##ModelId=3C8892D203D6
//##Documentation
class ELN2103S0Processor : public Processor
{
    public:

        ELN2103S0Processor();

        bool loadConfiguration(int argc, char** argv) throw(PacketException*);

				//##Documentation
        //## Gets a string that describes the version of current processor
        virtual char* processorVersion() { return "0.0.1"; };

				//##Documentation
        //## Gets a string that describes the version of current processor
        virtual char* processorAuthor() { return "Francesco Lazzarotto"; };

        virtual char* processorDescriptor() { return "BC SRN ELN 21.03 S0 Processor (INAF/OAPD)";};

	virtual char* processorID() { return "ELN2103S0"; };

    protected:

        virtual void createMemoryStructure();

        virtual bool setValue();

        /** Inizializza un array di stringhe che contiene i valori da passare all'inizializzazione del file FITS */
        virtual char** initCharValueForOutput_init();

        /** Inizializza un array di interi che contiene i valori da passare all'inizializzazione del file FITS */
        virtual int* initIntValueForOutput_init();

        /** Inizializza un array di stringhe che contiene i valori da passare alla chiusura del file FITS */
        virtual char** initCharValueForOutput_close();

        /** Inizializza un array di interi che contiene i valori da passare alla chiusura del file FITS */
        virtual int* initIntValueForOutput_close();        

    private:
        int apid;

        int century;


};
#endif                           /* ELN2103S0Processor_H_HEADER_INCLUDED_C3772E73 */
