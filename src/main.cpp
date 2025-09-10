/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Wed Sep  10 11:43:52 CET 2025
    copyright            : (C) 2025 by Francesco Lazzarotto
    email                : francesco.lazzarotto@inaf.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>
#include "ProviderDISCOS.h"
#include "ProviderSingleInput.h"
#include "ELN2103S0Processor.h"
#include "ELN2103S0_FITS.h"
#include "common.h"
#include "MonitorDummy.h"
#include "PacketExceptionIO.h"
using namespace std;
clock_t start;
clock_t tend;
time_t timevar1;
time_t  timevar2;

char* c1;

void report()
{
    //memory
		char* c2 = new char[1];
		printf("heap peak: %p\n", c2-c1);
    printf("Time0: %6.4f\n", (double)(clock() - (double)start) / CLOCKS_PER_SEC);
    //cout << "Time1: " << (double)(clock() - (double)start) / CLOCKS_PER_SEC << endl;
    time(&timevar2);
    cout << "Time: " << timevar2-timevar1 << endl;
    cout << "Number of ByteStream internal created: " << ByteStream::count_object2 << " mem allocated: " << ByteStream::count_object2 * sizeof(ByteStream) << endl;
    cout << "Number of ByteStream internal deleted: " << ByteStream::count_object_deleted2 << " mem free: " << ByteStream::count_object_deleted2 * (long)sizeof(ByteStream) << endl;

    cout << "Number of ByteStream external created (memory allocated): " << ByteStream::count_object << " mem allocated: " << ByteStream::count_object * sizeof(ByteStream) << endl;
    cout << "Number of ByteStream external deleted (memory allocated): " << ByteStream::count_object_deleted << " mem free: " << (long)(ByteStream::count_object_deleted * (long)sizeof(ByteStream)) << endl;

    cout << "Number of byte read by File class: " << File::byte_read <<  endl;
    cout << "Number of char read by File class: " << File::char_read <<  endl;
}


int main(int argc, char *argv[])
{
    try
    {
	c1 = new char[1];
        struct tm* tm_int;
        time_t timevar1;
        time_t timevar2;
        time(&timevar1);
        start = clock();
        ELN2103S0Processor* gp = (ELN2103S0Processor*) new ELN2103S0Processor();
        bool ret = gp->loadConfiguration(argc, argv);
        cout << "ret = "<< ret << endl;
        if(ret) gp->startMeasurementSession();
        /*ELN2103S0_FITS fits;
        fits.init(0,0,0);
        fits.close(0,0);*/


        time(&timevar2);
        //cout << "Time: " << timevar2-timevar1 << endl;
        //cout << "Media: " << gp->getTot_nrows() /   ((timevar2-timevar1)?(timevar2-timevar1):1) << endl;
        tend = clock();
	report();
        cout << "END" << endl;
        return 0;

    }
    catch(PacketExceptionIO* e)
    {
        cout << e->geterror();
    }
    catch(PacketException* e)
    {
        cout << e->geterror();
    }
}
