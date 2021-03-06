//
//  TCPServer.cpp: TCP/IP INET Server.
//  (c) Eric Lecolinet - Telecom ParisTech - 2013.
//  http://www.telecom-paristech.fr/~elc
//

#include "TCPServer.h"
#include <unistd.h>
#include <iostream>
#include <sstream>

#include <list>
#include <algorithm>
#include "Manage.h"

using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Hook for TCPServer::startReadMessages() (see TCPServer::run())

struct TCPServerHook {
  TCPServerHook(TCPServer* _server, Socket* _sock)
    : server(_server), sock(_sock) {}
  TCPServer* server;
  Socket* sock;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

TCPServer::TCPServer() : servsock() {
  pthread_rwlock_init(&lock, NULL);
  manager = new Manage();

  //add some thing into the manager for test
  manager->newVideo("first_group", "test", 12345, "$HOME/Downloads/test.mp4", 54321);
  manager->newPicture("second_group", "picture", 567, "$HOME/Downloads/picture.mp4", 0, 0); 
}

TCPServer::~TCPServer() {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int TCPServer::run(int port)
{
  // lier le ServerSocket a ce port
  int status = servsock.bind(port);

  if (status < 0) {
    cerr << "TCPServer: can't bind on port: "<<port<<" status="<<status<< endl;
    return status;  // valeur negative, voir Socket::bind()
  }

  // ignorer le signal SIGPIPE qui avorterait le serveur en cas d'erreur reseau
  servsock.ignoreSigPipe();

  while (true) {
    Socket* sock = servsock.accept();

    if (sock == NULL) {
      cerr << "TCPServer: accept() failed" << endl;
      continue;  // continue va a la prochaine iteration de while()
    }

    // lancer la lecture des messages de ce socket dans un thread.
    // pthread_create() cree un thread qui appelle la methode statique
    // startReadMessages() avec un argument de type (void*) qui est en fait
    // un TCPServerHook*. Ceci permet a startReadMessages() d'appeller la
    // methode d'instance readMessages() avec les argument adequats.

    TCPServerHook* hook = new TCPServerHook(this, sock);
    pthread_t tid;

    int status = pthread_create(&tid, NULL, startReadMessages, hook);
    if (status < 0)
      cerr << "TCPServer: pthread_create() failed" << endl;
  }

  pthread_exit(NULL);
  return 0;  // OK
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// called by pthread_create()

void* TCPServer::startReadMessages(void* _hook)
{
  TCPServerHook* hook = static_cast<TCPServerHook*>(_hook);
  hook->server->readMessages(hook->sock);
  delete hook;
  return NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TCPServer::readMessages(Socket* sock)
{
  SocketBuffer sockbuf(sock);

  while (true) {
    string message, response;

    // lire les données envoyées par le client
    // SocketBuffer::readLine() lit jusqu'au premier '\n' (qui est supprimé)
    ssize_t received = sockbuf.readLine(message);

    if (received < 0) {
      closeSocketAndThread(sock, "read error");
      continue;
    }

    if (received == 0) {
      closeSocketAndThread(sock, "connection closed by client");
      continue;
    }

    // traiter le message et retourner la reponse
    // ferme la connection si la valeur de retour est false
    if (! processMessage(message, response)) {
      closeSocketAndThread(sock, "closing connection with client");
      continue;
    }

    // toujours envoyer une reponse au client (sinon il va se bloquer)
    // SocketBuffer::writeLine() envoie tout et rajoute '\n'
    ssize_t sent = sockbuf.writeLine(response);

    if (sent < 0) {
      closeSocketAndThread(sock,"write error");
      continue;
    }

    if (sent == 0) {
      closeSocketAndThread(sock, "connection closed by client");
      continue;
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
 * deux commandes:
 *  - chercher [File ou Group] [name]
 *  - play [name]
 *   
 * ne pas oublier de convertir en lower case pour etre moins rigide...
 */

void TCPServer::closeSocketAndThread(Socket* sock, const char* msg)
{
  cerr << "TCPServer: " << msg << " on socket: "<< sock << endl;
  delete sock;          // detruit le socket (NB: le destr fait sock.close())
  pthread_exit(NULL);   // termine le thread
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Traite le message 'message' et retourne la reponse 'response' ainsi qu'un
// booleen (par return) qui indique si la connection avec le client doit se
// poursuivre (true) ou etre fermée (false).
//
// Cette fonction peut etre executee en parallele par plusieurs threads sauf si
// le verrou 'lock' est bloqué en mode WRITE

bool TCPServer::processMessage(const string& message, string& response)
{
  // cette variable indique si la commande modifie les donnees du programme
  // par defaut on suppose que non
  bool change_data = false;

  // supposons que la commande "deletePhotos" modifie les donnees
  if (message == "deletePhotos") change_data = true;

  // suivant le cas, bloquer le verrou en mode WRITE ou en mode READ
  if (change_data)
    pthread_rwlock_wrlock(&lock);  // bloque en mode WRITE
  else
    pthread_rwlock_rdlock(&lock);  // bloque en mode READ

  /****************************************************************************/
  // debut de l'implementation d'un "mini-langage" de communication seveur/client
  
  string word;
  list<string> wordsToAnalyse = list<string>(); 
  stringstream ss(message);
  while(getline(ss, word, ' ') ) {
    wordsToAnalyse.push_back(word);
  }
  
  /*
   * parsing the words sent to the server.
   * first -> search or play?
   * first - bis -> if search, file or group?
   * second -> in any case, search for file/group using its name.
   */
  if( wordsToAnalyse.size() > 1) {
    list<string>::iterator wordIterator = wordsToAnalyse.begin();
    string wordToCompare = (*wordIterator);
    transform(wordToCompare.begin(), wordToCompare.end(), wordToCompare.begin(), ::tolower);
    if( wordToCompare == "search" ) {
      wordIterator++;
      wordToCompare = (*wordIterator);
      transform(wordToCompare.begin(), wordToCompare.end(), wordToCompare.begin(), ::tolower);

      if (wordToCompare == "file") {
        wordIterator++;
        wordToCompare = (*wordIterator);
        intrusive_ptr<File> file = manager->searchFile(wordToCompare);
        if(file != NULL) {
          response = file->printAttr();
        } else {
          response = "No file with this name";
        }
      } else if (wordToCompare == "group") {
        wordIterator++;
        wordToCompare = (*wordIterator);
        intrusive_ptr<Group> group  = manager->searchGroup(wordToCompare); 
        if(group != NULL) {
          response = group->printGroup();
        } else {
          response = "No group at this name";
        }
      }
    } else if (wordToCompare == "play") {
      wordIterator++;
      wordToCompare = (*wordIterator);
      intrusive_ptr<File> file = manager->searchFile(wordToCompare);
      if ( file == NULL ) {
        response = "No file with this name";
      } else {
        file->play();
      }
    } else if (wordToCompare == "reprint" ) {
      response = "pour test";
    } else {
      response = "doesn't work; wrong argument: ";
      response += wordToCompare;
    }
  } else {
    response = "not enough arguments";
  }

  // fin de l'implementation du mini-langage
  /*************************************************************************/

  // debloque le verrou (attention ne pas oublier cette ligne !)
  pthread_rwlock_unlock(&lock);

  // retourner false pour fermer la connexion
  return true;
}
