#include"stdio.h"    
#include"stdlib.h"    
#include"sys/types.h"    
#include"sys/socket.h"    
#include"string.h"    
#include"netinet/in.h"    
#include"netdb.h"  
#include"pthread.h"  
#include "wrsock.h"    
#define BUF_SIZE 2000   

// Fonction recevoir le message du serveur 
//****************************************
void * receiveMessage(void * socket) {  
 int sockfd, ret;  
 char buffer[BUF_SIZE];   
 sockfd = (int) socket;  
 memset(buffer, 0, BUF_SIZE);    
 for (;;) {  
  ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);    
  if (ret < 0) {    
   printf("Error receiving data!\n");      
  } else {  
   printf("server: ");  
   fputs(buffer, stdout);  
    }    
 }  
}  

// Debut du program Chat
//**********************
int main(int argc, char**argv) {    
 struct sockaddr_in *addr, cl_addr;   
 int sockfd, ret;    
 char buffer[BUF_SIZE];   
 int port;  
 pthread_t rThread;  
  
 if (argc < 3) {  
  printf("Utilisation: ./client <non du serveur> <numero port>\n");  
  exit(1);    
 }  
// Creation du socket  
//*******************
 sockfd = socket(AF_INET, SOCK_STREAM, 0);    
 if (sockfd < 0) {    
  printf("Error de creation du socket!\n");    
  exit(1);    
 }    
 printf("Socket cree...\n");     
// creation de l address du socket
//********************************
  port = atoi(argv[2]);
  memset(&addr, 0, sizeof(addr));
 addr = (struct sockaddr_in *) CreerSockAddr (argv[1], port);   
//Connection au serveur
//*********************
 ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));    
  printf("Connecte au serveur...\n");    
  
 memset(buffer, 0, BUF_SIZE);  
 printf("Tapez votre message!\n");  
  
 //la création d'un nouveau thread pour recevoir des messages à partir du serveur.
 //*******************************************************************************
 ret = pthread_create(&rThread, NULL, receiveMessage, (void *) sockfd);  
 if (ret) {  
  printf("ERROR: RCode de retour à partir de pthread_create()  %d\n", ret);  
  exit(1);  
 }  
  
 while (fgets(buffer, BUF_SIZE, stdin) != NULL) {  
  ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));    
  if (ret < 0) {    
   printf("Error sending data!\n\t-%s", buffer);    
  }  
 }  
  
 close(sockfd);  
 pthread_exit(NULL);  
   
 return 0;      
}    
