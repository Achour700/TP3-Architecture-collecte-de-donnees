# ReadMe - Code de Routage Proactif avec LoRa

Ce code a été développé pour permettre la communication sans fil entre plusieurs nœuds à l'aide de la technologie LoRa. Chaque nœud possède une table de routage précalculée, et la communication est établie en consultant localement cette table.

## Configuration des Nœuds

- Les identifiants des nœuds (`NODE_A`, `NODE_B`, `NODE_C`, `NODE_D`) sont définis en tant que constantes.
- La MACRO `LOCAL_ID` est utilisée pour choisir le nœud à compiler, permettant ainsi une personnalisation aisée.

## Tableaux de Routage

- Les tables de routage sont initialisées en fonction du nœud local sélectionné. Chaque table contient des informations telles que le nœud suivant, le nœud de destination, le nombre de sauts, et le taux de réussite de transmission (PRR).

## Configuration Initiale (setup)

- La fonction `setup` initialise la communication série et le module LoRa avec une fréquence de 915 MHz. Le nœud local est affiché sur le port série.

## Fonction Principale (loop)

- La fonction `loop` gère la lecture du port série pour obtenir le nœud de destination. Elle convertit ensuite le nœud de destination en nœud correspondant et appelle les fonctions appropriées pour envoyer et recevoir des trames.

## Fonction de Recherche de Route (`FindRoute`)

- La fonction `FindRoute` recherche la route avec le plus haut PRR vers un nœud de destination donné. Elle parcourt la table de routage et sélectionne la meilleure route en fonction du PRR.

## Fonctions d'Envoi et de Réception de Trame (`SendTrame` et `ReciveTrame`)

- La fonction `SendTrame` envoie une trame contenant des informations telles que le nœud émetteur, le nœud suivant, le nœud final, et la température.
- La fonction `ReciveTrame` écoute en permanence les trames LoRa entrantes, extrait les informations, et prend des décisions en fonction du nœud local.

## Conclusion

Ce code offre une implémentation robuste du routage proactif avec LoRa, permettant une communication efficace et ciblée entre nœuds. La modularité du code facilite la configuration des identifiants et des tables de routage pour chaque nœud spécifique, offrant ainsi une flexibilité dans les déploiements de réseaux de capteurs sans fil.
