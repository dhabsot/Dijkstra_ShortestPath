/******************************************************************/
/* Programma per il calcolo del percorso minimo tra sorgente e    */ 
/* destinazione e calcolo del valore minimo, medio e massimo tra  */
/* tutti i percorsi minimi di comunicazione                       */
/******************************************************************/

/*****************************/
/* Inclusione delle librerie */
/*****************************/

#include <stdlib.h>
#include <stdio.h>

/***********************************/
/* Definizione costanti simboliche */
/***********************************/

#define INFINITO 500

/************************/
/* Definizione dei tipi */
/************************/

typedef struct vertice 
{
	int 	valore;
    	struct 	vertice *succ;
}vertice_t;

/**************************/
/* Dichiarazione funzioni */
/**************************/

vertice_t *leggi_lista(FILE *);
int acquisisci_grafo(vertice_t *[]);
int cerca_vertice_distanza_minima(vertice_t  **,
	       		          int []);
void inserisci(vertice_t **,
	       int);
void riduci(int [],
	    vertice_t *,
	    int,
	    int []);
void dijkstra(vertice_t *,
	      vertice_t *[],
	      int [],
	      int,
	      int [],
	      vertice_t *,
	      int,
	      int,
	      int,
	      int *,
	      int *,
	      int *,
	      int,
	      int *);
void inizializza(int,
		 int [],
		 int [],
		 int);
int acquisizione_sorgente_destinazione(int *,
		 		       int,
		 		       int *,
		 		       char *);
void calcolo_max_min(int [],
		     int,
		     int *,
		     int *,
		     int *,
		     int *);
void calcolo_percorso_minimo(int,
			     int,
		             int,
		             int [],
		             int,
		             int []);


/******************************/
/* Definizione delle funzioni */
/******************************/

/* Definizione della funzione main */
int main(void)
{
	/* Dichiarazione variabili */
	vertice_t *testa = NULL,
  	          *p = NULL,
	       	  *G[INFINITO];
	int	  s,
		  destinazione,
		  n,
		  u = 0,
		  v = 0,
		  scelta,
		  *d,
		  *padre,
		  i,
		  somma,
		  minimo,
		  massimo,
		  esito_lettura;
	char	  carattere_non_letto;
	double    media;

	/* Acquisizione grafo e acquisizione del numero di vertici */
	n = acquisisci_grafo(G);
	
	/* Allocazione dinamica array */
	d = (int *)calloc(n,
			  sizeof(int));
	padre = (int *)calloc(n,
			      sizeof(int));
	
	/* Menu' principale con validazione stretta per impedire che l'utente */
	/* possa digitare caratteri e/o valori non validi                     */	
	do
	{
		printf("\nMenu':\n1) Calcolo del percorso piu' breve\n"); 
		printf("2) Calcolo dei valori massimo, minimo e medio di tutti i");
		printf(" percorsi minimi di comunicazione\n3) Esci\n");
		do
		{
			printf("Scelta: ");
			esito_lettura = scanf("%d",
	      	      		              &scelta);
			if (esito_lettura != 1)
				do
					if (scanf("%c",
				      	      &carattere_non_letto) != 1);
				while (carattere_non_letto != '\n');
		}
		while ((esito_lettura != 1) || (scelta < 1) || (scelta > 3));

		switch (scelta)
		{
			case 1:
				/* Acquisizione sorgente e destinazione tramite funzione con */
				/* validazione stretta                                       */
				printf("\nInserisci la centrale di partenza: ");
				s = acquisizione_sorgente_destinazione(&s,
					         		       n,
						 		       &esito_lettura,
						 		       &carattere_non_letto);

				printf("Inserisci la centrale di destinazione: ");
				destinazione = acquisizione_sorgente_destinazione(&destinazione,
						            			  n,
							    			  &esito_lettura,
							    		          &carattere_non_letto);
				
				/* Confronto che verifica se la sorgente e la destinazione coincidono. */
				/* Se questo fosse vero l'algoritmo non viene invocato poiché sarebbe  */
				/* inutile calcolare il percorso  e il costo quando si sa già a priori */
				/* che sono entrambi 0.                                                */ 
				if (s == destinazione)
				{  
					printf("\nLa sorgente e la destinazione coincidono, ");
					printf("il costo per il raggiungimento della centrale e' 0.\n"); 
				}
				else
				{
					/* Invocazione algoritmo di Dijkstra */
					dijkstra(p,
						 G,
						 d,
						 u,
						 padre,
						 testa,
						 s,
						 n,
						 v,
						 &massimo,
						 &minimo,
						 &somma,
						 scelta,
						 &i);
					
					if (d[destinazione] >= INFINITO)
					{
						printf("\nNon e' possibile raggiungere");
						printf(" la centrale %d dalla centrale %d.\n",
						       destinazione,
						       s);
					}
					else
						/* Memorizzazione su array del percorso */
						/* minimo (inverso) e stampa (inversa)  */
						/* con il relativo costo                */
						calcolo_percorso_minimo(s,
									destinazione,
									v,
									padre,
									n,
									d);
				}
       				break;
			case 2:
				/* Inizializzazione variabili  */
				/* ad un valore iniziale       */ 
				i = 0;
				massimo = 0;
				minimo = INFINITO;
				somma = 0;

				/* Invocazione dell'algoritmo di Dijkstra */
				/* n volte facendo scorrere la sorgente   */	
				for (s = 0;
				     (s < n);
				     s++)
					dijkstra(p,
						 G,
						 d,
						 u,
						 padre,
						 testa,
						 s,
						 n,
						 v,
						 &massimo,
						 &minimo,
						 &somma,
						 scelta,
						 &i);
				media = (double)somma / i;
				printf("\nCosto del cammino piu' lungo possibile: %d",
				       massimo);
				printf("\nCosto del cammino piu' breve possibile: %d",
				       minimo);
				printf("\nCosto del cammino medio: %.2lf\n",
				       media);
				break;
		}
	}
	while ((scelta == 1) || (scelta == 2));
	
	printf("\nArrivederci!\n\n");
     	return(0);
}


/* Definizione della funzione per l'acquisizione del grafo e memorizzazione */
/* mediante n liste di adiacenza                                            */ 
int acquisisci_grafo(vertice_t *G[])
{
	/* Dichiarazione variabili locali alla funzione */
  	int 	  i,
	    	  n,
		  j,
		  vert,
		  nr_nodi_adiacenti;
	vertice_t *p,
		  *testa;
	FILE 	  *input_centrali;
	
	/* Apertura file in sola lettura */
	input_centrali = fopen("input_centrali.txt",
			       "r");
	/* Acquisizione numero vertici grafo dal file */
	if (fscanf(input_centrali,
	           "%d",
	           &n) != 1);

  	for (i = 0;
	     (i < n);
	     i++) 
	{			
		/* Acquisizione del numero di vertici adiacenti al vertice i dal file */
		if (fscanf(input_centrali,
	       	           "%d",
			   &nr_nodi_adiacenti) != 1);
    		testa = NULL;
    		for (j = 0;
		     (j < nr_nodi_adiacenti);
		     j++) 
    		{
			p = malloc(sizeof(vertice_t));
			/* Acquisizione vertice i e vertice adiacente al vertice i dal file */
			if (fscanf(input_centrali,
		       		   "%d %d",
		       		   &vert,
		       		   &p->valore) != 1);
			p->succ = testa;
			testa = p;
    		}
		G[i] = testa;
	}
	/* Chiusura file */
	fclose(input_centrali);
  	return(n);
}


/* Definizione della funzione che implementa l'algoritmo di Dijkstra */
void dijkstra(vertice_t *p,
	      vertice_t *G[],
	      int     	d[],
	      int     	u,
	      int     	padre[],
	      vertice_t *testa,
	      int 	s,
	      int 	n,
	      int 	v,
	      int 	*massimo,
	      int 	*minimo,
	      int 	*somma,
	      int	scelta,
	      int 	*i)
{
	/* Inizializzazione distanze e padri */
	inizializza(v,
		    d,
		    padre,
		    n);
	
	/* Impostazione della distanza dalla sorgente = 0 */
  	d[s] = 0;
	
	/* Inserimento degli elementi nella lista */
  	for (v = 0;
	     (v < n);
	     v++)
    		inserisci(&testa,
		          v);

  	while (testa != NULL)
	{
		u = cerca_vertice_distanza_minima(&testa,
						  d);
		for (p = G[u];
		     (p != NULL);
		     p = p->succ)
			/* Rilassamento archi */
			riduci(d,
			       p,
			       u,
			       padre);
   	}
   	/* Confronto che verifica se è stata scelta l'opzione 2 dal menu' */
	/* principale. Se è stata scelta vengono calcolati il massimo, il */
	/* minimo e il valore medio tra tutte le distanze.                */ 
   	if (scelta == 2)
		for (v = 0;
		     (v < n);
		     v++)
			calcolo_max_min(d,
					v,
					massimo,
					minimo,
					somma,
					i);
}


/* Definizione della funzioni per l'estrazione del vertice u a distanza minima */
int cerca_vertice_distanza_minima(vertice_t  **testa,
	       			  int 	     d[]) 
{
	int u;
	vertice_t *precedente = NULL,
	          *minimo_precedente = NULL,
		  *p,
	          *minimo_p;

	for (p = minimo_p = *testa;
	     (p != NULL);
	     precedente = p, p = p->succ)
		if(d[p->valore] < d[minimo_p->valore])
		{
			minimo_p = p;
			minimo_precedente = precedente;
		}
  	u = minimo_p->valore;
	
	if (minimo_precedente == NULL)
		*testa = (*testa)->succ;
  	else
    		minimo_precedente->succ = minimo_p->succ;
	free(minimo_p);
	return(u);
}


/*  Definizione funzione per l'inserimento di un elemento nella lista */
void inserisci(vertice_t **testa,
	       int	 v)
{
  	vertice_t  *p;
	
  	p = malloc(sizeof(vertice_t));
  	p->valore = v;
  	p->succ = *testa;
  	*testa = p;
}


/* Definizione funzione per il rilassamento degli archi */
void riduci(int       d[],
	    vertice_t *p,
	    int       u,
	    int       padre[])
{
	if (d[p->valore] > d[u] + 1)
	{
		padre[p->valore] = u;
		d[p->valore] = d[u] + 1;
	}
}

/* Definizione della funzione che Inizializza le distanze e i padri a infinito */
void inizializza(int v,
		 int d[],
		 int padre[],
		 int n)
{
	for (v = 0;
	     (v < n);
	     v++)
	{
    		d[v] = INFINITO;
		padre[v] = -1;
	}
}

/* Definizione della funzione per acquisire la sorgente e la */
/* destinazione con la validazione stretta                   */ 
int acquisizione_sorgente_destinazione(int  *val,
		 		       int  n,
		 	               int  *esito_lettura,
		 		       char *carattere_non_letto)
{
	do
	{
		*esito_lettura = scanf("%d",
  	 	      	      	       val);
		if (*esito_lettura != 1)
			do
				if (scanf("%c",
			     		  carattere_non_letto) != 1);
			while (*carattere_non_letto != '\n');
	}
	while ((*esito_lettura != 1) || (*val < 0) || (*val > n - 1));
	
	return(*val);
}

/* Definizione della funzione per il calcolo del valore massimo, */
/* minimo e medio. 						 */
void calcolo_max_min(int d[],
		     int v,
		     int *massimo,
		     int *minimo,
		     int *somma,
		     int *i)
{
	if ((*massimo < d[v]) && (d[v] < INFINITO))
		*massimo = d[v];
	if ((*minimo > d[v]) && (d[v] < INFINITO))
		*minimo = d[v];
	if ((d[v] < INFINITO) && (d[v] > 0))
	{
		*somma += d[v];
		*i = *i +1;
	}
}

/* Definizione della funzione per il calcolo del percorso minimo */
void calcolo_percorso_minimo(int s,
			     int destinazione,
		             int v,
		             int padre[],
		             int n,
		             int d[])
{
	/* Dichiarazione variabili locali alla funzione */
	int *percorso_inv,
	    var_temporanea;
	    
	percorso_inv = (int *)calloc(n,
				     sizeof(int));
	
	var_temporanea = destinazione;
	for (v = 0;
	     (v < d[destinazione]);
	     v++)
	{
		var_temporanea = padre[var_temporanea];
		percorso_inv[v] = var_temporanea;
	}
	printf("\nPercorso piu' breve tra la centrale ");
	printf("%d e la centrale %d:\n",
	       s, 
	       destinazione); 
	for (v = v-1;
	     (v >= 0);
	     v--)
		printf("centrale %d, ",
		       percorso_inv[v]);
	printf("centrale %d, con il costo di %d.\n",
	       destinazione,
	       d[destinazione]);
}
