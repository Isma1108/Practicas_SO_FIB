#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

void error_y_exit(char *msg,int exit_status)
{
    perror(msg);
    exit(exit_status);
}

int segundos = 0;

void funcion_alarma(int s)
{
    char buff[256];
    if (s == SIGALRM) {
        segundos += 10;
    }
    else if (s == SIGUSR1) {
        sprintf(buff, "SIGUSR1: reiniciando alarma -> 10 s\n");
        write(1, buff, strlen(buff));
    }
    
}
int main (int argc,char * argv[])
{
    struct sigaction sa;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK,&mask, NULL);

    /* REPROGRAMAMOS EL SIGNAL SIGALRM */
    sa.sa_handler = &funcion_alarma;
    sa.sa_flags = SA_RESTART; 
    sigfillset(&sa.sa_mask);

    if (sigaction(SIGALRM, &sa, NULL) < 0) error_y_exit("sigaction", 1);
    if (sigaction(SIGUSR1, &sa, NULL) < 0) error_y_exit("sigaction", 1);
    
    while (segundos<100)
    {
    	alarm(10); 	/* Programamos la alarma para dentro de 10 segundos */
    		  	/* Nos bloqueamos a esperar que nos llegue un evento */
    	sigfillset(&mask);
      sigdelset(&mask, SIGALRM);
    	sigdelset(&mask, SIGINT);
      sigdelset(&mask, SIGUSR1);
      sigsuspend(&mask);
    }
    exit(1);
}
