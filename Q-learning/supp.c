#define Q_INITIAL  25.0
#define CHOOSE_A_ERREUR  2.0
#define POLICY_IMPROVEMENT_ERREUR  0.2

float R = 0;
float gk = 1;

void  afficher();
void  afficher_Q();
void  afficher_Pi();

void  from(int, int);
void  to(int, int);

///////////////////////////////////////////////
int fromQtoPolicy() 
{
  int improvement = 0;
  int a, i, j;

  // printf("fromQtoPolicy: debut:\n");

  for (i=0; i<TAILLE; i++) {
    for (j=0; j<TAILLE; j++) {
      float qm = meilleurQ(i, j);
      int nam = 0;
      float old[N_ACTIONS];
      for (a=0; a<N_ACTIONS; a++) old[a] = Pi[i][j][a];
      for (a=0; a<N_ACTIONS; a++) {
	if (actionEgalQ(a, i, j, qm, POLICY_IMPROVEMENT_ERREUR)) {
	  Pi[i][j][a] = 1.0; nam++;
	}
	else {
	  Pi[i][j][a] = 0.0;
	}
      }
      for (a=0; a<N_ACTIONS; a++) {
	if (actionEgalQ(a, i, j, qm, POLICY_IMPROVEMENT_ERREUR)) Pi[i][j][a] /= nam;
	if (old[a] != Pi[i][j][a]) improvement++;
      }
      
    }
  }
  // printf("fromQtoPolicy: fin.\n");
  return improvement;
}


///////////////////////////////////////////////
void afficher() 
{
  int i, j;
  for (i=0; i<TAILLE; i++) {
    for (j=0; j<TAILLE; j++) {
      switch (grille[j][i]) {
      case VALEUR_A:  printf(" A  "); break;
      case VALEUR_A2: printf(" A' "); break;
      case VALEUR_B:  printf(" B  "); break;
      case VALEUR_B2: printf(" B' "); break;
      case VALEUR_Z:  printf(" X  "); break;
      default:  printf(" .  "); break;
      }
    }
    printf("\n");
  }
  printf("R = %f\n", R);
}


///////////////////////////////////////////////
void afficher_Q() 
{
  int i, j, a;
  for (i=0; i<TAILLE; i++) {
    for (j=0; j<TAILLE; j++) {
      for (a=3;;) {
	printf("%2.0f ", Q[j][i][a]);
	a++; if (a==N_ACTIONS) a = 0;
	if (a==N_ACTIONS-1) break;
      }
      printf("|");
    }
    printf("\n");
  }
}


///////////////////////////////////////////////
// Pi est implicite.
// Elle est calculee avec Q.
///////////////////////////////////////////////
void afficher_Pi() 
{
  int i, j, a;
  for (i=0; i<TAILLE; i++) {
    for (j=0; j<TAILLE; j++) {
      float qm = meilleurQ(j, i);
      printf("  ");
      for (a=3;;) {
	if ((actionEgalQ(a, j, i, qm, POLICY_IMPROVEMENT_ERREUR))) {
	  switch(a) {
	  case NORD:  printf("^"); break;
	  case EST:   printf(">"); break;
	  case SUD:   printf("v"); break;
	  case OUEST: printf("<"); break;
	  }
	}
	else       printf(" ");
	a++; if (a==N_ACTIONS) a = 0;
	if (a==N_ACTIONS-1) break;
      }
      printf("  ");
    }
    printf("\n");
  }
}




///////////////////////////////////////////////
void from(int x, int y) 
{
  if ((x==xA) && (y==yA))        grille[x][y] = VALEUR_A;
  else if ((x==xA2) && (y==yA2)) grille[x][y] = VALEUR_A2;
  else if ((x==xB) && (y==yB))   grille[x][y] = VALEUR_B;
  else if ((x==xB2) && (y==yB2)) grille[x][y] = VALEUR_B2;
  else grille[x][y] = VALEUR_RIEN;
}

///////////////////////////////////////////////
void to(int x, int y) 
{
  grille[x][y] = VALEUR_Z;
}


///////////////////////////////////////////////
// utilise la politique Pi.
///////////////////////////////////////////////
void un_pas_avec_politique(int p) 
{
  int r = 0; int aNext = -1;
  int Xnext = X; int Ynext = Y;

  // printf("un_pas_avec_politique: debut:\n");

  gk *= GAMMA;

  if ((X==xA) && (Y==yA)) { 
    from(xA, yA);  
    R+= 10*gk; 
    r = 10;
    Xnext = xA2; 
    Ynext = yA2;
  }
  else if ((X==xB) && (Y==yB)) { 
    from(xB, yB);  
    R+= 5*gk; 
    r = 5;
    Xnext = xB2; 
    Ynext = yB2; 
  }
  else {
    switch (A) {
    case NORD: {
      if (Y==0) { R -= gk; r = -1; }
      else { from(X, Y); Ynext = Y - 1; }
    } break;
    case EST: {
      if (X==TAILLE-1) { R -= gk; r = -1; }
      else { from(X, Y); Xnext = X + 1; }
    } break;
    case SUD: {
      if (Y==TAILLE-1) { R -= gk; r = -1; }
      else { from(X, Y); Ynext = Y + 1; }
    } break;
    case OUEST: {
      if (X==0) { R -= gk; r = -1; }
      else { from(X, Y); Xnext = X - 1; }
    } break;
    }
  }

  // choix de a' en fonction de s'
  aNext = choose_A(Xnext, Ynext, CHOOSE_A_ERREUR);

  // regle de mise a jour de SARSA:
  // Q[X][Y][A] += ALPHA * (r + GAMMA * Q[Xnext][Ynext][aNext] - Q[X][Y][A]);

  // regle de mise a jour de Q-Learning:
  Q[X][Y][A] += ALPHA * (r + GAMMA * meilleurQ(Xnext, Ynext) - Q[X][Y][A]);

  to(Xnext, Ynext);
  X = Xnext; Y = Ynext; A = aNext;

  nb_pas_total++;

  // printf("un_pas_avec_politique: fin.\n");

}


///////////////////////////////////////////////
int choose_A(int x, int y, float e) 
{
  int a;
  // L'état est celui de la récompense, on tire au hasard une action.
  if (((x==xA) && (y==yA)) || ((x==xB) && (y==yB))) a = rand() % N_ACTIONS;
  else {
    float qm = meilleurQ(x, y);
    int nam = 0;
    //On compte combien d'état/action on une valeur proche du max à 5 près
    for (a=0; a<N_ACTIONS; a++) if (actionEgalQ(a, x, y, qm, e)) nam++;
   
    int h = rand() % nam;
    for (a=0; a<N_ACTIONS; a++) {
      if (actionEgalQ(a, x, y, qm, e)) h--;
      if (h<0) break;
    }
  }
  return a;
}



