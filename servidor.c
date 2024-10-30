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

            int numero; /* Pel desar el número (protocol) */
            char resposta; /* Per desar la resposta */

            while (1)
            {
                recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);                
                sscanf(paquet, "%c", &resposta);
                printf("Resposta del client: %c\n", resposta);

                
                if(resposta == 'N'){
                    printf("Client ha cancel·lat la reserva\n");
                }
                printf("Esperant petició d'algun client...\n");
                /* Quan es rep un paquet, a adr_client s'hi anota la IP i port des d'on s'envia */
                recvfrom(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_client, &contacte_client_mida);
                printf("Paquet rebut!\n");

                /* Tractar la petició... */
                sscanf(paquet, "%d", &numero);

                printf("Volen que multipliqui %d per 2... Ho guardem en el fitxer FServer\n", numero);
                FILE *f = fopen("FServer", "a");
                fprintf(f, "Resultat: %d\n", numero * 2);
                fclose(f);
                printf("Càlcul realitzat i guardat en un fitxer!\n");

                while (fgets(paquet, MIDA_PAQUET, f)){  
                sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
                }
                fclose(f);
                strcpy(paquet, "EOF");
                sendto(s, paquet, strlen(paquet), 0, (struct sockaddr *)&contacte_client, contacte_client_mida);
                printf("Fitxer enviat!\n");
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
