/* Protocol: el client envia un número i el servidor retorna el número multiplicat per 2 */
/* CLIENT */

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

    if (argc == 3)
    {

        int s; /* El socket */
        struct sockaddr_in contacte_servidor; /* Adreça i port on hi ha el servidor */
        
        char paquet[MIDA_PAQUET];   /* Per posar les dades a enviar/rebre */

        /* Volem socket d'internet i no orientat a la connexio */
        s = socket(AF_INET, SOCK_DGRAM, 0);

        contacte_servidor.sin_family = AF_INET;  /* Socket a Internet */
        contacte_servidor.sin_addr.s_addr = inet_addr(argv[1]); /* Adreça on està el servidor */
        contacte_servidor.sin_port = htons(atoi(argv[2])); /* Port on escolta el servidor */

        /* Execucio del protocol */
        /* 1) Demanem un número pel teclat */
        int n;
        printf("Benvingut/da!\nPosa un número ");
        scanf("%d",&n);
        
        /* Muntem el buffer */
        sprintf(paquet, "%d\n", n);
        
        /* L'enviem */
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));
        printf("Paquet enviat! Espero resposta...\n");

        recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL); /* NULL -> No cal saber des d'on ens envien el paquet. */
        printf("He rebut la resposta: %s\n", paquet);

        /* Tanquem el socket */
        close(s);
    }

    else
    {
        printf("El nombre de paràmetres no és el correcte!\n");
    }

    return 0;
}