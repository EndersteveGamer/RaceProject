#!/bin/bash

premier_race26=0
premier_racehi=0
score_cumule_race26=0
score_cumule_racehi=0

# Répertoire pour enregistrer les résultats
resultats_dir="resultats"

# Créer le répertoire s'il n'existe pas
mkdir -p $resultats_dir

# Boucle pour exécuter la commande 100 fois
for i in {1..10}
do
    # Exécuter la ligne de commande
    resultat=$(./race-server ./cmake-build-debug/RaceProject ./race-hi 2>&1)
    
    # Extraire la position du joueur avec grep (exemple)
    joueur1=$(echo "$resultat" | awk '/\(1\)/ {print $2}')
    joueur2=$(echo "$resultat" | awk '/\(2\)/ {print $2}')
    score_joueur1=$(echo "$resultat" | awk '/\(1\)/ {gsub(/^ +| +$/,""); print $3}')
    score_joueur2=$(echo "$resultat" | awk '/\(2\)/ {gsub(/^ +| +$/,""); print $3}')
    
    # Vérifier quelle position est la première et incrémenter le compteur correspondant
    
    if [ "$joueur1" == "./cmake-build-debug/RaceProject:" ]; then
        premier_race26=$((premier_race26 + 1))
        if [[ "$score_joueur1" =~ ^[0-9]+$ ]]; then
            score_cumule_race26=$((score_cumule_race26 + score_joueur1))
        fi
        if [[ "$score_joueur2" =~ ^[0-9]+$ ]]; then
            score_cumule_racehi=$((score_cumule_racehi + score_joueur2))
        fi
    else
        premier_racehi=$((premier_racehi + 1))
        if [[ "$score_joueur2" =~ ^[0-9]+$ ]]; then
            score_cumule_race26=$((score_cumule_race26 + score_joueur2))
        fi
        if [[ "$score_joueur1" =~ ^[0-9]+$ ]]; then
            score_cumule_racehi=$((score_cumule_racehi + score_joueur1))
        fi
    fi

    # Afficher les positions dans le terminal
    echo "Premier ($i) : $joueur1, Score : $score_joueur1"
    echo "Deuxieme ($i) : $joueur2,  Score : $score_joueur2"
    echo ""
    
    # Enregistrer les positions dans un fichier distinct
    echo "$resultat" >> "$resultats_dir/resultat_$i.txt"
done
# Afficher le nombre de fois que race26 a fini premier
echo "Nombre de fois que race26 a fini premier : $premier_race26"
echo "Score total cumulé de race26 : $score_cumule_race26"

# Afficher le nombre de fois que race-hi a fini premier
echo "Nombre de fois que race-hi a fini premier : $premier_racehi"
echo "Score total cumulé de race-hi : $score_cumule_racehi"
echo "Fin du script"