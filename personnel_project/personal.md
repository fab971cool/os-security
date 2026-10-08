
# Projets

## Misc 145

Lors de l'ouverture et de la lecture d'un fichier, le démons audit.d enregistre ces actions.
Afin de ne pas être détecter, on utilise une bibliothèque de lecture asynchrone. Ici, io_uring met en place un ring buffer. va mettre en place une file afin de donner des actions d'ouverture et de lecture à une fonction du noyau.


Ce système est intéressant car il peut être mise en place pour un rootkit personnalisé qui se trouve dans le noyau. 
Cela pourrait être un prgrme utilisateur qui intéragit avec le rootkit 