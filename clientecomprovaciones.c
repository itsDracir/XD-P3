/* Protocol: el client envia un número i el servidor retorna el número multiplicat per 2 */
/* SERVIDOR */
 
/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/* Inclusio de fitxers .h per als sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
 
#define MIDA_PAQUET 50
 
int main(int argc, char **argv)
{
 
    if (argc == 2)
    {
        int s;                              /* El socket */
        struct sockaddr_in socket_servidor; /* Dades del socket on escolta el servidor */
        struct sockaddr_in contacte_client;      /* Adreça i port des d'on el client envia el paquet */
        socklen_t contacte_client_mida = sizeof(contacte_client);          /* Longitud de les dades adreça i port */
 
        char paquet[MIDA_PAQUET]; /* Per posar les dades a enviar/rebre */
 
        /* Volem socket d'internet i no orientat a la connexio */
        s = socket(AF_INET, SOCK_DGRAM, 0);
 
        /* Posem les dades del socket del servidor */
        socket_servidor.sin_family = AF_INET;            /* Socket a Internet */
        socket_servidor.sin_addr.s_addr = INADDR_ANY;    /* Qualsevol NIC */
        socket_servidor.sin_port = htons(atoi(argv[1])); /* Port on estarà escoltant el servidor */
 
        /* Enllacem el socket */
        int resultat_bind;
        resultat_bind = bind(s, (struct sockaddr *)&socket_servidor, sizeof(socket_servidor));
 
        if (resultat_bind < 0)
        {
            printf("No s'ha pogut enllaçar el socket\n");
        }
        else
        {
            /* Servidor operatiu! */
            printf("Servidor operatiu al port %d!\n", atoi(argv[1]));
 
            /*nom, numero, */
            char nom[20];
            int numero;
            int torn;
            char dia[10];
            char *semana[50] = {"Dilluns", "Dimarts", "Dimecres", "Dijous", "Divendres", "Dissabte", "Diumenge"};
 
            const char delimiter[2] = ";";  // separador
            char *token;
            int error = 0, encontrado = 0;
 
 
            while (1)
            {
                printf("Esperant petició d'algun client...\n");
                /* Quan es rep un paquet, a adr_client s'hi anota la IP i port des d'on s'envia */
                recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
                printf("Paquet rebut!\n");
                //copia del paquet, strtok modifica.
                recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
                char paquetcp[MIDA_PAQUET];
                strcpy(paquetcp, paquet);
                token = strtok(paquetcp, delimiter);

                while(token != NULL){
                    printf("Nom: %s\n", token);
                    strcpy(nom, token);
                    token = strtok(NULL, delimiter);
                    printf("Numero: %s\n", token);
                    sscanf(token, "%d", &numero);
                    if (numero > 20 || numero < 0){
                        error = 1;
                        break;
                    }
                    token = strtok(NULL, delimiter);
                    printf("Dia: %s\n", token);
                    strcpy(dia, token);
                    for (int i = 0; i < 7; i++){
                        if (strcmp(dia, semana[i]) == 0){
                            encontrado = 1;
                    }}
                    if (encontrado != 1) {
                        error = 2;
                        break;
                    }
                    token = strtok(NULL, delimiter);
                    printf("Torn: %s\n", token);
                    sscanf(token, "%d", &torn);
                    token = strtok(NULL, delimiter);
 
                }
                printf("Error %d", error);
                /* Tractar la petició... */
                sscanf(paquetcp, "%d", &numero);
 
 
                printf("Volen que multipliqui %d per 2...\n", numero);
                sprintf(paquet, "%d\n", numero * 2);
                printf("Càlcul realitzat!\n");
 
                /* Enviem el paquet a l'adreça i port on està esperant el client */
                sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
                printf("Càlcul enviat!\n");
            }
        }
 
        /* Tanquem el socket */
        close(s);
    }
 
    else
    {
        printf("El nombre de paràmetres no és el correcte!\n");
    }
    return 0;
}
