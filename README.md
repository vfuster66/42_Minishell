# 42-Minishell
Aussi mignon qu'un vrai shell

## Partie obligatoire
Écrivez un shell qui doit :
- Afficher un prompt en l’attente d’une nouvelle commande
- Posséder un historique fonctionnel
- Chercher et lancer le bon executable (basé sur une variable d’environnement `PATH` ou en utilisant un path absolu), comme dans `bash`
- Utiliser une variable globale au maximum
- Ne pas interpréter les quotes non fermés
- Ne pas interpréter les caractères spéciaux non demandés
- Gérer `'` qui doit empêcher le shel d'interpréter les méta-caractères présents dans la séquence entre guillemets
- Gérer `"` qui doit empêcher le shel d'interpréter les méta-caractères présents dans la séquence entre guillemets sauf `$`

### Redirections
- `<` doit rediriger l'entrée
- `>` doit rediriger la sortie
- `<<` doit recevoir un délimiteur et lire l'input donné jusqu'à rencontrer une ligne contenant le délimiteur. L'historique ne doit pas être mis à jour
- `>>` doit rediriger la sortie en mode append 
    
- `;` dans la ligne de commande doit séparer les commandes
- `'` et `"` doivent marcher comme dans bash, à l’exception du multiligne
- Les redirections `<`, `>`et `>>` doivent marcher comme dans bash, à l’exception des aggrégations de fd
- `|` doit marcher comme dans bash
- Les variables d’environnement (`$variable`) doivent marcher comme dans bash
- `$?` doit marcher comme dans bash
  
- `ctrl-C`, `ctrl-D` et `ctrl-\` doivent afficher le même résultat que dans bash
- `ctrl-C` affiche un nouveau prompt
- `ctrl-D` quitte le shell
- `ctrl-\` ne fait rien

### Builtins
Vous devez implémenter les builtins suivants :
  - `echo` et l’option `-n`
  - `cd` uniquement avec un chemin absolu ou relatif
  - `pwd` sans aucune option
  - `export` sans aucune option
  - `unset` sans aucune option
  - `env` sans aucune option ni argument
  - `exit` sans aucune option

## Partie bonus
Vou devez implémenter :
- `&&` et `||` avec des parenthèses pour les priorités
- Les wildcards `*` doivent fonctionner pour le répertoire courant

### Fonctions autorisées
- `fork` : créé un nouveau processus enfant
- `wait` : attend un signal d'arrêt d'un processus, wait4 pour attendre la fin d'un processus précis
- `exit` : termine le processus
- `getcdw` : pour obtenir le pwd
- `stat` : donne toutes les infos d'un fichier comme un ls
- `lstat` : same sauf que dans le cas d'un lien donne les infos sur le lien et pas le fichier pointé
- `fstat` : donne toutes les infos sur un fichier comme un ls, mais cette fois on lui passe en argument le file descriptor
- `kill` : envoie un signal d'arrêt au processus indiqué
- `signal(sig, fun)` : exécute `fun` si `sig` est reçu
- `execve` : exécuter un exécutable, avec ses arguments et l'environnement en paramètres
- `dup` : duplique le descripteur d'un objet, i.e. attribue un nouveau descripteur qui pointe vers le même objet, ainsi lorsque l'on read sur l'un on avance sur les deux
- `dup2` : permet en plus de spécifier la valeur du descripteur
- `chdir` :  change le répertoire courant
- `opendir` : ouvre le fichier descriptif du dossier
- `readdir` : lit l'entrée suivante dans le fichier descriptif du dossier
- `closedir` : referme le ficher descriptif du dossier
- `pipe` : rempli un tableau avec deux nouveaux file descriptors qui vont être utiles pour changer l'entrée standard et la sortie standard

### Valeurs de retour

| Value | Signification |
|----:|:-------------|
| 1 | Catchall for general errors	impermissible operations |
| 2 | Misuse of shell builtins |
| 126	| Command invoked cannot execute (permission problem or not an executable) |
| 127	| Command not found |
| 128	| Invalid argument to exit |
| 128+n | Error signal n |
| 130	| Script terminated by Control-C |
| 255 | Exit status out of range |

## Sources

- https://shell.multun.net/structure.html
- https://shell.figarola.fr/x212.html
- https://n-pn.fr/t/2318-c--programmation-systeme-execve-fork-et-pipe
- https://shell.figarola.fr/x54.html
- http://www.zeitoun.net/articles/communication-par-tuyau/start
- https://www.geeksforgeeks.org/fork-system-call/
- https://www.codequoi.com/creer-et-tuer-des-processus-fils-en-c/
- https://www.codequoi.com/pipe-une-methode-de-communication-inter-processus/
- https://www.codequoi.com/envoyer-et-intercepter-un-signal-en-c/
- https://www.codequoi.com/manipuler-un-fichier-a-laide-de-son-descripteur-en-c/
- https://www.codequoi.com/errno-et-la-gestion-derreur-en-c/

## Valide
![image](https://github.com/vfuster66/42-Minishell/assets/124174795/a3b1966a-0225-42f5-94fd-2608c06fbf34)

