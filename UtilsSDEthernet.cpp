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

#include "Arduino.h"
#include "SPI.h" // fichier libairie pour communication SPI utilisée par carte SD
#include "SD.h" // fichier librairie pour gestion de la carte SD
#include "Ethernet.h" // fichier librairie Ethernet 

#include "Utils.h"
#include "UtilsSDEthernet.h"


  UtilsSDEthernet::UtilsSDEthernet(){
 
  }
  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fonction affichant le contenu d'un répertoire de la carte mémoire SD sur le port série 
// modifié par X. HINAULT - www.mon-club-elec.fr - GPLv3 - 03/2013 – supporte appels itératifs de la fonction
// d'après exemple  listFiles de la librairie SD | created   Nov 2010  by David A. Mellis | modified 9 Apr 2012 by Tom Igoe

void UtilsSDEthernet::getContentDir(EthernetClient clientIn, char* dirIn, int numTabsIn) { // la fonction reçoit le nom du répertoire, le décalage tab et le drapeau de debug
  // La fonction reçoit : 
  // > le client Ethernet à utiliser 
  // > le nom du répertoire 
  // > le nombre de tab de début - obligatoire car reçu en paramètre ré-entrant...   
  // la fonction ne renvoie rien
  
  File openDir=SD.open(dirIn); // ouvre la SD Card à l'emplacement indiqué et renvoie l'objet File correspondant
  openDir.rewindDirectory(); // RAZ la position dans le rép
  
  boolean firstPassWhile=false; // drapeau 1er passage while 
  boolean debug=true; 
  
   while(true) { // tant que vrai = crée une "loop" qui s'exécute tant que contenu
   // la sortie se fait par break;
     
     File contenu =  openDir.openNextFile(); // ouvre le fichier ou repertoire suivant - NB : un répertoire est aussi un objet File 
          
     if (! contenu) { // si aucun nouveau fichier /repertoire
     
       //if (debug) Serial.println("** vide**");
       
       contenu.close(); // ferme fichier avant sortie while
       break; // sort de la fonction 
       
     } // fin si aucun nouveau fichier / répertoire
     
     if (firstPassWhile==false) {  // si premier passage while = on remonte pour etre sur d'etre au debut du rep 
     
         contenu.rewindDirectory(); 
         firstPassWhile==true; // est déjà passé 1 fois
     
     } // fin if first pass

     // affiche le nombre de tab voulu - 0 si racine, 1 si sous Rép, 2 si sous-sous rép, etc..
     for (int i=0; i<numTabsIn; i++) {if (debug) Serial.print('\t');}
  
     if (debug) clientIn.print(contenu.name()); // affiche le nom du fichier/repertoire courant 

     if (contenu.isDirectory()) { // si le fichier est un répertoire
       if (debug) clientIn.println("/"); // affiche un slash
       contenu.close(); // ferme fichier avant sortie while
       getContentDir(clientIn, contenu.name(), numTabsIn+1); // affiche le contenu en décalant d'un tab - Fonction ré-entrante qui s'appelle elle-meme!!
     } // fin si le fichier est un répertoire
     
     else { // sinon affiche la taille - les fichiers ont une taille, pas les répertoires 
       
       if (debug)clientIn.print("\t\t"); // affiche de Tab de décalage
       if (debug)clientIn.print(contenu.size(), DEC); // affiche la taille
       if (debug)clientIn.println(" octets"); // affiche la taille
       
       contenu.close(); // ferme fichier avant sortie while
       
     } // fin sinon = si pas un rép
     
   } // fin while(true)
   
   openDir.close(); // ferme fichier avant sortie fonction
   
   //delay(10); // pause
   
} // fin fonction getContentDir

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fonction renvoyant le nombre de lignes d'un fichier sur carte mémoire SD
// par X. HINAULT - www.mon-club-elec.fr - GPLv3 - 03/2013
int UtilsSDEthernet::getNumberOfLines(EthernetClient clientIn, char* nomFichierIn, boolean debug) {
  // La fonction reçoit : 
  // > le client Ethernet à utiliser 
  // > le nom du fichier au format 8.3
  // > un drapeau d'affichage de messages de debug
  
  // la fonction renvoie le nombre de ligne
 
   File dataFileIn=SD.open(nomFichierIn, FILE_READ); // ouvre le fichier en lecture - NOM FICHIER en 8.3 ++++
 
   if (debug) clientIn.println(F("------")); 
   
   int comptLine=0; // variable de comptage des lignes 
  
   if (dataFileIn){ // le fichier est True si créé

      if (debug)clientIn.println(F("Ouverture fichier OK")); 
  
      while (dataFileIn.available()) { // tant que des données sont disposnibles dans le fichier
      // le fichier peut etre considéré comme un "buffer" de données comme le buffer du port série
    
        char c = dataFileIn.read(); // lit le caractère suivant
        if (debug)clientIn.print(c); // affiche le caractère courant

        if (c==10) { // si le caractère est \n : c'est une fin de ligne
        comptLine=comptLine+1; 
        } // fin if saut de ligne
      
      } // fin while available   

     if (debug) clientIn.println("------"); 
  
      dataFileIn.close(); // fermeture du fichier obligatoire après accès 
    
      
    } // si fichier existe 
    else { // sinon = si probleme creation
      clientIn.println(F("Probleme ouverture fichier")); 
    } // fin else

    return(comptLine); // renvoie le nombre de ligne
  
} // fin fonction getNumberOfLines

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// fonction lecture de ligne dans un fichier de carte mémoire SD
// par X. HINAULT - www.mon-club-elec.fr -GPL v3 - 03/2013

 String UtilsSDEthernet::getLine(EthernetClient clientIn, char* nomFichierIn, int lineNumberIn, boolean debug) { 
  // la fonctin reçoit : 
  // > le client Ethernet à utiliser 
  // > le nom du fichier
  // > le numéro de la ligne
  // > un drapeau pour messages debug

  // la fonction renvoie le String correspondant à la ligne

   File dataFileIn=SD.open(nomFichierIn, FILE_READ); // ouvre le fichier en lecture - NOM FICHIER en 8.3 ++++
 
   if (debug) clientIn.println("------"); 

   //int line=1499; // numero de la ligne voulue - 1ère ligne = ligne 1 
   
   String strLine=""; 
   int comptLine=0; 
   int lastPosition=0; // dernière position
    int currentPosition=0; // dernière position

  if (dataFileIn){ // le fichier est True si créé
      if (debug)clientIn.println(F("Ouverture fichier OK")); 
  
      while (dataFileIn.available()) { // tant que des données sont disposnibles dans le fichier
      // le fichier peut etre considéré comme un "buffer" de données comme le buffer du port série
    
        char c = dataFileIn.read(); // lit le caractère suivant
        //Serial.print(c); // affiche le caractère courant

        if (c==10) { // si le caractère est \n : c'est une fin de ligne
               comptLine=comptLine+1; 
               
               if (comptLine==lineNumberIn) { // si on a atteint la ligne voulue
                 currentPosition=dataFileIn.position(); // mémorise la position actuelle 
                 
                 for (int i=lastPosition; i<currentPosition; i++) { // on défile de la dernière position retenue à la position courante
                 
                     dataFileIn.seek(i); // se position en i
                     strLine=strLine+char(dataFileIn.read()); // ajoute le caractère au strLine
                   
                 } // fin for
               
                 break; // sort du while
               
             }// fin if arrivé à la ligne voulue
           
               lastPosition=dataFileIn.position(); 
               //Serial.print ("lastPosition : "); 
               //Serial.println (lastPosition); 

        } // fin if saut de ligne
      
        //contenuFichier=contenuFichier+c; // ajoute le char au String  
  
      } // fin while available   
  
      dataFileIn.close(); // fermeture du fichier obligatoire après accès 
    
      if (debug) clientIn.println(F("ligne voulue : ")); 
      if (debug) clientIn.println(strLine); 
      if (debug) clientIn.println(F("------")); 
      
      //Serial.println("Contenu du fichier : "); 
      //Serial.println(contenuFichier); // affiche le contenu du fichier
    
    } // si fichier existe 
    else { // sinon = si probleme creation
      clientIn.println(F("Probleme creation fichier")); 
    } // fin else

    return(strLine); 

 } // fin fonction getLine
 
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 // Fonction renvoyant la taille en octet d'un fichier sur carte mémoire SD
// par X. HINAULT - www.mon-club-elec.fr - GPLv3 - 03/2013
int UtilsSDEthernet::getFileSize(EthernetClient clientIn, char* nomFichierIn, boolean debug) {
  // La fonction reçoit : 
  // > le client Ethernet à utiliser 
  // > le nom du fichier au format 8.3
  // > un drapeau d'affichage de messages de debug
  
  // la fonction renvoie le nombre d'octet du fichier
 
   File dataFileIn=SD.open(nomFichierIn, FILE_READ); // ouvre le fichier en lecture - NOM FICHIER en 8.3 ++++
     
     if (debug) clientIn.println("------"); 

    clientIn.println("------"); 

   if (dataFileIn){ // le fichier est True si créé

      if (debug)clientIn.println(F("Ouverture fichier OK")); 
    
  
      if (debug) clientIn.print(F("Taille du fichier : "));
      if (debug) clientIn.print(dataFileIn.size());
      if (debug) clientIn.println(F(" octets."));
  
      return(dataFileIn.size()); // renvoie la taille du fichier

       if (debug) clientIn.println(F("------")); 
  
      dataFileIn.close(); // fermeture du fichier obligatoire après accès 

    } // si fichier existe 
    else { // sinon = si probleme creation

      clientIn.println(F("Probleme ouverture fichier")); 
      return(0); 
      
    } // fin else
  
} // fin fonction getFileSize

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 

// Fonction affichant sur le port série le contenu d'un fichier sur carte mémoire SD
// par X. HINAULT - www.mon-club-elec.fr - GPLv3 - 03/2013
void UtilsSDEthernet::read(EthernetClient clientIn, char* nomFichierIn, boolean debug) {
  // La fonction reçoit : 
  // > le client Ethernet à utiliser 
  // > le nom du fichier au format 8.3
  // > un drapeau d'affichage de messages de debug
  
  // la fonction ne renvoie rien 
 
                // lecture du contenu du fichier  
                File dataFile=SD.open(nomFichierIn, FILE_READ); // ouvre le fichier en lecture - NOM FICHIER en 8.3 ++++
                // un seul fichier ne peut etre ouvert à la fois - fermer au préalable tout fichier déjà ouvert
               
                clientIn.println(F("------")); 
                    
                if (dataFile){ // le fichier est True si créé
                  if (debug) clientIn.println(F("Ouverture fichier OK")); 
                
                  while (dataFile.available()) { // tant que des données sont disposnibles dans le fichier
                  // le fichier peut etre considéré comme un "buffer" de données comme le buffer du port série
                
                    char c = dataFile.read(); // lit le caractère suivant
                    clientIn.print(c); // affiche le caractère courant
                    
                    //if (c==10) delay(10); // pause enttre 2 lignes
                  } // fin while available
                
                  dataFile.close(); // fermeture du fichier obligatoire après accès 
                  
                } // si fichier True 
                else { // sinon = si probleme creation
                  clientIn.println(F("Probleme lecture fichier")); 
                } // fin else              
  
} // fin fonction readSerial 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fonction ajoutant une chaine dans un fichier sur carte mémoire SD
// par X. HINAULT - www.mon-club-elec.fr - GPLv3 - 03/2013
void UtilsSDEthernet::writeFile(EthernetClient clientIn, char* nomFichierIn, String toWriteIn, boolean debug) {
  // La fonction reçoit : 
  // > le client Ethernet à utiliser 
  // > le nom du fichier au format 8.3
  // > la chaine à écrire dans le fichier 
  // > un drapeau d'affichage de messages de debug
  
  // la fonction ne renvoie rien 
 
               File dataFile=SD.open(nomFichierIn, FILE_WRITE); // crée / ouvre un objet fichier et l'ouvre en mode écriture - NOM FICHIER en 8.3 ++++
              
              if (dataFile){ // le fichier est True si créé
                if(debug)clientIn.println(F("Ouverture fichier OK")); 
                dataFile.println(toWriteIn); // chaine à écrire
                dataFile.close(); // fermeture du fichier obligatoire après accès 
                if(debug)clientIn.println(F("Ecriture fichier OK")); 
              } // si fichier ==True 
              else { // sinon = si probleme creation
                clientIn.println(F("Probleme ouverture fichier")); 
              } // fin else datafile
  
} // fin fonction writeFile 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fonction créant un fichier sur carte mémoire SD
// par X. HINAULT - www.mon-club-elec.fr - GPLv3 - 03/2013
void UtilsSDEthernet::createFile(EthernetClient clientIn, char* nomFichierIn, boolean debug) {
  // La fonction reçoit : 
  // > le client Ethernet à utiliser 
  // > le nom du fichier au format 8.3
  // > un drapeau d'affichage de messages de debug
  
  // la fonction ne renvoie rien 
 
              // création nouveau fichier  
              File dataFile=SD.open(nomFichierIn, FILE_WRITE); // crée / ouvre un objet fichier et l'ouvre en mode écriture - NOM FICHIER en 8.3 ++++
              // > soit en mode : FILE_WRITE: ouvre le fichier pour lecture et écriture, en démarrant au début du fichier. 
              // Important : Si le fichier est ouvert pour écriture, il sera créé si il n'existe pas déjà (cependant le chemin le contenant doit déjà exister)
             
              clientIn.println("------"); 
                
              if (dataFile){ // le fichier est True si créé
                clientIn.println(F("Creation nouveau fichier OK")); 
                dataFile.print(""); // chaine vide
                dataFile.close(); // fermeture du fichier obligatoire après accès 
              } // si fichier existe 
              else { // sinon = si probleme creation
                clientIn.println(F("Probleme creation fichier")); 
              } // fin else datafile
  
} // fin fonction createFile 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
