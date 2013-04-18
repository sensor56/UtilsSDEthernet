UtilsSDEthernet
===============

Ma librairie UtilsSDEthernet implémente toute une série de fonctions en surcouche de la librairie SD et qui rendent la manipulation de la carte SD plus « naturelle » pour l'utilisateur, version pour gestion par le réseau Ethernet.

Les fonctions disponibles sont notamment : 
* void getContentDir(EthernetClient clientIn, char* dirIn, int numTabsIn) : renvoie vers le client Ethernet le contenu d'un répertoire
* int getNumberOfLines(EthernetClient clientIn, char* nomFichierIn, boolean debug) : renvoie vers le client Ethernet le nombre de ligne d'un fichier
* String getLine(EthernetClient clientIn, char* nomFichierIn, int lineNumberIn, boolean debug) : renvoie vers le client Ethernet une ligne à partir de son numéro
* int getFileSize(EthernetClient clientIn, char* nomFichierIn, boolean debug) : renvoie vers le client Ethernet la taille d'un fichier
* void read(EthernetClient clientIn, char* nomFichierIn, boolean debug) : lit et renvoie vers le client Ethernet  le contenu d'un fichier
* void writeFile(EthernetClient clientIn, char* nomFichierIn, String toWriteIn, boolean debug) : ajoute une ligne + saut de ligne à la fin du fichier
* void createFile(EthernetClient clientIn, char* nomFichierIn, boolean debug)  : crée un fichier 

Cette librairie est utilisée notamment dans mon projet "The Open Datalogger Project" : 
https://github.com/sensor56/TheOpenDataloggerProject 
