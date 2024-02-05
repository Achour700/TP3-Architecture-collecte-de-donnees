#include <SPI.h>
#include <LoRa.h>

// Définition des identifiants des nœuds
#define NODE_A 1
#define NODE_B 2
#define NODE_C 3
#define NODE_D 4

// Variables globales
int temperature = -1;
int prr = 50;  // qualité de la transmission

// Choisissez le nœud à compiler (décommentez la ligne correspondante)
#define LOCAL_ID 1
// Tableau de routage des nœuds

// Initialisation du tableau de routage en fonction du nœud local sélectionné NEXT NODE, DESTINATION NODE, HOP, PRR
#ifdef LOCAL_ID
    #if LOCAL_ID == 1 
    //node A
        int tab_route[7][4] = { { NODE_B, NODE_B, 1, 95 },
                                { NODE_B, NODE_C, 2, 66.5 },
                                { NODE_C, NODE_C, 1, 70 },
                                { NODE_C, NODE_B, 2, 90.25 },
                                { NODE_C, NODE_D, 2, 90.25 },
                                { NODE_D, NODE_D, 1, 95 },
                                { NODE_D, NODE_C, 2, 66.5 } };
    #elif LOCAL_ID == 2    //node b
        int tab_route[6][4] = { { NODE_A, NODE_A, 1, 95 },
                                { NODE_A, NODE_C, 2, 66.5 },
                                { NODE_C, NODE_C, 1, 95 },
                                { NODE_C, NODE_A, 2, 66.5 },
                                { NODE_D, NODE_A, 2, 90.5 },
                                { NODE_D, NODE_C, 2, 90.5 } };
    #elif LOCAL_ID == 3    //node c
        int tab_route[7][4] = { { NODE_A, NODE_A, 1, 70 },
                                { NODE_A, NODE_B, 2, 90.25 },
                                { NODE_A, NODE_D, 2, 90.25 },
                                { NODE_B, NODE_B, 1, 95 },
                                { NODE_B, NODE_C, 2, 66.5 },
                                { NODE_D, NODE_D, 1, 95 },
                                { NODE_D, NODE_A, 2, 66.5 } };
    #elif LOCAL_ID == 4     //node D
        int tab_route[4][4] = { { NODE_A, NODE_A, 1, 95 },
                                { NODE_B, NODE_A, 2, 90 },
                                { NODE_C, NODE_C, 1, 95 },
                                { NODE_B, NODE_C, 2, 90 } };
    #endif
#endif


/**
 * @brief Fonction de configuration initiale.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  Serial.print("NODE : ");
  Serial.println(LOCAL_ID);
}

/**
 * @brief Fonction principale, gère la lecture du port série et appelle les fonctions correspondantes.
 */
void loop() {
  // Lecture du port série
  while (Serial.available()) {
    int destinationNode = Serial.read() - '0';
    Serial.readString();  // Lire la séquence restante du port série
    Serial.println(destinationNode);
    int node_dest = -1;

    switch (destinationNode) {
      case 1:
        node_dest = NODE_A;
        break;
      case 2:
        node_dest = NODE_B;
        break;
      case 3:
        node_dest = NODE_C;
        break;
      case 4:
        node_dest = NODE_D;
        break;
      default:
        Serial.println("Destination invalide");
        continue;  // Ignorer les destinations invalides
    }

    Serial.print("num du destinataire = ");
    Serial.println(node_dest);

    int node_emetteur = LOCAL_ID;                        // node émetteur
    int next_node = tab_route[FindRoute(node_dest)][0];  // node suivant
    int dest_node = tab_route[FindRoute(node_dest)][1];  // node final
    int temperature = random(0, 9);

    // Envoyer Trame
    SendTrame(node_emetteur, next_node, dest_node, temperature);
  }

  ReciveTrame();
}

/**
 * @brief Fonction pour trouver la route avec prr.
 * @param node_dest Le nœud de destination.
 * @return L'index de la route trouvée.
 */
int FindRoute(int node_dest) {
  int routeIndex = -1;
  int Val_max = 0;

  for (int i = 0; i < sizeof(tab_route) / sizeof(tab_route[0]); i++) {
    if (tab_route[i][1] == node_dest && tab_route[i][3] > prr) {
      if (tab_route[i][3] > Val_max) {
        Val_max = tab_route[i][3];
        routeIndex = i;
      }
    }
  }

  if (routeIndex < 0 || routeIndex >= sizeof(tab_route) / sizeof(tab_route[0])) {
    Serial.println("Route inconnue");
  }

  return routeIndex;
}

/**
 * @brief Fonction pour envoyer une trame.
 * @param node_emetteur Le nœud émetteur.
 * @param next_node Le nœud suivant.
 * @param dest_node Le nœud final.
 * @param temperature La température à envoyer.
 */
void SendTrame(int node_emetteur, int next_node, int dest_node, int temperature) {
  LoRa.beginPacket();
  LoRa.print(node_emetteur);
  LoRa.print(next_node);
  LoRa.print(dest_node);
  LoRa.print(temperature);
  LoRa.endPacket();
  Serial.println("Trame envoyee : ");
  Serial.print(node_emetteur);
  Serial.print(next_node);
  Serial.print(dest_node);
  Serial.println(temperature);
}

/**
 * @brief Fonction pour recevoir une trame.
 */
void ReciveTrame() {
  int node_emetteur_Recu, next_node_Recu, dest_node_Recu, temperature_Recu;
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print("Received packet ");

    // read packet
    while (LoRa.available()) {
      node_emetteur_Recu = (char)LoRa.read() - '0';
      next_node_Recu = (char)LoRa.read() - '0';
      dest_node_Recu = (char)LoRa.read() - '0';
      temperature_Recu = (char)LoRa.read() - '0';

      Serial.print("received trame = ");
      Serial.print(node_emetteur_Recu);
      Serial.print(next_node_Recu);
      Serial.print(dest_node_Recu);
      Serial.print(temperature_Recu);

      if (next_node_Recu != LOCAL_ID) {
        Serial.println("Je ne suis pas le récepteur concerné :");
      } else {
        Serial.print("Je suis le next ID.");

        if (dest_node_Recu != LOCAL_ID) {
          Serial.println(" j'envoie au destinataire concerné");
          // envoie de la trame
          SendTrame(node_emetteur_Recu, dest_node_Recu, dest_node_Recu, temperature_Recu);
        } else {
          Serial.println("Je suis le destinataire final  ");
          Serial.println("Temperature =  ");
          Serial.print(temperature_Recu);
        }
      }
    }
  }
}