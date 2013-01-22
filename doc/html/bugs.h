//--------------------------------------------------------------------------------

/*! \page bugs Bugs [in French]

<ul>
<li>\ref bug001
<li>\ref bug002
<li>\ref bug003
<li>\ref bug004
<li>\ref bug005
<li>\ref bug006
<li>\ref bug007
<li>\ref bug008
<li>\ref bug009
<li>\ref bug010
<li>\ref bug011
<li>\ref bug012
<li>\ref bug013
<li>\ref bug014
<li>\ref bug015
</ul>

<hr>

\section bug001 Bug 001 - [Perte solution] IbexBlockSolver + shaving

<div style="color:green">Version</div> 
0.2

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
L'usage de la 3B dans IbexBlockSolver fait perdre des solutions

<div style="color:green">Programme test</div> 
Pas de programme de test (la 3B n'est pas encore incorpor� dans IbexBlockSolver).

<div style="color:green">Fix� dans la version</div> 
0.3

<div style="color:green">Note interne</div> 
Les r�ductions faites sur les bandes � l'int�rieur de la 3B ne sont pas conserv�es, seulement, il se trouve 
que le rognage filtrait de fa�on intempestive sur les constantes symboliques (au lieu de la faire uniquement 
sur les variables), d'une part, et d'autre part que la synchronisation se fait de fa�on brutale entre un 
sous-syst�me et le syst�me global (toutes les entit�s sont synchronis�es) si bien que le syst�me global se 
trouvait � un certain stade � r�cup�rer dans des variables des intervalles correspondant � des constantes 
symboliques non valides (datant d'un filtrage ancien). 
Le probl�me a �t� r�gl� en interdisant le filtrage sur les constantes symboliques dans la classe IbexEvaluator.

<hr>

\section bug002 Bug 002 - [Exception inattendue] IbexBlockSolver avec 1 seul bloc/variable

<div style="color:green">Version</div> 
0.3

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
L'appel au constructeur IbexBlockSolver l�ve une exception IbexBlockSetException de type
NO_BLK lorsqu'on lui passe un seul bloc contenant une seule variable.

<div style="color:green">Programme test</div> 

\code
  IbexBlockSet bs(30, 1);

  IbexSystem sys("ponts.txt");

  bs.set_blk(sys.var("A_x"), 0); 

  IbexBlockSolver solver1(sys,bs);
\endcode

Output:
\verbatim
IbexBlockSetException
the variable does not belong to any block
\endverbatim

<div style="color:green">Fix� dans la version </div> 
0.4

<div style="color:green">Note interne</div> 
Probl�me dans la d�composition (effectu�e par IbexBlockSolver). Une contrainte n'�tait ajout�e � un sous-syst�me
que si les entit�s de la contrainte appartenaient au bloc courant ou � un bloc ant�rieur. Il fallait �galement prendre
en compte le fait qu'une variable pouvait n'appartenir � aucun bloc (d'o� l'exception).

<hr>

\section bug003 Bug 003 - [Erreur m�moire] R�solution sous-syst�me avec IbexDefaultSolver

<div style="color:green">Version</div> 
0.3

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
La r�solution d'un sous-syst�me (obtenu par d�composition) avec IbexDefaultSolver conduit � un crash m�moire.

<div style="color:green">Programme test</div> 

\code
  IbexBlockSet bs(30, 1);

  IbexSystem sys("ponts.txt");

  bs.set_blk(sys.var("A_x"), 0); 
  IbexSystemArray decomp = sys.decompose(bs);

  IbexDefaultSolver solver1(decomp[0]);
  solver1.explore();
\endcode

Output:<br>
seg fault (ou exception de type IbexInvalidEntityException, mais la m�moire a �t� viol�e).

<div style="color:green">Fix� dans la version </div> 
0.4

<div style="color:green">Note interne</div> 
La m�thode add_all_ctr() dans IbexSubSystemBuilder appelait add_ctr de la classe courante
au lieu de la classe m�re, cons�quence: les indices �taient interpr�t�s comme les num�ros
des contraintes dans le syst�me base au lieu des num�ros dans le repository. Ce probl�me
apparaissait au moment o� le sous-syst�me decomp[0] �tait copi� par le solveur.

\section bug004 Bug 004 - [Perte solutions] R�solution sous-syst�me avec IbexDefaultSolver + shaving

<div style="color:green">Version</div> 
0.4

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
La r�solution d'un sous-syst�me (obtenu par d�composition) avec IbexDefaultSolver avec shaving
perd des solutions.

<div style="color:green">Programme test</div> 

\code
 IbexBlockSet bs(30, 4);

  IbexSystem sys("ponts.txt"); // doit �tre la m�me chose que pontsE (au moins les 4 derni�res �quations!)

  bs.set_blk(sys.var("A_x"), 0); 

  bs.set_blk(sys.var("A_y"), 1); 

  bs.set_blk(sys.var("B_x"), 2); 

  bs.set_blk(sys.var("B_y"), 3); 

  IbexSystemArray decomp = sys.decompose(bs);

  /* solve block A_x=0 */
  IbexDefaultSolver solver0(decomp[0]);
  solver0.explore();
  decomp[0].box = solver0.border(0);
  sys.sync(decomp[0]);

  /* solve block A_y=0 */
  IbexDefaultSolver solver1(decomp[1]);
  solver1.explore();
  decomp[1].box = solver1.border(0);
  sys.sync(decomp[1]);

  /* solve block B_x=0 */
  IbexDefaultSolver solver2(decomp[2]);
  solver2.explore();
  decomp[2].box = solver2.border(0);
  sys.sync(decomp[2]);


  decomp[3].sync(sys);
  decomp[3].set_output_flags(15);

  /* solve block B_y=0 */
  IbexDefaultSolver solver3(decomp[3]);
  solver3.set_contractor(IBEX_SHAVE);
  solver3.explore();
  solver3.report();
\endcode

Output:<br>
\verbatim
found 0 inner box(es) found 0 border box(es)
\endverbatim

<div style="color:green">Fix� dans la version </div> 
0.5

<div style="color:green">Note interne</div> 
Bug introduit en changeant dans la version 0.4 le comportement de la m�thode hc4() en
cas d'infaisabilit�. Au lieu de vider simplement la bo�te, une exception de type
IbexUnfeasibilityException est d�sormais lev�e. Or, la m�thode shave() n'avait pas 
�t� adapt�e en cons�quence.

\section bug005 Bug 005 - [Erreur compilation] appel � bflag

<div style="color:green">Version</div> 
0.4

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
Erreur de compilation lors de l'appel � la m�thode bflag(int) de IbexDefaultSolver.

<div style="color:green">Programme test</div> 

\code
  IbexSystem sys(...);

  IbexDefaultSolver solver(sys);
  solver.explore();
  if (solver.bflag(0)==IBEX_UNIQUE) ...
\endcode

Output:<br>
Erreur de compilation
\verbatim
error: cannot convert ~INTERVAL_VECTOR~ to ~IbexBoxFlag~ in return
\endverbatim

<div style="color:green">Fix� dans la version </div> 
0.5

<div style="color:green">Note interne</div> 
Le type du champ bflags dans IbexSolver a �t� remplac� par vector<IbexBoxFlag>.

\section bug006 Bug 006 - [Erreur compilation] Equation x=0

<div style="color:green">Version</div> 
0.4

<div style="color:green">Soumis par</div> 
Gilles Chabert

<div style="color:green">Description</div> 
L'�quation x=0 ne peut �tre construite

<div style="color:green">Programme test</div> 

\code
  IbexSymbol x("x");        
  const IbexExpr& exr(x=0);
\endcode

Output:<br>
Erreur de compilation
\verbatim
ambiguous overload for ~operator=~ in ~x = 0~
\endverbatim

<div style="color:green">Fix� dans la version </div> 
0.5

<div style="color:green">Note interne</div> 
0 est cast� de fa�on intempestive en char* puis convertit en IbexSymbol via le constructeur 
IbexSymbol(const char*). Il en r�sulte une ambig�it� entre IbexSymbol::operator=(const IbexSymbol&)
et IbexSymbol::operator=(const INTERVAL&). Le constructeur impliqu� a donc �t� mis explicit.

\section bug007 Bug 007 - [Ex�cution inattendue] Contraintes d�sordonn�es avec IbexDefaultBuilder

<div style="color:green">Version</div> 
0.4

<div style="color:green">Soumis par</div> 
Gilles Chabert

<div style="color:green">Description</div> 
Les contraintes ne sont pas num�rot�es dans le syst�me suivant leur ordre d'ajout
dans le builder (ou ordre d'apparition dans le fichier).

<div style="color:green">Programme test</div> 

\code
  IbexSystem csp("ponts.txt");
  IbexRepository& rep(csp.rep);

  for (int i=0; i<csp.nb_ctr(); i++)
    IbexTernaryEncoder(rep,rep.get_syntax(i)).encode();

  IbexDefaultBuilder builder(rep);
  for (int j=csp.nb_ctr(); j<rep.nb_expr(); j++)
    builder.add_ctr(j);            

  for (int j=0; j<rep.nb_entities(); j++)
    builder.set_entity(j,IBEX_VAR);
  
  IbexSystem sys(builder);
  sys.set_output_flags(16);
  cout << sys << endl;
\endcode

Output:
Contraintes 193 -> 221 puis 30 -> 192

<div style="color:green">Fix� dans la version </div> 
0.5

<div style="color:green">Note interne</div> 
La structure hash_set<int> (non ordonn�e) de IbexDefaultBuilder a �t� remplac�e par set<int> (ordonn�e).

\section bug008 Bug 008 - [Fuites m�moires] version 0.5

<div style="color:green">Version</div> 
0.5

<div style="color:green">Soumis par</div> 
Bertrand Neveu & Gilles Chabert

<div style="color:green">Description</div> 
Diverses fuites m�moires

<div style="color:green">Programme test</div> 

\code
  /* first memory leak problem (destructor of class IbexSystem uncomplete) */
  IbexSystem sys("ponts.txt");

  /* other memory leak problem, when build() is not called with an IbexDefaultBuilder object */
  IbexSymbol x("x");
  IbexRepository rep;
  rep.add(x=0);
  IbexDefaultBuilder builder(rep);
  builder.set_entity(0,IBEX_VAR);
\endcode

Output:
Valgrind r�v�le un grand nombre de memory leaks

<div style="color:green">Fix� dans la version </div> 
0.6

<div style="color:green">Note interne</div> 
Les destructeurs des classes IbexSystem, IbexAdjacency, IbexSystemBuilder, IbexExpr ont �t� modifi�s
en cons�quence.

\section bug009 Bug 009 - [Erreur m�moire] Contrainte sans variable

<div style="color:green">Version</div> 
0.5

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
La m�thode decompose de IbexSystem provoque un seg fault si une contrainte
ne contient pas de variable

<div style="color:green">Programme test</div> 

\code
 IbexSystem sys("ponts.txt");
 IbexSubSystemBuilder builder (sys);

 builder.set_entity (IBEX_VAR,29,IBEX_SYB);
 builder.add_all_ctr();

 IbexSystem sysblock (builder);
 sysblock.sync(sys);

 IbexBlockSet bs(30, 1);

 bs.set_blk(sysblock.var("A_y"), 0);
 IbexSystemArray decomp = sysblock.decompose(bs);
\endcode

Output:
Seg fault

<div style="color:green">Fix� dans la version </div> 
0.6

<div style="color:green">Note interne</div> 
Le cas n'avait pas �t� pr�vu: le compteur max_blk restait � -1 dans la boucle
calculant le plus grand bloc auquel appartient une contrainte.

\section bug010 Bug 010 - [Erreur ex�cution] Certification

<div style="color:green">Version</div> 
0.5

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
La certification ne fonctionne pas

<div style="color:green">Programme test</div> 

\code
 IbexSystem sys("ponts.txt");
 IbexDefaultSolver solver(sys);
 solver.certification=true;
 solver.explore();
 cout << solver.bflag(0) << endl;
\endcode

Output:
0 (nothing proven)

<div style="color:green">Fix� dans la version </div> 
0.6

<div style="color:green">Note interne</div> 
La eps-inflation dans la m�thode certify ne testait pas la bonne
inclusion de bo�tes:
La bo�te en argument A �tait �largie en B puis contract� en C.
On testait alors "C incluse dans A", ce qui avait tr�s peu de chance de r�ussir
compte tenu des flottants. Il fallait remplacer par "C incluse dans B" puis remplacer
A par C. La bo�te en argument est donc devenue in-out.

\section bug011 Bug 011 - [Erreur m�moire] Chargement fichier inconnu IbexSystem

<div style="color:green">Version</div> 
0.5

<div style="color:green">Soumis par</div> 
Gilles Chabert

<div style="color:green">Description</div> 
Une erreur de type "unknown file" provoque un seg fault.
De plus il y a une fuite m�moire.

<div style="color:green">Programme test</div> 

\code
 IbexSystem sys("ponts");
\endcode

Output:
seg fault

<div style="color:green">Fix� dans la version </div> 
0.6

<div style="color:green">Note interne</div> 
Le destructeur de la classe IbexAdjacency �tait appel� lors de la lev�e de l'exception
(� l'int�rieur du constructeur de IbexSystem) alors que l'objet n'est pas encore initialis�.
La condition _nb_ctr>0 a �t� ajout�e dans le destructeur d'IbexAdjacency pour �viter ce cas.
La fuite m�moire est li�e au champ IbexRepository qui �tait non d�truit en cas d'exception.

\section bug012 Bug 012 - [Erreur ex�cution] maxdelta & maxratiodelta

<div style="color:green">Version</div> 
0.6

<div style="color:green">Soumis par</div> 
Gilles Chabert

<div style="color:green">Description</div> 
maxdelta et maxratiodelta font un "min" au lieu d'un "max"

<div style="color:green">Programme test</div> 

\code
  INTERVAL_VECTOR box1(3);
  INTERVAL_VECTOR box2(3);
  box1(1)=INTERVAL(0,0);
  box2(1)=INTERVAL(0,0);
  box1(2)=INTERVAL(-1,0);
  box2(2)=INTERVAL(-1,0);
  box1(3)=INTERVAL(1,4);
  box2(3)=INTERVAL(1.5,3);
  cout << box1.maxratiodelta(box2) << endl;
\endcode

Output:
 0 (au lieu de 0.5)

<div style="color:green">Fix� dans la version </div> 
0.7

<div style="color:green">Note interne</div> 
Petite inversion dans le code des fonctions.

\section bug013 Bug 013 - [Erreur ex�cution] restart

<div style="color:green">Version</div> 
0.6

<div style="color:green">Soumis par</div> 
Gilles Chabert

<div style="color:green">Description</div> 
Le restart de IbexDefaultSolver ne semble pas r�initialiser la recherche
avec la bo�te initiale.

<div style="color:green">Programme test</div> 

\code
  IbexSystem sys("ponts.txt");

  IbexDefaultSolver solver(sys);
  solver.explore();
  solver.report();

  solver.explore(); // call restart()
  solver.report();
\endcode

Output:
\verbatim
found 0 inner box(es) found 128 border box(es)
31975 boxes created.
total time   : 10.5047s

found 0 inner box(es) found 0 border box(es) // PROBLEM!
1 boxes created.
total time   : 0s
\endverbatim

<div style="color:green">Fix� dans la version </div> 
0.7

<div style="color:green">Note interne</div> 
 La bo�te initiale est d�sormais stock�e (champ start_box).

\section bug014 Bug 014 - [Exception inattendue] R�cup�ration timeout

<div style="color:green">Version</div> 
0.7

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
 
Impossible de r�cup�rer l'exception IbexTimeOutException. 
Le catch est "travers�" et l'execution s'arr�te en
"terminate called after throwing an instance of 'IbexTimeOutException'
Aborted"

<div style="color:green">Fix� dans la version </div> 
0.8

<div style="color:green">Note interne</div> 
Oubli de d�claration de l'exception dans le prototype de la m�thode explore()
de IbexDefaultSolver.


\section bug015 Bug 015 - [Erreur Ex�cution] D�composition

<div style="color:green">Version</div> 
1.05

<div style="color:green">Soumis par</div> 
Bertrand Neveu

<div style="color:green">Description</div> 
 
Deux probl�mes: 
- Empty composite exception (unrecoverable)
pour un multisyst�me construit � partir d'un bloc 1x1 dont la variable
n'est pas la premi�re du syst�me initial <br>
- Apparition de bo�tes dans le sous-pavage associ� au contracteur de pr�cision dont la 
 taille est largement au dessus du seuil de pr�cision (le sous-pavage comportant un nombre
de bo�tes beaucoup plus imortant que celui attendu).

<div style="color:green">Fix� dans la version </div> 
1.06

<div style="color:green">Note interne</div> 
- decompose() dans MultiSystem, confusion entre n� de variable et clef<br>
- contract(Cell&, Param&) dans Contractor, le space sauvegard� doit �tre celui obtenu APRES le sync() et non avant.


*/

