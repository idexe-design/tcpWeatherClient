#include <stdio.h>          //Ein-/Ausgabesystem von ANSI-C
#include <sys/types.h>      //Deklariert Typ time_t für Zeitfunktionen
#include <arpa/inet.h>      //Definiert Internetfunktionen, z.B. in_addr
#include <sys/socket.h>     //Einbinden der Socket APIQ
#include <netinet/in.h>     //Stellt Internet-Protokollfamilie bereit
#include <unistd.h>         //Standard für symbolische Konstanten und Typen (Textersetzung beim Compalieren)
#include <iostream>         //Ein-/Ausgabesystem auf objektorientierter Klassenhierarchie (Erweitert stdio)

using namespace std;        //Im Gültigkeitsbereich der Datei wird der Namensbereich "std" durch using zur
                            //Verfügung gestellt. Damit wird z.B. aus std::cout nur cout

//Funktion: "find_line_end", Rückgabewert: String
// Zum Bestimmen des String-Endes. Da Eingebene Städtenamen mit ":" versehen werden, um das Eingabeende zu ermitteln.
string find_line_end(string line)
{
    //Deklaration: Variable: "position", Datentyp: unsigned long [bzgl. find Rückgabetyp von find()]
    unsigned long position;
    //Zuweisung: Variable "position" wird Position des ":"-Zeichens aus der Eingabe Zeile zugewiesen.
    position = line.find(":",0);
    //String-Funktion erase: Löscht alle Zeichen des Strings "line" ab Position.
    line.erase(position);
    //Rückgabe: Gibt String "line" zurück. Enthält alle Zeichen bis ausschließlich ":".
    return line;
}

int main(int argc, char *argv[]) {      //Hauptfunktion :: Dem Programm können Parameter übergeben werden
                                        //argc ist die Anzahl und argv entspricht den Werten der Parameter
    
    //Deklaration: Variable "client_socket", Datentyp: integer, wird für die Socket-Deskriptoren genutzt.
    int client_socket;
    
    //Deklaration: Variablen "server_address", Datentyp: (strukturierten Datentyp) sockaddr_in (netinet/in.h), wird für die IP-Adressen genutzt.
    struct sockaddr_in server_address;
    
    //Deklaration: "buffer", Datentyp: char, Speicher für "recv"-Empfang als Zeichenkette 255 Zeichen.
    char buffer[255];
    
    ///Deklaration: Variablen "input", "output", Datentyp: string, werden für Ein-und Ausgaben genutzt.
    string input, output;
    
    //Deklaration "verbindung_aktiv", Datentyp: bool, für die Programmlaufzeit auf "true" gesetzt
    bool verbindung_aktiv = true;
    
    //Schleife für aktive Verbindungsdauer, von Variable "verbindung_aktiv" abhängig.
    while(verbindung_aktiv) {
      //client_socket ist Deskriptor eines Sockets
      //Protokolfamilie: AF_INET(netinet/in.h) stellt TCP bei verbindungsorieten Stream-Transfers, UDP bei verbindungslosen Stream-Transfers bereit
      //Type: SOCK_STREAM legt verbindungsorieten Stream-Fest
      //Protocol: 0 bedeutet Standardprotokoll benutzen
      client_socket = socket(AF_INET, SOCK_STREAM, 0);
        
      //Überprüfung ob Socket geöffnet werden kann, sonst Fehlerausgabe
      if (client_socket < 0) {
        printf("Fehler: öffnen des Client-Sockets fehlgeschlagen!");
        return EXIT_FAILURE;
      }
        
      //Server_Address initialisieren
      //Variable server_address vom strukturierten Typ sockaddr_in (netinet/in.h) wird nun initialisiert
        server_address.sin_family = AF_INET;    //Protokolfamilie TCP/UDP
      //Serveradresse sin_addr von strukturierten Datentyp in_addr (netinet/in.h) :: Loopback-Adresse bewirkt, dass jede der eigene Rechner angesprochen werden soll.
      server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
      //Serverport: 50000
      //htons (Host To Network Short) gibt Nummer in Network Byte Order zurück
      server_address.sin_port = htons(50000);
        
      //Die Funktion "connect()" versucht eine für den Client-Socket eine Verbindung zum Server aufzubauen
      //Ausgabe einer Fehlermeldung, falls Verbindung fehlschlägt.
      if (connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
          printf("Fehler: Verbindung zum Server konnte nicht hergestellt werden!");
      }
      

        
        //Anzeige der Anwendungsinformationen zur Handhabung des Client-Programms
        cout<<"Bitte geben Sie den Namen der Stadt ein,\nfuer die Sie das Wetter wissen moechten."<<endl;
        cout<<"Zur Auswahl stehen\nLeipzig\nStuttgart\nHamburg\nErlangen\nKonstanz"<<endl;
        cout<<"Zum Beenden des Programm druecken Sie bitte 'Q'."<<endl;
        cin>>input;
        
        //Eingabe von "Q" führt zum Abbruch des porgramm, sonst Wetter-Abfrage und anschließende Schleifenwiederholung
        if(input=="Q")
        {
            verbindung_aktiv = false;
        }
        else
        {
            //An den Input wird ein ":" als Zeichen des Zeilenendes angehängt.
            input = input +":";
            //Die vollständige Eingabe wird in den Ausgabestream geschoben.
            cout<<input;
            //Der Client versucht über die Verbindung (client_socket) die Eingabe an den Server zu senden
            //die string Ausgabe wird mit "c.str()" in ein Feld geschrieben und liefert einen Zeiger auf das Feld zurück. Hängt eine 0 am Ende an (als C-String-Endzeichen)
            //Es werden mit 0-Bits für Optionen angehangen, also werden keine Flags o.ä. übertragen
            //Fehlermeldung, falls "send()" fehlschlägt.
            if(send(client_socket,input.c_str(),input.size(),0)== -1)
            {
                cout<<("Fehler beim Senden!\n")<<endl;
            }
            //Client erhält Wetterdaten als Antwort vom Server und schreib diese in den "buffer".
            recv(client_socket, buffer, sizeof(buffer),0);
            
            //Ausgabe Variable mit dem "buffer"-Inhalt besetzt.
            output =  buffer;
            //Es wird die Empfangene Zeile ohne ":" in den Ausgabestream geschoben und an der Konsole ausgegeben
            cout<< find_line_end(output) <<endl;
            //Leerzeichen wird in den Ausgabestrom geschoben und an der Konsole ausgegeben.
            cout<<"\n"<<endl;
        }
        //Die Verbindung zum Server wird beendet.
        close(client_socket);
        
    }
    //Rückgabe, dass Programm natürlich (nicht wegen eines Fehlers) beendet wurde.
    return EXIT_SUCCESS;
}

