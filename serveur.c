#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <mariadb/mysql.h>
#include <mariadb/my_global.h>


int main()
{

//structure donnant les informations sur le serveur et sur le client

struct sockaddr_in information_server;
struct sockaddr_in information_client;

int socketID = socket(AF_INET, SOCK_STREAM, 0);
int connexion = 0;
int pid;
int id=0;
char msg[4000];
char msg2[4000];

char node[255];
char machine[255];
char os[255];
char date[255];
char ip[255];
char dns[255];
char mac[255];

id=0;
socklen_t len = sizeof(struct sockaddr_in); //déclaration d' une variable du type socklen_t qui contiendra la taille de la structure

if (socketID == -1)
{
    perror("socket");
    exit(-1);
}

/*initialisation du protocole, TCP  adresse 127.0.0.1 et port du serveur 2500 */

memset(&information_server, 0, sizeof(struct sockaddr_in));
information_server.sin_port = htons(2500);
information_server.sin_family = AF_INET;
information_server.sin_addr.s_addr=inet_addr("IP DU SERVEUR");

/* création de la connexion*/

if ((bind(socketID, (struct sockaddr *) &information_server, sizeof(struct sockaddr))) == -1)
{
    perror("bind");
    exit(-1);
}

/* le serveur écoute si un client cherche à se connecter*/

if ((listen(socketID, 5)) == -1)
{
    perror("listen");
    exit (-1);
}
while (1)
{
    memset(&information_client, 0, sizeof(struct sockaddr_in));
    connexion = accept(socketID, (struct sockaddr *) &information_client, &len); 
    //le serveur accepte la connexion

    if (connexion == -1)
    {
        perror("accept");
        exit(-1);
    }
    id+=1;

    /* Creation du processus fils pour répondre  */

    pid = fork();

    if (pid < 0)
    {
        perror("ERROR on fork");exit(1);
    }
    if (pid == 0)
    {

        /* je suis dans le processus fils  */

        close(socketID);

        // printf ("Connexion acceptée de : client %i\n",id);
        

        // le serveur -fils - envoie le message bien venu au client accompagné du numéro du client
        memset(msg, 0, 255);
        sprintf(msg,"client %i",id);
        send(connexion, msg, strlen(msg), 0);

        
        memset(msg, 0, 255);
        recv(connexion, msg, 255*12, 0);
        printf ("client %d : %s\n",id,msg);

        strcpy(msg2, msg);
        
        // Trouver le nom

        // La définitions de séparateurs connus.
        const char * separators = ";";

        // On cherche à récupérer, un à un, tous les mots (token) de la phrase
        // et on commence par le premier.
        char * strToken = strtok ( msg2, separators );
        
        strcpy(node, strToken);
        strToken = strtok ( NULL, separators );

        strcpy(machine, strToken);
        strToken = strtok ( NULL, separators );

        strcpy(os, strToken);
        strToken = strtok ( NULL, separators );

        strcpy(date, strToken);
        strToken = strtok ( NULL, separators );

        strcpy(ip, strToken);
        strToken = strtok ( NULL, separators );

        strcpy(dns, strToken);
        strToken = strtok ( NULL, separators );

        strcpy(mac, strToken);

        // Info Mysql

        MYSQL *con;
        con = mysql_init(NULL);
        char sql_statement[2048];

        if( mysql_real_connect(con, "IP MYSQL", "UTILISATEUR", "MOT DE PASSE", "BASE", 3306, NULL, 0) == NULL)
        {
            printf("The authentication failed with the following message:\n");
            printf("%s\n", mysql_error(con));
            exit(1);
        }

        // Construct the SQL statement
        sprintf(sql_statement, "INSERT INTO SERVER(info, nom, machine, os, datesys, ip, mac, dns) VALUES('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",  msg, node, machine, os, date, ip, mac, dns);
        if(mysql_query(con, sql_statement) !=0)
        {
            printf("Query failed  with the following message:\n");
            printf("%s\n", mysql_error(con));
            exit(1);
        }

        printf("Inserted into the database\n");

        mysql_close(con);


        //si le client ecrit bye, il est deconnecté 
        
        shutdown(socketID, SHUT_RDWR);
        close (connexion);
        exit (0);
    }
    else
    {
        close(connexion);
    }
}
return 0;

}
