#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;
int main( int argc, char *argv[] )
{
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  /* First call to socket() function */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("ERROR opening socket");
    exit(1);
  }
  /* Initialize socket structure */
  memset((char *) &serv_addr, 0, sizeof(serv_addr));
  portno = 5001;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  /* Now bind the host address using bind() call.*/
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
  sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
  }
  /* Now start listening for the clients, here process will
  * go in sleep mode and will wait for the incoming connection
  */
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  /* Accept actual connection from the client */
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
  &clilen);
  if (newsockfd < 0)
  {
    perror("ERROR on accept");
    exit(1);
  }
  string cmd;
  while(1){
  /* If connection is established then start communicating */
      memset(buffer, 0, 256);
      n = read(newsockfd,buffer,255 );
      if (n < 0)
      {
        perror("ERROR reading from socket");
        exit(1);
      }
      printf("Here is the message: %s\n",buffer);
      
      cmd = string(buffer);
      if(cmd.find("turn left")!=std::string::npos){
        turnLeft();
        cout << "left\n";
      }
      if(cmd.find("turn right")!=std::string::npos){
        turnRight(); 
        cout << "right\n";
      }
      if(cmd.find("left")!=std::string::npos){
        string local = string(cmd.c_str() + 5);
	int amount = atoi(local.c_str());
        turnLeft(amount);
        cout << "left\t"<<amount<<"\t"<<local<<"\n";
      }
      if(cmd.find("right")!=std::string::npos){
        string local = string(cmd.c_str() + 6);
	int amount = atoi(local.c_str());
        cout << "right\t"<<amount<<"\t"<<local<<"\n";
        turnRight(amount);
      }
      if(cmd.find("drive")!=std::string::npos){
        cout << "drive\n";
        drive();  
      }
      if(cmd.find("print sensor")!=std::string::npos){
        cout << "print sensor";
        //getSensorData();
      }
      if(cmd.find("goto")!=std::string::npos){
        //int amount_x = atoi(cmd.substr(5, cmd.find(" ")-5));
        //int amount_y = atoi(cmd.substr(5, cmd.find(" ", cmd.find(" "))));
        //cout<<"goto\t"<<amount_x<<"\t"<<amount_y<<"\n";
        //moveTo(x,y);
      }
      if(cmd.find("return")!=std::string::npos){
        //returnToStartPosition();
        cout<<"return"<<"\n";
      }
      if(cmd.find("reset")!=std::string::npos){
        //resetStartPosition();
        cout<<"reset"<<"\n";
      }
      //while(buffer[l++]!='\0');
      //write(tty, buffer, l);
      //printf("buffer len : %d", l);
      /* Write a response to the client */
  }
  return 0;
}
