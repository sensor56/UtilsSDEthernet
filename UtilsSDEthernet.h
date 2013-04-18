/*
 * Copyright (c) 2013 by Xavier HINAULT - support@mon-club-elec.fr
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 3
 * or the GNU Lesser General Public License version 3, both as
 * published by the Free Software Foundation.
 */

/* Cette librairie contient plusieurs fonctions générales utiles
notamment pour assurer la gestion de la carte SD à distance par le réseau ethernet
*/

#ifndef UtilsSDEthernet_h
#define UtilsSDEthernet_h

#include "Arduino.h"
#include "Ethernet.h"


class UtilsSDEthernet {

  private : // variables internes
  
  public :
 
  public :
    UtilsSDEthernet(); // constructeur
  
	void getContentDir(EthernetClient clientIn, char* dirIn, int numTabsIn); 
	int getNumberOfLines(EthernetClient clientIn, char* nomFichierIn, boolean debug); 
	String getLine(EthernetClient clientIn, char* nomFichierIn, int lineNumberIn, boolean debug); 
	int getFileSize(EthernetClient clientIn, char* nomFichierIn, boolean debug); 
	void read(EthernetClient clientIn, char* nomFichierIn, boolean debug); 
	void writeFile(EthernetClient clientIn, char* nomFichierIn, String toWriteIn, boolean debug);
	void createFile(EthernetClient clientIn, char* nomFichierIn, boolean debug);
	
  private : // fonctions internes


}; // fin classe 


#endif
