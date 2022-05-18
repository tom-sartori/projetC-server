# projetC-server


## Commandes
| Fonction          | Descriptif                                                           | Commande    | Attribut 1 | Attribut 2 |
|:------------------|:---------------------------------------------------------------------|:------------|:----------:|:----------:|
| Aide              | Indique la liste des commandes disponibles.                          | /help       |            |            |
| Déconnexion       | Se déconnecter.                                                      | /disconnect |            |            |
| Kick              | Kick/déconnecte un utilistauer connecté.                             | /kick       |  username  |            |
| Liste utilisateur | Affiche la liste des utilisateurs connectés.                         | /users      |            |            |
| Message privé     | Envoie un message privé à un utilisateur.                            | /mp         |  username  |  message   |
| Fichier           | Envoie un fichier à un autre utilisateur.                            | /file       |  username  |  filename  |
| Fichier           | Envoie au serveur un fichier placé dans le dossier 'uploads'.        | /file       |   -send    |  filename  |
| Fichier           | Récupère un fichier placé dans le dossier 'uploads' du serveur.      | /file       |    -get    |  filename  |
| Liste fichier     | Liste les fichiers disponibles dans le dossier 'uploads' du client.  | /files      |     -c     |            |
| Liste fichier     | Liste les fichiers disponibles dans le dossier 'uploads' du serveur. | /files      |     -s     |            |
| Renommage         | Se renommer.                                                         | /rename     |  username  |            |
| Liste channel     | Liste les différents channels accessibles.                           | /channels   |            |            |


Idées commandes
- /r pour réponse au dernier message. 
- Admins ? 
- /rules
- /important : message important en rouge
- /all suivant les salons
