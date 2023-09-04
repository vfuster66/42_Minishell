# 42-Minishell
Aussi mignon qu'un vrai shell

# Sources

https://www.codequoi.com/creer-et-tuer-des-processus-fils-en-c/

https://www.codequoi.com/pipe-une-methode-de-communication-inter-processus/

https://www.codequoi.com/envoyer-et-intercepter-un-signal-en-c/

https://www.codequoi.com/manipuler-un-fichier-a-laide-de-son-descripteur-en-c/

https://www.codequoi.com/errno-et-la-gestion-derreur-en-c/

# Notes

Fonctions autorisées

    fork : créé un nouveau processus enfant
    wait : attend un signal d'arrêt d'un processus, wait4 pour attendre la fin d'un processus précis
    exit : termine le processus
    getcdw : pour obtenir le pwd
    stat : donne toutes les infos d'un fichier comme un ls
    lstat : same sauf que dans le cas d'un lien donne les infos sur le lien et pas le fichier pointé
    ftat : donne toutes les infos sur un fichier comme un ls, mais cette fois on lui passe en argument le file descriptor
    kill : envoie un signal d'arrêt au processus indiqué
    signal(sig, fun) : exécute fun si sig est reçu
    execve : exécuter un exécutable, avec ses arguments et l'environnement en paramètres
    dup : duplique le descripteur d'un objet, i.e. attribue un nouveau descripteur qui pointe vers le même objet, ainsi lorsque l'on read sur l'un on avance sur les deux
    dup2 : permet en plus de spécifier la valeur du descripteur
    chdir : change le répertoire courant
    opendir : ouvre le fichier descriptif du dossier
    readdir : lit l'entrée suivante dans le fichier descriptif du dossier
    closedir : referme le ficher descriptif du dossier
    pipe : rempli un tableau avec deux nouveaux file descriptors qui vont être utiles pour changer l'entrée standard et la sortie standard

