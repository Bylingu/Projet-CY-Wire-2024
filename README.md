# Projet-CY-Wire-2024
Pour bien executer le fichier bien verifier que les permissions sont bien donner ainsi que vous vous trouvez bien dans le bon fichier.
- sudo chmod 777 (Nom du fichier ) pour donner toute les permission 
-cd (nom fichier)
ensuite taper Make -
puis excuter ./c-write.sh vous serait ensuite guider par le programme.


Type de fichier:

    Script Shell (c-wire.sh)
    Le script Shell gère la validation des entrées, le filtrage des données et la coordination entre le fichier CSV et le programme en C. Les principales fonctionnalités incluent :

    Paramètres d'entrée :
        Chemin du fichier CSV d'entrée
        Type de station (hvb, hva, lv)
        Type de consommateur (comp, indiv, all)
        Identifiant de centrale électrique (optionnel)
    Validation : Vérifie la validité des paramètres et affiche un message d'erreur avec les instructions d'utilisation si nécessaire.
    Filtrage des données : Utilise awk pour filtrer les données pertinentes en fonction des paramètres fournis.
    Intégration avec le programme en C : Compile et exécute le programme en C si nécessaire, en passant les données filtrées pour traitement.
    Gestion des sorties : Génère des fichiers CSV structurés et des sorties graphiques (pour les stations LV).

    Programme en C (cwire.c)
    Le programme en C traite les données filtrées pour calculer les résumés de capacité énergétique et de consommation. Les principales fonctionnalités incluent :

    Implémentation d'un arbre AVL :
        Les nœuds représentent les stations avec des champs pour l'identifiant de station, la capacité et la consommation.
        Assure une insertion et une récupération équilibrées pour une gestion efficace des données.
    Traitement des données :
        Lit les données CSV filtrées.
        Agrège les données de consommation pour chaque type de station.
    Génération de sorties :
        Produit des données au format CSV requis.
        Retourne des codes d'erreur en cas de problèmes inattendus.

    Makefile
    Le Makefile automatise la compilation et le nettoyage pour le programme en C. Les cibles principales incluent :

    make : Compile l'exécutable cwire.
    make clean : Supprime les fichiers compilés et les données temporaires.

    Fichiers CSV :

    Fichiers nommés en fonction du type de station, du type de consommateur et de l'identifiant de la centrale électrique optionnel (par exemple, hva_comp.csv, lv_all_2.csv).
    Incluent l’identifiant de la station, la capacité et la consommation totale.

Analyse Min/Max (Stations LV) :

    lv_all_minmax.csv : Contient les 10 stations LV les plus et les moins chargées en énergie.

Graphiques :

    Graphique en barres visualisant les 10 stations LV les plus et les moins chargées en énergie.
    Généré avec GnuPlot et enregistré sous forme de fichier graphs/lv_all_minmax.png.
