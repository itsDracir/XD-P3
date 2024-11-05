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
        char resposta; /* Per desar la resposta */
        printf("Benvingut/da vols fer una reserva? (S/N) ");
        scanf("%c", &resposta);
        
        sprintf(paquet, "%c\n", resposta);
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));
                if(resposta == 'N'){
                    printf ("Se ha cancelado la reserva");
                    close(s);
                    return 0;
        }
        char nom[20], dia[10];
        int n, torn;

        /*Seleccio del nom de la reserva*/
        printf("Benvingut/da!\nIntrodueix el seu nom: ");
        scanf("%s",nom);

        /*Seleccio del nombre de persones*/
        printf("Nombre de persones per a la reserva (maxim 20): ");
        scanf ("%d", &n);

        /*Seleccio del dia de la setmana*/
        printf ("Dia de la reserva: ");
        scanf ("%s", dia);

        /*Selecció de torn*/
        printf ("Tria el torn de la reserva:\n");
        printf("1. Torn de migdia (13:00 h a 15:00 h)\n2. Torn de nit (20:00 h a 22:00 h)\n");
        printf("Selecciona el torn (1 o 2): ");
        scanf ("%d", &torn);

        /* Muntem el buffer */
        sprintf(paquet, "%s;%d;%s;%d", nom, n, dia, torn);
        
        /* L'enviem */
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&contacte_servidor, sizeof(contacte_servidor));
        printf("Paquet enviat! Espero resposta...\n");
        FILE *file = fopen("RESULTAT", "w");

        while (1)
        {
            recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL);
            paquet[strcspn(paquet, "\n")] = '\0';  // Remove newline for "EOF" check

            if (strcmp(paquet, "EOF") == 0)
                break;

            fprintf(file, "%s\n", paquet);
        }
        
        printf("Fitxer rebut i guardat com %s\n", "RESULTAT");
        fclose(file);

        /* Tanquem el socket */
        close(s);
    }

    else
    {
        printf("El nombre de paràmetres no és el correcte!\n");
    }

    return 0;
}
