#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no {
	int valor;
	char naipe;
	struct no *prox;
}noCarta;

noCarta	*monte;
noCarta	*pilhaDeBaixo[7];
noCarta	*pilhaDeCima[4];
noCarta	*atual;

int		linhaEscondida[7];

noCarta	*removeMonte(int);				/* Remove uma carta do monte */
void	criarMonte(void);				/* Aloca o baralho a ser usado no jogo */
void	imprimeMonte(void);				/* Imprime todas as cartas do monte, função de debug */
void	embaralharMonte(void);			/* Embaralha todas as cartas do monte */
void	distribuiCartas(void);			/* Cria a mesa do jogo, distribui as cartas pelas colunas de baixo */
void	imprimeCartas(void);			/* Imprime a mesa do jogo */
void	proximaCarta(void);				/* Mostra a próxima carta do monte de cima */
int		podeMover(noCarta*, noCarta*);	/* Verifica se uma operação de mover cartas é válida */
int		moveBaixoBaixo(int, int, int);
int		moveMonteBaixo(int);
int		naipeCima(char);
int		moveBaixoCima(int);
int		moveMonteCima(void);
int		moveCimaBaixo(char,int);

void main(void)
{
	char opcao,c;
	int i,j,j2;

	srand(time(NULL));

	criarMonte();
	embaralharMonte();
/*	imprimeMonte();    */
	distribuiCartas();

	
	while(1)
	{
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

		imprimeCartas();

		printf("O que você deseja fazer?\na - Ver a próxima carta do monte\n"
		       "b - Mover uma carta de uma pilha a outra\n"
		       "c - Mover uma carta do monte a uma pilha\n"
		       "d - Mover uma carta de uma pilha até a pilha de cima\n"
		       "e - Mover carta do monte para pilha de cima\n"
		       "f - Mover a carta da pilha de cima para a pilha de baixo\n\n");

		scanf(" %c",&opcao);

		switch (opcao)
		{
			case 'A':
			case 'a':
				proximaCarta();
				break;
			case 'B':
			case 'b':
				printf("Digite a linha e coluna da carta que você deseja mover.\n");
				scanf("%d %d",&i,&j);
				printf("Digite a coluna onde você deseja colocar a carta.\n");
				scanf("%d",&j2);
				moveBaixoBaixo(i,j,j2);
				break;
			case 'C':
			case 'c':
				printf("Digite a coluna onde você deseja colocar a carta.\n");
				scanf("%d",&j);
				moveMonteBaixo(j);
				break;
			case 'D':
			case 'd':
				printf("Digite a coluna de onde você deseja tirar a carta.\n");
				scanf("%d",&j);
				moveBaixoCima(j);
				break;
			case 'E':
			case 'e':
				moveMonteCima();
				break;
			case 'F':
			case 'f':
				printf("Digite o naipe [c/e/o/p] e a coluna onde você deseja inserir a carta.\n");
				scanf(" %c %d",&c,&j);
				moveCimaBaixo(c,j);
				break;
			default:
				break;
		}
	}
}

void criarMonte(void)
{
	int      c1,c2;
	noCarta *n = NULL;

	n= malloc(sizeof(noCarta));

	monte=n;

	for(c1=0;c1<4;c1++)
	{
		for(c2=1;c2<=13;c2++)
		{
			n->valor=c2;

			if(c1==0)
				n->naipe='p'; //naipe de paus
			else if(c1==1)
				n->naipe='c'; //naipe de coração
			else if(c1==2)
				n->naipe='e'; //naipe de espada
			else if(c1==3)
				n->naipe='o'; //naipe de ouros

			if(c1!=3 || c2!=13)
			{
				n->prox = malloc(sizeof(noCarta));
				n=n->prox;
			}
			else
				n->prox=NULL;
		}
	}
}

noCarta *removeMonte(int posicao)
{
	noCarta *atual = monte;
	noCarta *anterior = NULL;

	int c;

	for(c=0;c<posicao;c++)
	{
		anterior=atual;
		atual=atual->prox;
	}

	if (atual==monte)
		monte=monte->prox;
	else
		anterior->prox=atual->prox;

	return (atual);
}

void imprimeMonte(void) 
{
	noCarta *atual = monte;

	while(atual!=NULL)
	{
		printf("[%d%c]\n",atual->valor,atual->naipe);
		atual=atual->prox;
	}
}

void embaralharMonte(void) 
{
	noCarta *n = NULL, *novomonte = NULL;
	int      c;

	n=removeMonte(rand()%52);
	novomonte=n;

	for(c=51;c>0;c--)
	{
		n->prox=removeMonte(rand()%c);
		n=n->prox;
	}

	n->prox=NULL;

	monte=novomonte;

}

void distribuiCartas(void) 
{
	noCarta *atual = NULL;
	int      c1,c2;

	for(c1=0;c1<7;c1++)
	{
		pilhaDeBaixo[c1]=monte;

		for(c2=0;c2<=c1;c2++)
			monte=monte->prox;

		atual=pilhaDeBaixo[c1];

		while(atual->prox!=monte)
			atual=atual->prox;

		atual->prox=NULL;
		linhaEscondida[c1]=c1;
	}
}

void imprimeCartas(void) 
{
	noCarta *baixo[7],*cima[4];
	int      c1,c2;

	printf("\n\n\n");

	for(c1=0;c1<4;c1++)
		cima[c1]=pilhaDeCima[c1];

	for(c1=0;c1<7;c1++)
		baixo[c1]=pilhaDeBaixo[c1];

	printf("Monte ->  ");
	if(atual==NULL)
		printf("[   ]");
	else
		printf("[%2d%c]",atual->valor,atual->naipe);

	printf("       ");

	for(c1=0;c1<4;c1++)
	{
		if(cima[c1]==NULL)
			printf("[   ] ");
		else
		{
			while(cima[c1]->prox!=NULL)
				cima[c1]=cima[c1]->prox;
			printf("[%2d%c] ",cima[c1]->valor,cima[c1]->naipe);
		}
	}

	printf("\n\n");
	printf("      0     1     2     3     4     5     6\n\n");

	c1=-1;

	while (baixo[0]!=NULL || baixo[1]!=NULL || baixo[2]!=NULL || baixo[3]!=NULL
           || baixo[4]!=NULL || baixo[5]!=NULL || baixo[6]!=NULL)
	{
		c1++;
		printf("%2d  ",c1);

		for(c2=0;c2<7;c2++)
			if(baixo[c2]!=NULL)
			{
				if(linhaEscondida[c2]<=c1)
					printf("[%2d%c] ",baixo[c2]->valor,baixo[c2]->naipe);
				else
					printf("[???] ");
				baixo[c2]=baixo[c2]->prox;
			}else
				printf("      ");

		printf("\n");
	}
	printf("\n");
}

void proximaCarta(void)
{
	if(atual == NULL)
		atual=monte;
	else
		atual=atual->prox;
}

int podeMover(noCarta *rem,noCarta *add)
{
	if(rem->valor!=(add->valor-1))
		return 0;

	while(rem->prox != NULL)
	{
		if(rem->valor!=(rem->prox->valor+1))
			return 0;
		rem=rem->prox;
	}

	return 1;
}


int moveBaixoBaixo(int i,int j,int j2)
{
	noCarta *add = NULL, *rem = NULL, *rem_ant=NULL;
	int      c;

	add=pilhaDeBaixo[j2];
	rem=pilhaDeBaixo[j];

	if(rem == NULL)
		return 0;

	for(c=0;c<i;c++)
	{
		rem_ant=rem;
		rem=rem->prox;
		if(rem==NULL)
			return 0;
	}

	if(add == NULL)
	{
		if(rem->valor != 13)
			return 0;

		pilhaDeBaixo[j2] = rem;
		if(rem_ant != NULL)
			rem_ant->prox = NULL;
		else
			pilhaDeBaixo[j] = NULL;
	}else{

		while(add->prox != NULL)
			add = add->prox;

		if(podeMover(rem,add) == 0)
			return 0;

		if((add->naipe=='c' && rem->naipe=='o') ||
		   (add->naipe=='o' && rem->naipe=='c') ||
		   (add->naipe=='p' && rem->naipe=='e') ||
		   (add->naipe=='e' && rem->naipe=='p') ||
		   (add->naipe=='c' && rem->naipe=='c') ||
		   (add->naipe=='e' && rem->naipe=='e') ||
		   (add->naipe=='o' && rem->naipe=='o') ||
		   (add->naipe=='p' && rem->naipe=='p'))
			return 0;

		add->prox = rem;
		if(rem_ant != NULL)
			rem_ant->prox = NULL;
		else
			pilhaDeBaixo[j] = NULL;
	}

	linhaEscondida[j]--;

	return 1;
}

int moveMonteBaixo(int j)
{
	noCarta *n1= pilhaDeBaixo[j], *n_ant=monte,*n2 = NULL;
	int      c;

	if(atual==NULL)
		return 0;

	if(n1!=NULL)
	{
		while(n1->prox!=NULL)
			n1=n1->prox;

	printf("\n[%d%c]\n",n1->valor,n1->naipe);

		if(atual->valor!=(n1->valor-1))
			return 0;

		if((n1->naipe=='c' && atual->naipe=='o') ||
		   (n1->naipe=='o' && atual->naipe=='c') ||
		   (n1->naipe=='p' && atual->naipe=='e') ||
		   (n1->naipe=='e' && atual->naipe=='p') ||
		   (n1->naipe=='c' && atual->naipe=='c') ||
		   (n1->naipe=='e' && atual->naipe=='e') ||
		   (n1->naipe=='o' && atual->naipe=='o') ||
		   (n1->naipe=='p' && atual->naipe=='p'))
			return 0;
	}

	if(atual==n_ant)
		n_ant=NULL;
	else
	{
		while(n_ant->prox!=atual)
			n_ant=n_ant->prox;
	}

	if(n1!=NULL){

		n1->prox=atual;
	}
	else
	{
		if(atual->valor!=13)
			return 0;
		pilhaDeBaixo[j]=atual;
	}

	if(n_ant!=NULL)
		n_ant->prox=atual->prox;
	else
		monte=atual->prox;
	n2=atual;
	atual=atual->prox;
	n2->prox=NULL;

	return 1;
}

int naipeCima(char c)
{

	if(c=='c')
		return 0;
	if(c=='e')
		return 1;
	if(c=='o')
		return 2;
	if(c=='p')
		return 3;
	return -1;

}

int moveBaixoCima(int j)
{
	noCarta *n1=pilhaDeBaixo[j], *n2 = NULL,*n1_ant=NULL;
	int      c;

	if(n1==NULL)
		return 0;

	while(n1->prox!=NULL)
	{
		n1_ant=n1;
		n1=n1->prox;
	}

	c=naipeCima(n1->naipe);
	n2=pilhaDeCima[c];

	if(n2==NULL)
	{
		if(n1->valor==1)
		{
			pilhaDeCima[c]=n1;
			if(n1_ant!=NULL)
			{
				n1_ant->prox=NULL;
			}else{
				pilhaDeBaixo[j]=NULL;
			}

			linhaEscondida[j]--;

			return 1;
		}else{
			return 0;
		}
	}else{
		while(n2->prox!=NULL)
			n2=n2->prox;

		if(n2->valor==(n1->valor-1))
		{
			n2->prox=n1;

			if(n1_ant!=NULL)
				n1_ant->prox=NULL;
			else
				pilhaDeBaixo[j]=NULL;

			linhaEscondida[j]--;

			return 1;
		}
	}

	return 0;
}

int moveMonteCima(void)
{
	noCarta *n1=atual, *n2 = NULL, *n1_ant=monte;
	int      c;

	if(n1==NULL)
		return 0;

	if(n1_ant==n1)
		n1_ant=NULL;
	else
		while(n1_ant->prox!=n1)
			n1_ant=n1_ant->prox;

	c=naipeCima(n1->naipe);
	n2=pilhaDeCima[c];

	if(n2==NULL)
	{
		if(n1->valor==1)
		{
			pilhaDeCima[c]=n1;
			atual=atual->prox;

			if(n1_ant!=NULL)
			{
				n1_ant->prox=n1->prox;
				n1->prox=NULL;
			}else{
				monte=n1->prox;
				n1->prox=NULL;
			}

			return 1;
		}else{
			return 0;
		}
	}else{
		while(n2->prox!=NULL)
			n2=n2->prox;

		if(n2->valor!=(n1->valor-1))
			return 0;

		n2->prox=n1;
		atual=atual->prox;

		if(n1_ant!=NULL)
		{
			n1_ant->prox=n1->prox;
			n1->prox=NULL;
		}else{
			monte=n1->prox;
			n1->prox=NULL;
		}

			return 1;
	}

	return 0;
}

int moveCimaBaixo(char c,int j)
{
	noCarta *n1 = NULL,*n1_ant = NULL,*n2 = NULL;

	if(naipeCima(c) == -1)
		return 0;

	n1=pilhaDeCima[naipeCima(c)];
	n2=pilhaDeBaixo[j];

	if(n1 == NULL)
		return 0;

	while(n1->prox != NULL)
	{
		n1_ant=n1;
		n1=n1->prox;
	}

	if(n2 == NULL)
	{
		if(atual->valor!=13)
			return 0;

		pilhaDeBaixo[j]=n1;
		if(n1_ant==NULL)
			pilhaDeCima[naipeCima(c)]=NULL;
		else
			n1_ant->prox=NULL;
	}else{
		while(n2->prox!=NULL)
			n2=n2->prox;

		if(n1->valor!=(n2->valor-1))
			return 0;

		if((n1->naipe=='c' && n2->naipe=='o') ||
		   (n1->naipe=='o' && n2->naipe=='c') ||
		   (n1->naipe=='p' && n2->naipe=='e') ||
		   (n1->naipe=='e' && n2->naipe=='p') ||
		   (n1->naipe=='c' && n2->naipe=='c') ||
		   (n1->naipe=='e' && n2->naipe=='e') ||
		   (n1->naipe=='o' && n2->naipe=='o') ||
		   (n1->naipe=='p' && n2->naipe=='p'))
			return 0;

		n2->prox=n1;

		if(n1_ant==NULL)
			pilhaDeCima[naipeCima(c)]=NULL;
		else
			n1_ant->prox=NULL;
	}

	return 1;
}
