1) PARSING :
enfaite il n'y a pas vraiment de parsing a faire, tout sera gere par la suite si il y a des erreurs pendant le LEXER par exemple

2) LEXER :

- il faut reussir a distinguer chaque char* value, c'est-a-dire creer une liste chainee dans laquelle chaque element contiendra uniquement 2 variables :
le char* value qui correspond a la chaine de caracteres (ex : ls, cat, |, >, ...) et le *next pour pointer sur l'element d'apres (evidemment)

-> dans un premier temps, il faut verifier les quotes (doubles et simples) pour reussir a trouver les premiers char* value les plus evidents,
* faire attention a ne pas compter une simple quote comme une quote si celle-ci est entre des doubles quotes et inversement
et par la meme occasion verifier si toutes les quotes sont bien fermees a chaque fois sinon erreur: check your quotes

-> lors de cette verification de quotes, regarder lorsqu'on est entre dans une quote si il y a des expanders ou pas ($) , si on en trouve
alors on reflechira plus tard a ca --> il y aura 2 cas possibles soit cet expander sera entre simple quotes soit entre double quotes
-- simple = on garde le caractere $ ainsi que tout ce qui s'en suit
-- double = on prend tous les caracteres colles au $ et on cherche si cette variable (chaine) existe dans notre environnement,
si elle existe alors on remplace cette variable (chaine) par sa valeur, sinon on la remplace par une chaine vide (donc rien)
et on enleve bien sur le $ dans les 2 cas

- toujours pour l'expander, il y a 3 cas possibles ou on gere le $.. , dont 2 cas ou le resultat est le meme c'est-a-dire si on est
entre double quotes ou bien si le $.. n'est pas entre quotes ALORS ici on va chercher dans les variables d'env si la variable .. existe ou pas;
si elle existe on remplace $.. par la valeur sinon on remplace $.. par rien (une chaine vide);
dans le cas d'un simple quote le $.. n'est pas un expander, il est considere comme un caractere normal --> A FAIRE dans le PARSER possiblement

- cas particulier sur l'expander : echo "$US'E'R" , ici l'expander agit sur US et s'arrete a la 1ere quote qu'elle rencontre meme si celle-ci
est un caractere

- pour separer les mots au niveau des redir, quand on tombe sur une redir, de base on s'en sert comme separateur, sauf qu'il faut verifier si
c'est une redir simple ou pas donc si on tombe par exemple sur <, on regarde le caractere juste apres et tant que c'est < ou > alors on doit
etre sur une meme chaine et stocker le tout (ex: <<><<>) dans un seul char *value, l'erreur sera soulevee dans la partie PARSER;
et c'est pareil pour separer les mots au niveau des pipes, quand on tombe sur | il faut regarder le caractere juste apres et tant qu'on a un pipe
alors on stocke le tout dans une seule chaine (peut etre necessaire de faire ca, creer un char *value  pour chaque pipe dans ce cas est bon aussi,
vu qu'a la fin on retournera la meme erreur quand meme.. A voir)

- au niveau des quotes , il faut envoyer la liste en gardant les quotes, ne pas les enlever pour que je puisse faire l'expander ainsi
que pour differencier un token redir ou pipe d'un nom de fichier

3) PARSER :

- tout ce qui n'est pas un token (| , > , <, <<, >>) est une commande et des arguments si il y a plusieurs mots a la suite,
ce n'est pas a ce moment que l'on detecte si il y a une erreur de commande ou pas; il faut faire le parser comme si tout etait deja ok;
et c'est seulement dans la partie execve que l'erreur sera soulevee

- dans cette partie, on va devoir creer une liste chainee de tranches de pipe pour traiter chaque element de cette liste independamment, car peu importe
les erreurs qu'il peut y avoir entre 2 pipes, cela n'affectera pas les autres tranches de pipe qui s'executeront normalement, mais il faut quand meme
bien penser a afficher toutes les erreurs que l'on rencontre au cours de chaque execution de tranche de pipe si il y en a.

- pour les redirections simples (>, <) ainsi que le append (>>), peu importe le nombre d'entre eux, on aura au final dans une tranche de pipes un seul infile
et un seul outfile --> typiquement un exemple : <in<out<in grep b > a >b>b>b>c>>d>c | echo uuu , ici a gauche du pipe on aura pour infile le fichier in car
la derniere redirection < s'applique sur in, et on aura pour outfile le fichier c pour la meme raison avec > (il faut regarder le dernier > ou >>);
** il faut ABSOLUMENT ouvrir tous les fichiers un a un quand ils sont rediriges et pas seulement ouvrir le dernier infile et outfile

- une fois que le infile et outfile sont determines, il ne reste plus qu a executer LA commande qui se trouve dans notre tranche de pipes, si jamais
il y a au moins une 2eme commande dans cette tranche alors il faut retourner comme dans cet exemple : <in<out<in grep b wc -l > a >b>b>b>c>>d>c
terminal bash => grep: wc: No such file or directory , ici grep ne doit prendre qu'un seul argument au maximum suivi possbiblement d'un nom de fichier
qui sera quoi qu'il arrive son fichier infile peu importe les redirections precedentes et suivantes.. cas particulier du grep

- note importante au niveau des redirections : si un fichier 'c' devient outfile a un moment donne avec la redir >, alors si apres on a a nouveau une
redirection sur 'c' mais avec >> alors c'est la premiere redirection qui sera prise en compte, comme l'exemple qui suit :
<in<out<in wc >b>b>b>c>>d>>c<b<in -l --> ici on ecrit le resultat de la commande wc -l dans c en supprimant au prealable ce qui avait dedans,
c'est la redir > qui est prise en compte; en gros si un fichier est appele a y etre redirige plusieurs fois alors on garde la premiere redir

- autre cas avec les redir : si par exemple on a cat p > pp, et que p n'existe pas alors si pp n'existe pas deja on le cree en ne mettant rien dedans,
car la commande cat ne renvie rien dans ce cas; dans ce meme cas si on a cat p >>pp alors le fichier pp ne sera pas modifie

- redir encore a savoir : un heredoc n'est pas un cas particulier, le fichier cree avec un heredoc est un infile , et meme si on a plusieurs heredoc,
et qu'au bout de notre chaine on a une redir simple < in , alors ce fichier in sera notre infile.. et pour les heredoc, il faut les ouvrir dans l'ordre
dans lesquels ils se trouvent sur la ligne.. ex : <<p cat >pp <<dfdf <<kkkk <in --> on ouvre le heredoc p en 1er, puis dfdf, puis kkkk

- **pipes : il faut faire attention a un cas particulier avant de commencer a executer nos tranches de pipe, il faut verifier si il n'y a pas 2 pipes consecutives,
auquel cas aucune tranche de pipe ne doit s'executer; et meme chose si la ligne commence par un pipe, il faut afficher une erreur et rien executer

- lorsque j'ai au moins un heredoc sur ma ligne, celui-ci est execute en 1er avant toute autre execution, et une fois que tous les heredoc sont termines, alors
je peux executer tout le reste, et aussi meme si le nom du heredoc est le meme qu un fichier deja existant il n'a absolument aucun rapport avec celui-la,
c'est un fichier heredoc a part entiere

- quand on met une redir ou un pipe entre simple ou double quote sur bash, ce n'est plus considere comme un token pipe ou redir mais plutot comme un fichier;
est-ce qu'il faut gerer ce cas ??

- pour le test suivant assez particulier : wc < in| <cat >> pol2 | echo b >cat , dans la 2e tranche la chaine est utilisee comme infile mais n'existe pas de base,
malgre ceci il n'y a pas d'erreur renvoyee par bash; et pour d'autres commandes entrees en infile comme echo ou bien ls, il y a bien une erreur qui s'affiche disant
bash: ls: No such file or directory; pourquoi on a un cas particulier pour cat et pas une autre commande? ce cas ne sera surement pas pris en compte dans le projet 
* autre cas particulier ambigu : wc < in| <cat cat >> pol2 | echo bmm , on echo le 'bmm' en l'ajoutant dans le fichier pol2 .. pk ?
* un autre : wc < in| <cat cat >> pol2 | echo bmm ljj i  i i , ici le 'bmm ljj i  i i' est affiche sur le terminal et dans pol2 on ajoute seulement bmm .. pk?

- dans le cas ou je n'ai pas de commande a executer mais que j ai des redirections de fichier, je dois faire attention ouvrir mes fichiers comme il se doit selon
la redir imposee et SURTOUT NE PAS EXECUTER la fonction execve avec NULL comme commande -> comportement indefini

- au niveau des redir , a chaque redir que je trouve je dois ouvrir le fichier correspondant et verifier son chmod; si j'ai une redir infile '<' alors j'ouvre
le fichier et je verifie qu'il y a les droits de lecture seulement, et si j'ai une redir outfile '>' alors j'ouvre le fichier et je verifie qu'il y a les droits
d'ecriture seulement; on retourne une erreur si on a pas les droits correspondants et on s'arrete au 1er fail de fichier qu'on trouve, on ne teste pas les autres
redir et on passe au pipe_part d'apres

4) EXEC :

- si je n'ai pas de pipe, je dois regarder quelle est la commande, si ma commande doit se faire execve pour s'executer alors je dois creer un process enfant
pour ne pas perdre mon programme (comme exemple cat, ls, ... A regarder avec pipex_bonus, voir les fonctions find_path et search_the_path)

- au niveau des heredocs, pendant que l'utilisateur ecrit a l'interieur, si le Ctrl+C est effectue il faut que ca quitte correctement le heredoc, et c'est la qu'intervient
la variable globale avec laquelle il faut que je comprenne mieux ce qu'il faut faire (histoire de kill)

5) BUILTIN :

- export : essayer de prendre en compte le cas ou on ajouterait une chaine a une variable deja creee ; ex: export TOTO+=pp -> doit join la chaine 'pp' a la valeur
de TOTO ; il faut preciser que tout doit etre colle; ex: export TOTO +=pp -> error a ecrire ; et ex: export TOTO+= aa -> rajoute une chaine vide a TOTO car l'espace
separe la variable et ajoute une nouvelle variable aa
** cas particulier avec '\' a comprendre ; ex: export TOTO+=\aa ajoute aa; ex: export TOTO+=\\aa ajoute \\aa  ???

- cd : se fait tres rapidement  -> A VOIR

- echo : des cas particuliers a gerer