#define CENTRE 0  // Pate des photodiodes
#define AXE1 1
#define AXE2 2
#define AXE3 3

#define X_POS1 6.0    // Position des photodiodes
#define X_POS2 3.0
#define Y_POS1 6.0

#define SENSIBILITE 0.5 // sensibilité de la cible. 0 < SENSIBILITE < 1; Le plus haute le plus moins selectif
#define SEUIL 40.0     // define la valeur moyenne minimum pour determiner la pourcentage

#define SCORE_MINIMAL 00.0

void setup(void) {
  Serial.begin(115200);

}

void loop(void)
{
  byte percent = 0;

  unsigned int photodiode[4] = {0, 0, 0, 0}; // declare le vecteur de ints qui auront les valeurs du analogread
  unsigned int ptd_insig;     // le numero de la photodiode moins fort
  float ptd_sum = 0;
  int x = 7, y = 7;
  byte indice_insig = 0;
  float xcm = -1, ycm = -1;

  

  // ------ Obtention des valeurs -----
  for (byte i = 0; i < 4; i++) {
    for (byte j = 0; j < 32; j++)   // on fait la moyenne entre 32 echantillons
    {
      photodiode[i] += analogRead(i);  // On prends les valeurs du analogRead sur 1024 <-> (int plus rapide que float)
    }

    photodiode[i] /= 32;          // on fait la moyenne entre les 32 echantillons
    ptd_sum += photodiode[i];     // on somme tous les valeurs
 
  }

  if ( ptd_sum > SEUIL)   // premier check pour voir si le minimum est attendue
  {

    // ------ Check photodiode avec valeur plus faible -----

    ptd_insig = photodiode[0];

    for (byte i = 1; i <= 3; i++)
    {
      if ( ptd_insig > photodiode[i] )
      {
        ptd_insig = photodiode[i];
        indice_insig = i;
      }
      ptd_sum += photodiode[i];
    }

    // -------- Check tir ------ //
    if ( photodiode[indice_insig] < (int)(SENSIBILITE * ptd_sum / 4.0) )
    {

      // --------- Delete l'element insignificatif ----- //
      ptd_sum = ptd_sum - photodiode[indice_insig];
      photodiode[indice_insig] = 0;

      // --------- Determine les Xcm et Ycm ------------ //
      xcm = ( photodiode[1] * X_POS1 + X_POS2 * (photodiode[2] + photodiode[3]) ) / ( ptd_sum );
      ycm = ( Y_POS1 * (photodiode[2] + photodiode[3]) ) / ( ptd_sum );

      // --------- float = slow; on le transform en int pour faire les manips ------ //

      x = (int)((xcm * 10) + 0.5);
      y = (int)((ycm * 10) + 0.5);

      // --------- On trouve la distance du centre de masse à l'origine, et on trouve le score correspondant a cetter distance

      percent = (byte)map( sqrt( x * x + y * y ) , 68.0, 0, SCORE_MINIMAL, 100); // notez que sqrt(x2+y2) = distance

    }
  }

  Serial.print(photodiode[0]);
  Serial.print("\t");
  Serial.print(photodiode[1]);
  Serial.print("\t");
  Serial.print(photodiode[2]);
  Serial.print("\t");
  Serial.print(photodiode[3]);
  Serial.print("\t");
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.println(percent);

  delay(1000); // debug only

}
