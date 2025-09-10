/***************************************************************************
                          ELN2103S0_FITS.h  -  description
                             -------------------
    begin                : Wed Sep 10 2025
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

#ifndef ELN2103S0_FITS_H
#define ELN2103S0_FITS_H

#include "FITSBinaryTable.h"
#include "ProcessorDefinition.h"

/**
 *@author Francesco Lazzarotto
 */

class ELN2103S0_FITS : public FITSBinaryTable
{
    public:
        ELN2103S0_FITS();
        ~ELN2103S0_FITS();
        virtual bool init(char**c, int*i, char* filenameconfig = 0) throw(PacketException*);

        virtual bool close(char**c, int* i);

        virtual bool writeData(void* data, dword nrows);

    private:

        void updateKey(char**c, int* i);

        int ncol1;
        int ncol2;
        char **ttype1;
	char **tform1;
	char **tunit1;
        char **ttype2;
	char **tform2;
	char **tunit2;
        long nrows_event_hdu;
        int tm_apid;
        int tm_type;
        int tm_subtype;

};
#endif
