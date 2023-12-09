# Dev_Vernam

Depot de Alexis Wamster

Instructions pour compiler:
---------------------------

gcc -o "executable" cryptage.c
(pas d'option particulière à passer à gcc)

Instructions pour utiliser:
---------------------------

Attention, on ne peut pas executer en faisant simplement ./executable.
Il faut passer une option (-k -c ou -d) et un certain nombres de fichiers en ligne de commande. (détails ci-dessous)

- -k ==> Géneration d'un fichier clef selon la taille de fichier message

ex: ./executable -k message.txt clef.txt

- -c ==> Création d'un fichier message codé à partir du fichier message et fichier clef

ex: ./executable -c message.txt clef.txt    message_code.txt

- -d ==> Création d'un fichier message decodé à partir du fichier message codé et fichier clef

ex: ./executable -d message_decode.txt clef.txt    message_code.txt

Limites de l'outil:
------------------

Les caracteres que l'on peut utiliser son seulement ceux de la table ASCII (compris entre 0 et 255)

Les explications lorsque ça ne marche pas:
------------------------------------------
Le programme renvoie un message d'erreur lorsque:
- vous ne fournissez pas les bons arguments sur la ligne de commande
- Vous fournissez une clef plus petites que le message ou que le message codé
- Le programme n'arrive pas à ouvrir un certain fichier dans un certain mode
- Le programme n'arrive pas à écrire dans un certain fichier.