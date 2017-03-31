#define CENTRE 0  // Pate des photodiodes
#define AXE1 1
#define AXE2 2
#define AXE3 3

#define X_POS1 6    // Position des photodiodes
#define X_POS2 3
#define Y_POS1 6

#define SENSIBILITE 0.5 // sensibilité de la cible. 0 < SENSIBILITE < 1; Le plus haute le plus selectif

#define SCORE_MINIMAL 30
 
void setup(void) {
  Serial.begin(9600);

}
 
void loop(void) {
  byte percent = 0;
  
  percent = getScore();

  Serial.print(percent); // on change apres pour envoyer au xBee
  
  delay(100);          // ajuster
}


byte getScore()
{
  unsigned int photodiode[4];    // declare le vecteur de ints qui auront les valeurs du analogread
  unsigned int ptd_insig, ptd_sum;                              // le numero de la photodiode moins fort
  unsigned int x, y;
  byte indice_insig, percent;
  float xcm, ycm;


  // ------ Obtention des valeurs -----
  for(int i=0; i<4; i++)
    photodiode[i] = analogRead(i);  // On prends les valeurs du analogRead sur 1024 <-> (int plus rapide que float)

  
  // ------ Check photodiode avec valeur plus faible -----

  ptd_insig = photodiode[0];                                                                   
  
  for (byte i=1; i <= 3; i++)
  {
    if ( ptd_insig > photodiode[i] )
    {
      ptd_insig = photodiode[i];
      indice_insig = i;
    }
    ptd_sum += photodiode[i];
  }

  // -------- Check tir ------ //
  if ( photodiode[indice_insig] < SENSIBILITE*( ptd_sum / 4) )
  {
  // --------- Delete l'element insignificatif -----
    ptd_sum = ptd_sum - photodiode[indice_insig];     
    photodiode[indice_insig] = 0;

  // --------- Determine les Xcm et Ycm
    xcm = ( photodiode[1]*X_POS1 + X_POS2*(photodiode[2] + photodiode[3]) ) / ( ptd_sum );
    ycm = ( Y_POS1*(photodiode[2] + photodiode[3]) ) / ( ptd_sum );

  // --------- float = slow; on le transform en int pour faire les manips
    x = (int)((xcm*10)+0.5);
    y = (int)((ycm*10)+0.5);
    
  // --------- On trouve la distance du centre de masse à l'origine, et on trouve le score correspondant a cetter distance    
    return (byte)map( sqrt( x^2 + y^2 ) , 85, 0, SCORE_MINIMAL, 100);    // notez que sqrt(x2+y2) = distance
  }

  else  // si on ne passe pas  le check de tension
    return 0;
  
}


