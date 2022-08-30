#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>

#include <time.h>
#include <locale.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

enum {MAX_SIZE = 256};

char buf [2000];
struct informations{
    char node[255];
    char machine[255];
    char os[255];
    char date[255];
    char ip[255];
    char dns[255];
    char mac[255];
}info;



int datatosend(void)
{

/* Var systeme */

    struct utsname uts;
    if (uname(&uts) == -1)
        perror("uname");

/* Var heure */

    char d[100];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(d, sizeof(d)-1,"%Y-%m-%d %H:%M:%S", tm);

/* Adresse IP */
FILE* fip;
char ip[255];
system("/bin/ip a s | grep 'inet ' | tr '\n' ' ' | sed -e 's/     /,/g' | sed -e 's/    //g' > tmp.ip");
fip = fopen("tmp.ip", "r");
fgets(ip, 255, fip);

FILE* fdns;
char dns[255];
system("cat /etc/resolv.conf | tr '\n' ',' > tmp.dns");
fdns = fopen("tmp.dns", "r");
fgets(dns, 255, fdns);

FILE* fmac;
char mac[255];
system("cat /sys/class/net/*/address | tr '\n' ',' | rev | cut -c2- | rev > tmp.mac");
fmac = fopen("tmp.mac", "r");
fgets(mac, 255, fmac);

FILE* fos;
char os[255];
system("cat /proc/version | tr -d '\n' > tmp.os");
fos = fopen("tmp.os", "r");
fgets(os, 255, fos);

/* Gestion des données */

    strncpy(info.node, uts.nodename,strlen(uts.nodename));
    strcpy(info.machine, uts.machine);
    strcpy(info.os, os);
    strcpy(info.date, d);
    strcpy(info.ip, ip);
    strcpy(info.dns, dns);
    strcpy(info.mac, mac);

fclose(fip);
fclose(fdns);
fclose(fmac);
fclose(fos);

system("rm tmp.*");

return 0;
}

int main()
{
printf("%c", datatosend());

pid_t pid;
int id;
int dial=1;
char msg[255];//variable qui contiendrat les messages

struct sockaddr_in informations;  //structure donnant les informations sur le serveur

/*initialisation du protocole, TCP  adresse : 127.0.0.1 et  port du serveur 2500 */
informations.sin_family = AF_INET;
informations.sin_port = htons(2500);
informations.sin_addr.s_addr = inet_addr("IP DU SERVEUR");
int socketID = socket(AF_INET, SOCK_STREAM, 0); 
// creation du socket propre au client

if (socketID == -1)    
//test si la  création du socket  est impossible alors on arrete
{
    perror("socket");
    exit (-1);
}

if ((connect(socketID, (struct sockaddr *) &informations, sizeof(struct sockaddr_in))) == -1)   
//vérifier que connexion au serveur est possible, sinon on arrete 
{
    perror("connect");
    exit (-1);
}

//lire le message envoyé par le serveur

memset(msg, 0, 255);
recv(socketID, msg, 255, 0);
printf ("nom : %s \n",msg);

id+=1;

// Concaténation des données

strcat(buf,info.node);
strcat(buf,";");
strcat(buf,info.machine);
strcat(buf,";");
strcat(buf,info.os);
strcat(buf,";");
strcat(buf,info.date);
strcat(buf,";");
strcat(buf,info.ip);
strcat(buf,";");
strcat(buf,info.dns);
strcat(buf,";");
strcat(buf,info.mac);
strcat(buf,"\0");



// Envoie du message
if ((send(socketID, buf, 2000, 0)) == -1) perror("send");

shutdown(socketID, SHUT_RDWR);// fermeture du socket

}
