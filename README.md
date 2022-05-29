# projetC-server
## Made by Italiano Lorenzo and Sartori Tom. 

[GitHub of the server. ](https://github.com/tom-sartori/projetC-server)  
[GitHub of the client. ](https://github.com/tom-sartori/projetC-client)

## To run
```shell
./serveur.sh
```

## To see the documentation
```shell
open docs/index.html
```

## Lis of available commands
| Fonction          | Descriptif                                                           | Commande    |  Attribut 1  | Attribut 2 |
|:------------------|:---------------------------------------------------------------------|:------------|:------------:|:----------:|
| Aide              | Indique la liste des commandes disponibles.                          | /help       |              |            |
| Déconnexion       | Se déconnecter.                                                      | /disconnect |              |            |
| Kick              | Kick/déconnecte un utilistauer connecté.                             | /kick       |   username   |            |
| Liste utilisateur | Affiche la liste des utilisateurs connectés.                         | /users      |              |            |
| Message privé     | Envoie un message privé à un utilisateur.                            | /mp         |   username   |  message   |
| Fichier           | Envoie un fichier à un autre utilisateur.                            | /file       |   username   |  filename  |
| Fichier           | Envoie au serveur un fichier placé dans le dossier 'uploads'.        | /file       |    -send     |  filename  |
| Fichier           | Récupère un fichier placé dans le dossier 'uploads' du serveur.      | /file       |     -get     |  filename  |
| Liste fichier     | Liste les fichiers disponibles dans le dossier 'uploads' du client.  | /files      |      -c      |            |
| Liste fichier     | Liste les fichiers disponibles dans le dossier 'uploads' du serveur. | /files      |      -s      |            |
| Renommage         | Se renommer.                                                         | /rename     |   username   |            |
| Channel actuel    | Indique le channel actuel dans lequel est l'utilisateur.             | /channel    |              |            |
| Liste channel     | Liste les différents channels accessibles.                           | /channels   |              |            |
| Rejoindre channel | Rejoint le channel spécifié.                                         | /join       | channelIndex |            |
| Message public    | Envoi du message à tous les clients de tous les channels.            | /all        |   message    |            |
| Mode disco        | Active le mode disco!                                                | /disco      |              |            |
| Pew pew           | Pew pew!                                                             | /pew pew    |              |            |
