#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> //ricordati!!!
#include <sys/wait.h>  //ricordati!!!
#include <stdlib.h> //ricordati!!!


int main(void) {
    int fd[2]; //ho creato la pipe, cioò il mio tubo che collega due stanze, si mette il due perchè con 0 leggo e con 1 scrivo
    if (pipe(fd) == -1) { //mi serve per controllare se si è fatto un errore nella sua creazione, farlo sempre!!
        perror("pipe");
        return 1;
    }
    pid_t pid = fork(); //faccio nascere il figlio, la seconda stanza
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(fd[1]); // chiudo la parte di tubo del figlio dicendo che non può scrivere

        int regali = 0;
        read(fd[0], &regali, sizeof(regali)); //gli dico che praaticamente deve leggere il numero di regali che il padre gli sta dicendo
        close(fd[0]); // chiudo il tubo di leggere perchè ho finito di leggere

        int scatole = regali / 3; // essendo che una scatola contiene 3 regali faccio questo isi calcolo
        printf("la fabbrica: mi hai chiesto %d regali -> servono %d scatole\n", regali, scatole);

        return 0;
    }else {
        close(fd[0]); // visto che il figlio prima ha letto il padre deve scrivergli il numero di regali, quindi chiude la lettura

        int regali;
        printf("quanti regali devo preparare? ");
        fflush(stdout); //mi serve per buttare subito tutto quello che h in memoria sullo schermo senza priima metterlo nel buffer come fa il printf
        scanf("%d", &regali);

        write(fd[1], &regali, sizeof(regali)); //il padre sta scrivendo al figlio  il numero dei regali
        close(fd[1]); // finito di scrivere

        wait(NULL); // aspetta il figlio
        printf("la fabbrica ha finito\n");
    }
    return 0;
}