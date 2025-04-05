/**********************************************************************************
 * In questo codice sperimento un po' con l'utilizzo dei thread e dei mutex. 
 *   
 * "Forzo" una race condition imponendo delle attese.
 * In questo modo, mi è possibile sperimentare individuando in maniera ottima 
 * la sezione critica.
 * 
 * Il numero di thread e di incrementi da effettuare può essere
 * stabilito tramite le costanti INCREMENT_NO e THREAD_NO.
 * 
 * Se si vuole sostituire la funzione Sleep() specifica per Windows, la si può 
 * sostituire con la seguente riga di codice.
 * 
 *
 * for (volatile int i = 0; i < 1000000; i++); 
 *
 * 
 * Inoltre, rimuovendo la funzione Sleep(), è possibile osservare come cambi il
 * comportamento dei thread, che tendono a monopolizzare il lavoro.
 * 
 * 
 * Consiglio: metti tempi di Sleep() molto lunghi (10 secondi = 10000 ms) e osserva
 * come aumentare il numero di thread riduca di molto i tempi di attesa.
 * 
 * Un thread su 4 incrementi con sleep(10000) richiede tempo >= 40 secondi
 * Più thread su ===========================  richiede meno tempo
 * 
 * spostare la prossima riga di codice dedicata alla stampa del singolo incremento dentro o fuori
 * la sezione critica, influisce sull'accuratezza dei messaggi d'incremento, ma non su quella del risultato. 
 * Il risultato finale è corretto a priori, ma bisogna valutare se è meglio ridurre la dimensione della 
 * sezione critica, o rendere atomica anche la stampa del messaggio dell'incremento.
 * 
***********************************************************************************/


#include <pthread.h>
#include <stdio.h>
#include <windows.h>
#define INCREMENT_NO 1000
#define THREAD_NO 4
#define MS_FOR_EVERY_OPERATION 2


pthread_mutex_t mutex;
int contatore = 0;
int key = 0;
int threadCounter[THREAD_NO] = {0};


void *incrementatore(void *arg){
    
    while(1){
        Sleep(MS_FOR_EVERY_OPERATION); // SIMULAZIONE DEL TEMPO DI ATTESA DI ALTRE OPERAZIONI

        pthread_mutex_lock(&mutex); // INIZIO SEZIONE CRITICA

        if (contatore >= INCREMENT_NO){ 
            pthread_mutex_unlock(&mutex);
            break; 
        }

        contatore = contatore + 1;
        threadCounter[*(int*)arg]++;
        printf("Contatore: %d, incremento da thread n. %ld \n", contatore, *(int*)arg);  
        pthread_mutex_unlock(&mutex); // FINE SEZIONE CRITICA
      
    }
    return NULL;
}


int main(){
    
    pthread_t threads[THREAD_NO];
    int threads_id[THREAD_NO];
    pthread_mutex_init(&mutex, NULL);


    for(int i=0; i<THREAD_NO; i++){
        threads_id[i] = i;
        pthread_create(&threads[i], NULL, incrementatore, (void *)&threads_id[i]);
    }

    for(int i=0; i<THREAD_NO; i++){
        pthread_join(threads[i], NULL);
    }


    pthread_mutex_destroy(&mutex);
    printf("valore finale: %d\n", contatore);

    for(int i=0; i<THREAD_NO; i++){
        printf("Il thread n. %d ha incrementato %d volte.\n", i, threadCounter[i]);
    }

}
