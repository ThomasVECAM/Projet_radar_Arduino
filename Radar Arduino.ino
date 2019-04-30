// Nous avons à chaque fois pris des extraits, que nous avons légerement modifié.

// 1. Code capteur ultra son
     //   https://www.carnetdumaker.net/articles/mesurer-une-distance-avec-un-capteur-ultrason-hc-sr04-et-une-carte-arduino-genuino/ 

// 2. Code permettant la remémoration de la fonction millis (déjà utilisée auparavant)
    // https://www.carnetdumaker.net/articles/faire-plusieurs-choses-la-fois-avec-une-carte-arduino/

//3. Convertir un nombre à 4 chiffres en 4 variables de 1 chiffres (nous avons pris seulement 4 lignes de ce code à 100 lignes)
    //https://create.arduino.cc/projecthub/pentiumcadiz/4-digit-rtc-clock-85068b

//Pour la Led RGB, le buzzer, nous nous souvenions des concepts vus en première année. Nous n'avons pas trouvé utile de faire des recherches pour allumer une led
//ni pour transformer un nombre en binaire.

//-------------------------------------------------------------------------------------------------------------------------------------------
    
    //Variables de modifications destinées aux clients

const float SOUND_SPEED = 340.0;  //en m/s   
int distance_capteur_max = 1000;  //en mm   //distance maximum de zone de détection du capteur
int distance_capteur_min = 200;   //en mm   //distance minimale de zone de détection du capteur
float espacement_capteur = 68;    //en cm   // espacement entre les deux capteurs


//--------------------------------------------------------------------------------------------------------------------------------------------

    //Variables relatives aux capteurs
    
bool activate_captor_1 = true;
bool activate_captor_2 = false;
const byte TRIGGER_PIN_1 = A2; 
const byte ECHO_PIN_1 = A3;    
const byte TRIGGER_PIN_2 = A0;
const byte ECHO_PIN_2 = A1; 
const unsigned long MEASURE_TIMEOUT_1 = 25000UL; // 25ms = ~8m à 340m/s
const unsigned long MEASURE_TIMEOUT_2 = 25000UL; // 25ms = ~8m à 340m/s
unsigned long temps_enregistrer_1;
unsigned long temps_enregistrer_2;


    //Variables relatives à l'écran et à l'affichage
int nombre;
int nb1; // valeur digit 1 du 4x7 segments
int nb2; // valeur digit 2 du 4x7 segments
int nb3; // valeur digit 3 du 4x7 segments
int nb4; // valeur digit 4 du 4x7 segments

int ecran_1 = 6;  // allumage digit 1 du 4x7 segments
int ecran_2 = 7;  // allumage digit 2 du 4x7 segments
int ecran_3 = 8;  // allumage digit 3 du 4x7 segments
int ecran_4 = 9;  // allumage digit 4 du 4x7 segments
int virgule = 10; // allumage virgule centrale du 4x7 segments

int sortie_bin1 = 2; //les sorties binaires permettent d'amener l'information du nombre au "convertisseur logique"
int sortie_bin2 = 3;
int sortie_bin3 = 4;
int sortie_bin4 = 5;

int temps_switch = 1; //en microsecondes

    //Autre
int RGBR = 12;
int RGBV = 13;

unsigned long temps_actuel;
float vitesse;
int buzzer = 11;

int vitesse_autorisee;
unsigned long temps_demarrage_ecran;

bool affichage_vitesse_passage = false;


void setup() {   
  pinMode(TRIGGER_PIN_1, OUTPUT);
  pinMode(TRIGGER_PIN_2, OUTPUT);

  digitalWrite(TRIGGER_PIN_1, LOW);
  digitalWrite(TRIGGER_PIN_2, LOW);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(ECHO_PIN_2, INPUT);

  pinMode(ecran_1,OUTPUT);
  pinMode(ecran_2,OUTPUT);
  pinMode(ecran_3,OUTPUT);
  pinMode(ecran_4,OUTPUT);
  pinMode(sortie_bin1,OUTPUT);
  pinMode(sortie_bin2,OUTPUT);
  pinMode(sortie_bin3,OUTPUT);
  pinMode(sortie_bin4,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(virgule, OUTPUT);
  pinMode(A5,INPUT);

  pinMode(RGBV, OUTPUT);
  pinMode(RGBR, OUTPUT);
  
  digitalWrite(RGBV, HIGH);
  digitalWrite(RGBR, HIGH);
  
  changement_vitesse(); //appele une première fois la fonction changement de vitesse, pour initialiser
}
 
  //---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop(){
  temps_actuel = millis();
  int bouton_state = digitalRead(A5);
  
  if (bouton_state == HIGH){
      changement_vitesse();
  }
  else{
     
    if (affichage_vitesse_passage == true){
      ecran();
    }

    else{
      if (activate_captor_1 == true){
        capteur_1();   }
      
      else{ 
    capteur_2();    }
    }
  }
}

  //---------------------------------------------------------------------------------------------------------------------------------------------------------------------

void changement_vitesse(){
      int sensorValue = analogRead(A4);
      float voltage = sensorValue * (5.0 / 1023.0);
      vitesse_autorisee = (voltage*4)+2;
      nombre= vitesse_autorisee*100;
      ecran();
}
void capteur_1() {
        /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_1, LOW);
  
        /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure_1 = pulseIn(ECHO_PIN_1, HIGH, MEASURE_TIMEOUT_1);
   
        /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm_1 = measure_1 / 2.0 * (SOUND_SPEED/1000);
   
  if(distance_mm_1 < distance_capteur_max and distance_mm_1 > distance_capteur_min){
      temps_enregistrer_1 = temps_actuel;
      activate_captor_1 = false;
  }  
}

void capteur_2() {
  
        /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_2, LOW);
  
        /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure_2 = pulseIn(ECHO_PIN_2, HIGH, MEASURE_TIMEOUT_2);
   
        /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm_2 = measure_2 / 2.0 * (SOUND_SPEED/1000);
   
  if(distance_mm_2 < distance_capteur_max and distance_mm_2 > distance_capteur_min){
    temps_enregistrer_2 = temps_actuel;
    float temps_parcours = temps_enregistrer_2 - temps_enregistrer_1;
    vitesse = (espacement_capteur*36)/(temps_parcours);
    activate_captor_1 = true;

    nombre = vitesse*100;
    affichage_vitesse_passage = true;
    temps_demarrage_ecran = temps_actuel;
    
    actions();
  }  
}


void actions(){
      if(vitesse > vitesse_autorisee){
        tone(buzzer, 700, 1000); // Send 1KHz sound signal...
        digitalWrite(RGBR, LOW);
        digitalWrite(RGBV, HIGH);         
    }
    else{
      digitalWrite(RGBV, LOW);
      digitalWrite(RGBR, HIGH);
    }
}

void ecran() {
  
   //le nombre en question est multiplié par 100 pour obtenir un nombre entier et pour pouvoir utiliser sans réfléchir la source internet suivante
  int nb1 = nombre%10;
  int nb2 = (nombre%100)/10;
  int nb3 = (nombre%1000)/100;
  int nb4 = (nombre%10000)/1000;

    //démarrage de l'écran 1
  call_number(nb4);
  digitalWrite(ecran_1,HIGH);
  delay(temps_switch);
  digitalWrite(ecran_1,LOW);

    //démarrage de l'écran 2
  call_number(nb3);
  digitalWrite(ecran_2,HIGH);
  digitalWrite(virgule, LOW); // à LOW pour allumer car fonctionne en anode commune donc logique inversée
  delay(temps_switch);
  digitalWrite(ecran_2,LOW);
  digitalWrite(virgule, HIGH);
    
    //démarrage de l'écran 3
  call_number(nb2);
  digitalWrite(ecran_3,HIGH);
  delay(temps_switch);
  digitalWrite(ecran_3,LOW);
      
      //démarrage de l'écran 4
  call_number(nb1);
  digitalWrite(ecran_4,HIGH);
  delay(temps_switch);
  digitalWrite(ecran_4,LOW);

  int difference = temps_actuel - temps_demarrage_ecran;
  Serial.println(difference);
  if (temps_actuel - temps_demarrage_ecran > 3000){  
        affichage_vitesse_passage = false;
        digitalWrite(RGBV, HIGH); 
        digitalWrite(RGBR, HIGH);
  }
}

//  Fonctions convertissant un nombre hexadécimal sur les sorties bianires
void call_number(int entree){
  if(entree == 0){
    digitalWrite(sortie_bin1, LOW);
    digitalWrite(sortie_bin2, LOW);
    digitalWrite(sortie_bin3, LOW);
    digitalWrite(sortie_bin4, LOW);  }  
  else if(entree == 1){
     digitalWrite(sortie_bin1, HIGH);
    digitalWrite(sortie_bin2, LOW);
    digitalWrite(sortie_bin3, LOW);
    digitalWrite(sortie_bin4, LOW); }
  else if(entree == 2){
    digitalWrite(sortie_bin1, LOW);
    digitalWrite(sortie_bin2, HIGH);
    digitalWrite(sortie_bin3, LOW);
    digitalWrite(sortie_bin4, LOW);  }
  else if(entree == 3){
    digitalWrite(sortie_bin1, HIGH);
    digitalWrite(sortie_bin2, HIGH);
    digitalWrite(sortie_bin3, LOW);
    digitalWrite(sortie_bin4, LOW);   }
  else if(entree == 4){
    digitalWrite(sortie_bin1, LOW);
    digitalWrite(sortie_bin2, LOW);
    digitalWrite(sortie_bin3, HIGH);
    digitalWrite(sortie_bin4, LOW);   }
  else if(entree ==5){
    digitalWrite(sortie_bin1, HIGH);
    digitalWrite(sortie_bin2, LOW);
    digitalWrite(sortie_bin3, HIGH);
    digitalWrite(sortie_bin4, LOW);  }
  else if(entree == 6){
    digitalWrite(sortie_bin1, LOW);
    digitalWrite(sortie_bin2, HIGH);
    digitalWrite(sortie_bin3, HIGH);
    digitalWrite(sortie_bin4, LOW);  }
  else if(entree == 7){
    digitalWrite(sortie_bin1, HIGH);
    digitalWrite(sortie_bin2, HIGH);
    digitalWrite(sortie_bin3, HIGH);
    digitalWrite(sortie_bin4, LOW);    }
  else if(entree == 8){
    digitalWrite(sortie_bin1, LOW);
    digitalWrite(sortie_bin2, LOW);
    digitalWrite(sortie_bin3, LOW);
    digitalWrite(sortie_bin4, HIGH);   }
  else if(entree == 9){
    digitalWrite(sortie_bin1, HIGH);
    digitalWrite(sortie_bin2, LOW);
    digitalWrite(sortie_bin3, LOW);
    digitalWrite(sortie_bin4, HIGH);  }
}
