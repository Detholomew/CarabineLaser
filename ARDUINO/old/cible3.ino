#define CENTRE 0  // Pate des photodiodes
#define AXE1 1
#define AXE2 2
#define AXE3 3

#define X_POS1 6.0    // Position des photodiodes
#define X_POS2 3.0
#define Y_POS1 6.0

#define SENSIBILITE 0.5 // sensibilité de la cible. 0 < SENSIBILITE < 1; Le plus haute le plus moins selectif
#define SEUIL 20.0     // define la valeur moyenne minimum pour determiner la pourcentage

#define SCORE_MINIMAL 30.0
 
void setup(void) {
  Serial.begin(9600);
  

}
 
void loop(void) 
{
  unsigned long tinit = micros();
  unsigned long t1,t2;

  byte percent = 0;
  
  unsigned int photodiode[4];    // declare le vecteur de ints qui auront les valeurs du analogread
  unsigned int ptd_insig;     // le numero de la photodiode moins fort
  float ptd_sum=0.0;
  int x, y;
  byte indice_insig=0;
  float xcm, ycm;


  // ------ Obtention des valeurs -----
  for(byte i=0; i<4; i++)
  {
    for(byte j=0; j<32; j++)
     {
      photodiode[i] += analogRead(i);  // On prends les valeurs du analogRead sur 1024 <-> (int plus rapide que float)
     }
  }
  
  
  for(byte k=0; k<4; k++)
  {
    photodiode[k] /= 32;
  }
  
  /*t1 = micros()-tinit;
  Serial.print("T1: ");
  Serial.print(t1); */


  // ------ Check photodiode avec valeur plus faible -----

  ptd_sum += photodiode[0];
  
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

  xcm = -1; ycm = -1; percent=0;
  
  // -------- Check tir ------ //
  if ( (photodiode[indice_insig] < (int)(SENSIBILITE*ptd_sum/4.0) ) && (ptd_sum > SEUIL ) )
  {
    
  // --------- Delete l'element insignificatif ----- //
    ptd_sum = ptd_sum - photodiode[indice_insig];     
    photodiode[indice_insig] = 0;

  // --------- Determine les Xcm et Ycm ------------ // 
    xcm = ( photodiode[1]*X_POS1 + X_POS2*(photodiode[2] + photodiode[3]) ) / ( ptd_sum );
    ycm = ( Y_POS1*(photodiode[2] + photodiode[3]) ) / ( ptd_sum );

  // --------- float = slow; on le transform en int pour faire les manips ------ //
    x = (int)((xcm*10)+0.5);
    y = (int)((ycm*10)+0.5);
    
  // --------- On trouve la distance du centre de masse à l'origine, et on trouve le score correspondant a cetter distance    
    percent = (byte)map( sqrt( x^2 + y^2 ) , 85.0, 0, SCORE_MINIMAL, 100);    // notez que sqrt(x2+y2) = distance
  }

  Serial.print(photodiode[0]);
  Serial.print("\t");
  Serial.print(photodiode[1]);
  Serial.print("\t");
  Serial.print(photodiode[2]);
  Serial.print("\t");
  Serial.print(photodiode[3]);
  Serial.print("\t");
  Serial.println(percent);

}
