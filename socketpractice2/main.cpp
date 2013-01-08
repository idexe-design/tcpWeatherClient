//
//  main.cpp
//  socketpractice2
//
//  Created by Johannes Schmidt on 08.01.13.
//  Copyright (c) 2013 Kevin Dreßler. All rights reserved.
//

#include <stdio.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <iostream>

using namespace std;

// Stringende ermitteln und Zurückgeben.
string find_string_end(string line)
{
    unsigned long symbol_position;
    symbol_position = line.find(":",0);
    line.erase(symbol_position);
    return line;
}

int main(int argc, char *argv[]) {
    //Int-Deklaration für Client_Socket Deskripter
    int client_socket;
    
    //server_address vom strukturierten Datentyp sockaddr_in
    struct sockaddr_in server_address;
    
    //Char-Zeichenkette der Länge 255 für Speicher
    char buffer[255];
    
    //String-Deklaration für Ein-und Ausgabe
    string eingabe, ausgabe;
    
    //Bool-Deklaration für verbindung_aktiv
    bool verbindung_aktiv = true;
    
    //Schleife für aktive Verbindungsdauer
    while(verbindung_aktiv) {
      client_socket = socket(AF_INET, SOCK_STREAM, 0);
      if (client_socket < 0) {
        printf("Fehler: öffnen des Client-Sockets fehlgeschlagen!");
        return EXIT_FAILURE;
      }
        
      //Server_Address initialisieren
      server_address.sin_family = AF_INET;
      server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
      server_address.sin_port = htons(50000);
        
      if (connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
          printf("Fehler: Verbindung zum Server konnte nicht hergestellt werden!");
      }
      

        
        
        cout<<"Bitte geben Sie den Namen der Stadt ein,\nfuer die Sie das Wetter wissen moechten."<<endl;
        cout<<"Zur Auswahl stehen\nLeipzig\nStuttgart\nHamburg\nErlangen\nKonstanz"<<endl;
        cout<<"Zum Beenden des Programm druecken Sie bitte 'Q'."<<endl;
        cin>>eingabe;
        
        if(eingabe=="Q")
        {
            cout<<"'Q' wurde gerückt!"<<endl;
            verbindung_aktiv = false;
        }
        else
        {
            eingabe = eingabe +":";
            cout<<eingabe;
            if(send(client_socket,eingabe.c_str(),eingabe.size(),0)== -1)
            {
                cout<<("Fehler beim Senden!\n")<<endl;
            }
            recv(client_socket, buffer, sizeof(buffer),0);
            ausgabe =  buffer;
            cout<< find_string_end(ausgabe) <<endl;
            cout<<"\n"<<endl;
        }
        
        close(client_socket);
        
    }
    return 0;
}
