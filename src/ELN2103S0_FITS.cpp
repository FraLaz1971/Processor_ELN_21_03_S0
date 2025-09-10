/***************************************************************************
                          ELN2103S0_FITS.cpp  -  description
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

#include "ELN2103S0_FITS.h"
#include "ProcessorDefinition.h"
#include <string.h>
#include <fitsio.h>
#include "ELN2103S0Processor.h"

#define NC_TIME                 1
#define NC_INT_COUNTER  2
#define NC_MC_SIGNAL    3
#define NPRECOL 2

ELN2103S0_FITS::ELN2103S0_FITS()
{
  ncol1 = 20;
  ncol2 = 45;
  ttype1 = new char* [ncol1];
  ttype2 = new char* [ncol2];
  tform1 = new char* [ncol1];
  tform2 = new char* [ncol2];
  tunit1 = new char* [ncol1];
  tunit2 = new char* [ncol2];
  // CCSDS header data fields  
  tform1[0] = "1J"; ttype1[0] = "PAKTNUMB"; tunit1[0] = ""; // n. of packet
  tform1[1] = "1B"; ttype1[1] = "VERNUM";   tunit1[1] = ""; // packet id version number 3bit f0
  tform1[2] = "1B"; ttype1[2] = "PKTTYPE";  tunit1[2] = ""; // type of the packet (0=TM/1=TC) 1bit f1
  tform1[3] = "1B"; ttype1[3] = "DFHFLAG";  tunit1[3] = ""; // data field header flag 1bit f2
  tform1[4] = "1B"; ttype1[4] = "PID";     tunit1[4] = ""; // PID (source or destination) f3
  tform1[5] = "1B"; ttype1[5] = "CAT";     tunit1[5] = ""; // CATEGORY  f3
  tform1[6] = "1U"; ttype1[6] = "APID";     tunit1[6] = ""; // Application ID (source or destination) f3
  tform1[7] = "1B"; ttype1[7] = "SEQFLAG";  tunit1[7] = ""; // sequence flag 2 bit f4
  tform1[8] = "1U"; ttype1[8] = "SRCSEQCNT";tunit1[8] = ""; // source sequence counter 14 bit f5
  tform1[9] = "1U"; ttype1[9] = "PKTLENGTH";tunit1[9] = ""; // packet length (n. of next bytes-1) 16 bit f6
  // DFH data fields
  tform1[10] = "1B"; ttype1[10] = "SPARE0";   tunit1[10] = "";  // spare0 1 bit f0
  tform1[11] = "1B"; ttype1[11] = "PUSVER";   tunit1[11] = ""; // PUS version 3 bit f1
  tform1[12] = "1B"; ttype1[12] = "SPARE1"; tunit1[12] = ""; // spare1 4 bit f2
  tform1[13] = "1B"; ttype1[13] = "TYPE";   tunit1[13] = "";// data type 8 bit f3
  tform1[14] = "1B"; ttype1[14] = "SUBTYPE";tunit1[14] = "";// data subtype 8 bit f4
  tform1[15] = "1B"; ttype1[15] = "DEST";   tunit1[15] = "";// destination ID  f5
  tform1[16] = "1U"; ttype1[16] = "TIMEMSW";tunit1[16] = "s*2^16";// time tag seconds (MSW) f6
  tform1[17] = "1U"; ttype1[17] = "TIMELSW";tunit1[17] = "s";// time tag seconds (LSW) f7
  tform1[18] = "1U"; ttype1[18] = "SUBSEC"; tunit1[18] = "s";// time tag seconds (LSW) f8
  tform1[19] = "1D"; ttype1[19] = "TIME";   tunit1[19] = "s";// time tag seconds f6<<16+f7+f8
   
  // second binary table     
  tform2[0] = "1J"; ttype2[0] = "PAKTNUMB"; tunit2[0] = ""; // n of packet
  tform2[1] = "1B"; ttype2[1] = "SID"; tunit2[1] = "";// Structure ID 8 bit f0
  tform2[2] = "1B"; ttype2[2] = "NSECTORS"; tunit2[2] = "";// number of sectors 32 or 4 8 bit f1
  tform2[3] = "1U"; ttype2[3] = "ACCTIME"; tunit2[3] = "s";// accumulation time 16 bit seconds f2
  tform2[4] = "1B"; ttype2[4] = "DFLAG"; tunit2[4] = "";// data flag 1 simulated/sampled bit f3
  tform2[5] = "1B"; ttype2[5] = "SFLAG"; tunit2[5] = "";// shutter flag 1 bit enabled/disabled f4
  tform2[6] = "1U"; ttype2[6] = "SHUTFREQ"; tunit2[6] = "";// shutter frequency 14 bit f5
  tform2[7] = "1U"; ttype2[7] = "HV1_CUR"; tunit2[7] = "";// HV 1 current 16 bit f6
  tform2[8] = "1U"; ttype2[8] = "HV2_CUR"; tunit2[8] = "";// HV 2 current 16 bit  f7
  tform2[9] = "1U"; ttype2[9] = "HVMCPCUR"; tunit2[9] = "";// HV MCP current 16 bit  f8
  tform2[10] = "1U"; ttype2[10] = "MCP_TEMP"; tunit2[10] = "";// MCP temperature 16 bit f9
  tform2[11] = "1U"; ttype2[11] = "H_CH01"; tunit2[11] = "";// Sector Histogram channel 1  16 bit f10
  tform2[12] = "1U"; ttype2[12] = "H_CH02"; tunit2[12] = "";// Sector Histogram channel 2 16 bit f11
  tform2[13] = "1U"; ttype2[13] = "H_CH03"; tunit2[13] = "";// Sector Histogram channel 3 16 bit f12
  tform2[14] = "1U"; ttype2[14] = "H_CH04"; tunit2[14] = "";// Sector Histogram channel 4 16 bit f13
  tform2[15] = "1U"; ttype2[15] = "H_CH05"; tunit2[15] = "";// Sector Histogram channel 5 16 bit f14
  tform2[16] = "1U"; ttype2[16] = "H_CH06"; tunit2[16] = "";// Sector Histogram channel 6 16 bit f15
  tform2[17] = "1U"; ttype2[17] = "H_CH07"; tunit2[17] = "";// Sector Histogram channel 7 16 bit f16
  tform2[18] = "1U"; ttype2[18] = "H_CH08"; tunit2[18] = "";// Sector Histogram channel 8 16 bit f17
  tform2[19] = "1U"; ttype2[19] = "H_CH09"; tunit2[19] = "";// Sector Histogram channel 9 16 bit f18
  tform2[20] = "1U"; ttype2[20] = "H_CH10"; tunit2[20] = "";// Sector Histogram channel 10 16 bit f19
  tform2[21] = "1U"; ttype2[21] = "H_CH11"; tunit2[21] = "";// Sector Histogram channel 11 16 bit f20
  tform2[22] = "1U"; ttype2[22] = "H_CH12"; tunit2[22] = "";// Sector Histogram channel 12 16 bit f21
  tform2[23] = "1U"; ttype2[23] = "H_CH13"; tunit2[23] = "";// Sector Histogram channel 13 16 bit f22
  tform2[24] = "1U"; ttype2[24] = "H_CH14"; tunit2[24] = "";// Sector Histogram channel 14 16 bit f23
  tform2[25] = "1U"; ttype2[25] = "H_CH15"; tunit2[25] = "";// Sector Histogram channel 15 16 bit f24
  tform2[26] = "1U"; ttype2[26] = "H_CH16"; tunit2[26] = "";// Sector Histogram channel 16 16 bit f25
  tform2[27] = "1U"; ttype2[27] = "H_CH17"; tunit2[27] = "";// Sector Histogram channel 17 16 bit f26
  tform2[28] = "1U"; ttype2[28] = "H_CH18"; tunit2[28] = "";// Sector Histogram channel 18 16 bit f27
  tform2[29] = "1U"; ttype2[29] = "H_CH19"; tunit2[29] = "";// Sector Histogram channel 19 16 bit f28
  tform2[30] = "1U"; ttype2[30] = "H_CH20"; tunit2[30] = "";// Sector Histogram channel 20 16 bit f29
  tform2[31] = "1U"; ttype2[31] = "H_CH21"; tunit2[31] = "";// Sector Histogram channel 21 16 bit f30
  tform2[32] = "1U"; ttype2[32] = "H_CH22"; tunit2[32] = "";// Sector Histogram channel 22 16 bit f31
  tform2[33] = "1U"; ttype2[33] = "H_CH23"; tunit2[33] = "";// Sector Histogram channel 23 16 bit f32
  tform2[34] = "1U"; ttype2[34] = "H_CH24"; tunit2[34] = "";// Sector Histogram channel 24 16 bit f33
  tform2[35] = "1U"; ttype2[35] = "H_CH25"; tunit2[35] = "";// Sector Histogram channel 25 16 bit f34
  tform2[36] = "1U"; ttype2[36] = "H_CH26"; tunit2[36] = "";// Sector Histogram channel 26 16 bit f35
  tform2[37] = "1U"; ttype2[37] = "H_CH27"; tunit2[37] = "";// Sector Histogram channel 27 16 bit f36
  tform2[38] = "1U"; ttype2[38] = "H_CH28"; tunit2[38] = "";// Sector Histogram channel 28 16 bit f37
  tform2[39] = "1U"; ttype2[39] = "H_CH29"; tunit2[39] = "";// Sector Histogram channel 29 16 bit f38
  tform2[40] = "1U"; ttype2[40] = "H_CH30"; tunit2[40] = "";// Sector Histogram channel 30 16 bit f39
  tform2[41] = "1U"; ttype2[41] = "H_CH31"; tunit2[41] = "";// Sector Histogram channel 31 16 bit f40
  tform2[42] = "1U"; ttype2[42] = "H_CH32"; tunit2[42] = "";// Sector Histogram channel 32 16 bit f41
  tform2[43] = "1U"; ttype2[43] = "MARK"; tunit2[43] = "";// End packet mark (CAFE)  16 bit f42
  tform2[44] = "1D"; ttype2[44] = "TIME";   tunit2[44] = "s";// time tag seconds f6<<16+f7+f8

}




ELN2103S0_FITS::~ELN2103S0_FITS()
{
  delete[] tform1;
  delete[] ttype1;
  delete[] tunit1;
  delete[] tform2;
  delete[] ttype2;
  delete[] tunit2;  
}




bool ELN2103S0_FITS::init(char**c, int*i, char* filenameconfig) throw (PacketException*)
{
    int status;
    long nrows = 0;

    char extname1[]  = "PACKETS";
    char extname2[]  = "SECTORS";    

    char* filename = getFileName();
    nrows_event_hdu = 1;
    //important
    currentrow = 1;
	status=0;

    /* create new FITS file */
    if (fits_create_file(fptr, filename, &status))
        printerror( status );

    if ( fits_create_tbl(*fptr, BINARY_TBL, nrows, ncol1, ttype1, tform1,
        tunit1, extname1, &status) )
        printerror( status );
        
    writeInitHeaderKey(2);
  
    if ( fits_create_tbl(*fptr, BINARY_TBL, nrows, ncol2, ttype2, tform2,
        tunit2, extname2, &status) )
        printerror( status );

    writeInitHeaderKey(3);

    printf("ELN2103S0 processor: FITS header written\n");


    return status;
}


void ELN2103S0_FITS::updateKey(char**c, int* i) {
    int status = 0;
    int tot_trig_bad = i[1];
    int tot_nblocks = i[0];
    int century = i[2];    
   	if ( fits_update_key(*fptr, TINT, "APID",
 		(void*) &tm_apid, "Application Process ID", &status) )
 		printerror( status );
   	if ( fits_update_key(*fptr, TINT, "PKTTYPE",
 		(void*) &tm_type, "Packet Type", &status) )
 		printerror( status );
   	if ( fits_update_key(*fptr, TINT, "PKTSTYPE",
 		(void*) &tm_subtype, "Packet Sub-Type", &status) )
 		printerror( status );
   	if ( fits_update_key(*fptr, TINT, "CENTURY",
 		(void*) &century, "Valid Century value", &status) )
 		printerror( status );   
       
}


bool ELN2103S0_FITS::close(char**c, int* i)
{
    int status = 0;
    if ( fits_movabs_hdu(*fptr, 1, 0, &status) )
        printerror( status );
    writeEndHeaderKey(1);
    updateKey(c,i);

    if ( fits_movabs_hdu(*fptr, 2, 0, &status) )
        printerror( status );
    writeEndHeaderKey(2);
    updateKey(c,i);

    if ( fits_movabs_hdu(*fptr, 3, 0, &status) )
        printerror( status );
    writeEndHeaderKey(3);
    updateKey(c,i);
    
    /* close the FITS file */
    if ( fits_close_file(*fptr, &status) )
        printerror( status );

    return status;

}




bool ELN2103S0_FITS::writeData(void* data, dword nrows)
{
    long firstelem;
    int status;
    byte b;             //1 byte unsigned
    word w, wh, wl;     //2 byte unsigned
    long int li, lil, lih;
    dword d, dh, dl;    //4 byte unsigned
    double d64;
    float e, e1, e2;
    status=0;
    firstelem = 1;
    currentrow = nrows; //il numero progressivo del packet nella misura corrente
    
    BLOCK* block = (BLOCK*) data;
    Packet* p = block->p;
    tm_apid = (p->header->getFieldValue(3)<<4)+p->header->getFieldValue(4);
    tm_type = p->dataField->dataFieldHeader->getFieldValue(3);
    tm_subtype = p->dataField->dataFieldHeader->getFieldValue(4);
    
    //********************************************************************************************    
    //PARTE 1: scrivere una riga per ogni packet nel HDU "PACKET"
    //********************************************************************************************    
    if ( fits_movabs_hdu(*fptr, 2, 0, &status) )
        printerror( status );
      //1.1) Scrivere l'header ************************************************
      //Packet Number
      if(fits_write_col(*fptr, TINT32BIT, 1, currentrow, firstelem, (long)1, &currentrow, &status))
        printerror(status);      
      //VERNUM
      b = p->header->getFieldValue(0);
      if(fits_write_col(*fptr, TBYTE, 2, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //PKTTYPE
      b = p->header->getFieldValue(1);
      if(fits_write_col(*fptr, TBYTE, 3, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //DFHFLAG           
      b = p->header->getFieldValue(2);
      if(fits_write_col(*fptr, TBYTE, 4, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //PID
      b = p->header->getFieldValue(3);
      if(fits_write_col(*fptr, TBYTE, 5, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //CATEGORY
      b = p->header->getFieldValue(4);
      if(fits_write_col(*fptr, TBYTE, 6, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //APID  
      w = (p->header->getFieldValue(3)<<4)+p->header->getFieldValue(4);
      if(fits_write_col(*fptr, TUSHORT, 7, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
      //SEQFLAG
      b = p->header->getFieldValue(5);
      if(fits_write_col(*fptr, TBYTE, 8, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //SRCSEQCNT  
      w = p->header->getFieldValue(6);
      if(fits_write_col(*fptr, TUSHORT, 9, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
      //PKTLENGT  
      w = p->header->getFieldValue(7);
      if(fits_write_col(*fptr, TUSHORT, 10, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
      //1.2) Scrivere il data field header ****************************************
      //SPARE0
      b = p->dataField->dataFieldHeader->getFieldValue(0);
      if(fits_write_col(*fptr, TBYTE, 11, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //PUSVER
      b = p->dataField->dataFieldHeader->getFieldValue(1);
      if(fits_write_col(*fptr, TBYTE, 12, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);      
      //SPARE1
      b = p->dataField->dataFieldHeader->getFieldValue(2);
      if(fits_write_col(*fptr, TBYTE, 13, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //TYPE
      b = p->dataField->dataFieldHeader->getFieldValue(3);
      if(fits_write_col(*fptr, TBYTE, 14, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      //SUBTYPE
      b = p->dataField->dataFieldHeader->getFieldValue(4);
      if(fits_write_col(*fptr, TBYTE, 15, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      // DESTINATION
      b = p->dataField->dataFieldHeader->getFieldValue(5);
      if(fits_write_col(*fptr, TBYTE, 16, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
      // TIMEMSW
      w = p->dataField->dataFieldHeader->getFieldValue(6);
      if(fits_write_col(*fptr, TUSHORT, 17, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
      // TIMELSW
      w = p->dataField->dataFieldHeader->getFieldValue(7);
      if(fits_write_col(*fptr, TUSHORT, 18, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
      // SUBSECONDS
      w = p->dataField->dataFieldHeader->getFieldValue(8);
      if(fits_write_col(*fptr, TUSHORT, 19, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
      // TIME
      dh = p->dataField->dataFieldHeader->getFieldValue(6);
      dl = p->dataField->dataFieldHeader->getFieldValue(7);
      w = p->dataField->dataFieldHeader->getFieldValue(7);
      d64=(dh<<16)+dl+w;
      if(fits_write_col(*fptr, TDOUBLE, 20, currentrow, firstelem, (long)1, &d64, &status))
        printerror(status);

      SDFNoBlockFixed* sdf;
      sdf = (SDFNoBlockFixed*) p->dataField->sourceDataField;
      //SID

      //1.4) Accesso alla parte variabile del SDF. Si scorrono tutti i blocchi      
        if ( fits_movabs_hdu(*fptr, 3, 0, &status) )
            printerror( status );
        //Packet Number
        if(fits_write_col(*fptr, TINT32BIT, 1, nrows_event_hdu, firstelem, (long)1, &currentrow, &status))
            printerror(status);  
      b = sdf->getFieldValue(0);
      if(fits_write_col(*fptr, TBYTE, 2, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
       // N. SECTORS
      b = sdf->getFieldValue(1);
      if(fits_write_col(*fptr, TBYTE, 3, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
       // ACCTIME
      w = sdf->getFieldValue(1);
      if(fits_write_col(*fptr, TUSHORT, 4, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // DATATYPEFLAG
      b = sdf->getFieldValue(3);
      if(fits_write_col(*fptr, TBYTE, 5, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
       // SHUTTERFLAG
      b = sdf->getFieldValue(4);
      if(fits_write_col(*fptr, TBYTE, 6, currentrow, firstelem, (long)1, &b, &status))
        printerror(status);
       // SHUTTERFREQ
      w = sdf->getFieldValue(5);
      if(fits_write_col(*fptr, TUSHORT, 7, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // HV1CURR
      w = sdf->getFieldValue(6);
      if(fits_write_col(*fptr, TUSHORT, 8, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // HV2CURR
      w = sdf->getFieldValue(7);
      if(fits_write_col(*fptr, TUSHORT, 9, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // HVMCPCURR
      w = sdf->getFieldValue(8);
      if(fits_write_col(*fptr, TUSHORT, 10, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // MCPTEMP
      w = sdf->getFieldValue(9);
      if(fits_write_col(*fptr, TUSHORT, 11, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH01
      w = sdf->getFieldValue(10);
      if(fits_write_col(*fptr, TUSHORT, 12, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH02
      w = sdf->getFieldValue(11);
      if(fits_write_col(*fptr, TUSHORT, 13, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH03
      w = sdf->getFieldValue(12);
      if(fits_write_col(*fptr, TUSHORT, 14, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH04
      w = sdf->getFieldValue(13);
      if(fits_write_col(*fptr, TUSHORT, 15, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH05
      w = sdf->getFieldValue(14);
      if(fits_write_col(*fptr, TUSHORT, 16, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH06
      w = sdf->getFieldValue(15);
      if(fits_write_col(*fptr, TUSHORT, 17, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH07
      w = sdf->getFieldValue(16);
      if(fits_write_col(*fptr, TUSHORT, 18, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH08
      w = sdf->getFieldValue(17);
      if(fits_write_col(*fptr, TUSHORT, 19, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH09
      w = sdf->getFieldValue(18);
      if(fits_write_col(*fptr, TUSHORT, 20, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH10
      w = sdf->getFieldValue(19);
      if(fits_write_col(*fptr, TUSHORT, 21, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH11
      w = sdf->getFieldValue(20);
      if(fits_write_col(*fptr, TUSHORT, 22, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH12
      w = sdf->getFieldValue(21);
      if(fits_write_col(*fptr, TUSHORT, 23, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH13
      w = sdf->getFieldValue(22);
      if(fits_write_col(*fptr, TUSHORT, 24, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH14
      w = sdf->getFieldValue(23);
      if(fits_write_col(*fptr, TUSHORT, 25, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH15
      w = sdf->getFieldValue(24);
      if(fits_write_col(*fptr, TUSHORT, 26, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH16
      w = sdf->getFieldValue(25);
      if(fits_write_col(*fptr, TUSHORT, 27, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH17
      w = sdf->getFieldValue(26);
      if(fits_write_col(*fptr, TUSHORT, 28, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH18
      w = sdf->getFieldValue(27);
      if(fits_write_col(*fptr, TUSHORT, 29, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH19
      w = sdf->getFieldValue(28);
      if(fits_write_col(*fptr, TUSHORT, 30, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH20
      w = sdf->getFieldValue(29);
      if(fits_write_col(*fptr, TUSHORT, 31, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH21
      w = sdf->getFieldValue(30);
      if(fits_write_col(*fptr, TUSHORT, 32, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH22
      w = sdf->getFieldValue(31);
      if(fits_write_col(*fptr, TUSHORT, 33, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH23
      w = sdf->getFieldValue(32);
      if(fits_write_col(*fptr, TUSHORT, 34, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH24
      w = sdf->getFieldValue(33);
      if(fits_write_col(*fptr, TUSHORT, 35, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH25
      w = sdf->getFieldValue(34);
      if(fits_write_col(*fptr, TUSHORT, 36, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH26
      w = sdf->getFieldValue(35);
      if(fits_write_col(*fptr, TUSHORT, 37, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH27
      w = sdf->getFieldValue(36);
      if(fits_write_col(*fptr, TUSHORT, 38, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH28
      w = sdf->getFieldValue(37);
      if(fits_write_col(*fptr, TUSHORT, 39, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH29
      w = sdf->getFieldValue(38);
      if(fits_write_col(*fptr, TUSHORT, 40, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH30
      w = sdf->getFieldValue(39);
      if(fits_write_col(*fptr, TUSHORT, 41, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH31
      w = sdf->getFieldValue(40);
      if(fits_write_col(*fptr, TUSHORT, 42, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // H_CH32
      w = sdf->getFieldValue(41);
      if(fits_write_col(*fptr, TUSHORT, 43, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
       // MARK
      w = sdf->getFieldValue(42);
      if(fits_write_col(*fptr, TUSHORT, 44, currentrow, firstelem, (long)1, &w, &status))
        printerror(status);
      // TIME
      dh = p->dataField->dataFieldHeader->getFieldValue(6);
      dl = p->dataField->dataFieldHeader->getFieldValue(7);
      w = p->dataField->dataFieldHeader->getFieldValue(7);
      d64=(dh<<16)+dl+w;
      if(fits_write_col(*fptr, TDOUBLE, 45, currentrow, firstelem, (long)1, &d64, &status))
        printerror(status);
        
        //importante            
        nrows_event_hdu++;    
                                          
    
        
    return status;
}
