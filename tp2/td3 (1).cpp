//*** Solutionnaire version 2, sans les //[ //] au bon endroit car le code est assez différent du code fourni.
#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures (1).hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <limits>
#include <algorithm>

#include "gsl/span"
#include "cppitertools/range.hpp"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{
template <typename T>
T lireType(istream& fichier)
{
	T valeur{};
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}
#define erreurFataleAssert(message) assert(false&&(message)),terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;
size_t lireUintTailleVariable(istream& fichier)
{
	uint8_t entete = lireType<uint8_t>(fichier);
	switch (entete) {
	case enteteTailleVariableDeBase+0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase+1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase+2: return lireType<uint32_t>(fichier);
	default:
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
	}
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUintTailleVariable(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

//TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.
//[
void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film** nouvelleListe = new Film*[nouvelleCapacite];
	
	if (elements != nullptr) {  // Noter que ce test n'est pas nécessaire puique nElements sera zéro si elements est nul, donc la boucle ne tentera pas de faire de copie, et on a le droit de faire delete sur un pointeur nul (ça ne fait rien).
		nElements = min(nouvelleCapacite, nElements);
		for (int i : range(nElements))
			nouvelleListe[i] = elements[i];
		delete[] elements;
	}
	
	elements = nouvelleListe;
	capacite = nouvelleCapacite;
}

void ListeFilms::ajouterFilm(Film* film)
{
	if (nElements == capacite)
		changeDimension(max(1, capacite * 2));
	elements[nElements++] = film;
}

//]

//TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
//[
// On a juste fait une version const qui retourne un span non const.  C'est valide puisque c'est la struct qui est const et non ce qu'elle pointe.  Ça ne va peut-être pas bien dans l'idée qu'on ne devrait pas pouvoir modifier une liste const, mais il y aurais alors plusieurs fonctions à écrire en version const et non-const pour que ça fonctionne bien, et ce n'est pas le but du TD (il n'a pas encore vraiment de manière propre en C++ de définir les deux d'un coup).
span<Film*> ListeFilms::enSpan() const { return span(elements, nElements); }

void ListeFilms::enleverFilm(const Film* film)
{
	for (Film* (& element) : enSpan()) {  // Doit être une référence au pointeur pour pouvoir le modifier.
		if (element == film) {
			if (nElements > 1)
				element = elements[nElements - 1];
			nElements--;
			return;
		}
	}
}
//]

//TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
//[
span<shared_ptr<Acteur>> spanListeActeurs(const ListeActeurs& liste) { return span(liste.elements.get(), liste.nElements); }

shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const
{
	for (const Film* film : enSpan()) {
		for (const shared_ptr<Acteur>& ptrActeur : spanListeActeurs(film->acteurs)) {
			if (ptrActeur->nom == nomActeur)
				return ptrActeur;
		}
	}
	return nullptr ; // retourne un objet par defaut si l'acteur n'est pas present 
}
//]


//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
shared_ptr<Acteur>  lireActeur(istream& fichier//[
, ListeFilms& listeFilms//]
)
{
	Acteur acteur = {};
	acteur.nom            = lireString(fichier);
	acteur.anneeNaissance = int(lireUintTailleVariable (fichier));
	acteur.sexe           = char(lireUintTailleVariable(fichier));
	//[

	shared_ptr<Acteur> ptrActeur = listeFilms.trouverActeur(acteur.nom);
	bool estActeurExistant = (ptrActeur != nullptr);

	if (!estActeurExistant) {
		cout << "Création Acteur " << acteur.nom << endl;
		ptrActeur = make_shared <Acteur>(acteur); // allouer espace pour le nouvel acteur + initialiser acteur
	}
	return ptrActeur; 
}




Film* lireFilm(istream& fichier//[
, ListeFilms& listeFilms//]
)
{
	Film* filmp = new Film;
	filmp->titre = lireString(fichier);
	filmp->realisateur = lireString(fichier);
	filmp->anneeSortie = int(lireUintTailleVariable(fichier));
	filmp->recette     = int(lireUintTailleVariable(fichier));
	int nActeurs = int(lireUintTailleVariable(fichier));  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
	//[
	cout << "Création Film " << filmp->titre << endl;
	filmp->acteurs = ListeActeurs(nActeurs); 
	/*
	//]
	for (int i = 0; i < film.acteurs.nElements; i++) {
		//[
	*/
	for (shared_ptr<Acteur>& ptrActeur : spanListeActeurs(filmp->acteurs)) {
		ptrActeur =
		//]
		lireActeur(fichier//[
		, listeFilms//]
		); //TODO: Placer l'acteur au bon endroit dans les acteurs du film.
		//TODO: Ajouter le film à la liste des films dans lesquels l'acteur joue.
	//[
		//ptrActeur->joueDans.ajouterFilm(filmp);
	//]
	}
	//[
	return filmp;
	//]
}

ListeFilms::ListeFilms(const string& nomFichier) : possedeLesFilms_(true)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	
	int nTotalFilmes = int(lireUintTailleVariable(fichier));

	//TODO: Créer une liste de films vide.
	//[
	/*
	//]
	for (int i = 0; i < nElements; i++) {
		//[
	*/
	for ([[maybe_unused]] int i : range(nTotalFilmes)) { //NOTE: On ne peut pas faire un span simple avec spanListeFilms car la liste est vide et on ajoute des éléments à mesure.
		ajouterFilm(
		//]
		lireFilm(fichier//[
		, *this  //NOTE: L'utilisation explicite de this n'est pas dans la matière indiquée pour le TD2.
		//]
		)//[
		)
		//]
		; //TODO: Ajouter le film à la liste.
	}
	
	//[
	/*
	//]
	return {}; //TODO: Retourner la liste de films.
	//[
	*/
	//]
}

//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.
//[
//void detruireActeur(Acteur* acteur)
//{
//	cout << "Destruction Acteur " << acteur->nom << endl;
//	delete acteur;
//}

//bool joueEncore(const shared_ptr <Acteur>& acteur)
//{
//	return acteur->joueDans.size() != 0;
//}
void detruireFilm(Film* film)
{
	//for (shared_ptr <Acteur>& acteur : spanListeActeurs(film->acteurs)) {
	//	acteur->joueDans.enleverFilm(film);
		//if (!joueEncore(acteur))
		//	detruireActeur(acteur);
	//}

	cout << "Destruction Film " << film->titre << endl;
	//delete[] film->acteurs.elements; // suppression automatique avec unique ptr
	delete film;
}
//]

//TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.
//[
//NOTE: Attention que c'est difficile que ça fonctionne correctement avec le destructeur qui détruit la liste.  Mon ancienne implémentation utilisait une méthode au lieu d'un destructeur.  Le problème est que la matière pour le constructeur de copie/move n'est pas dans le TD2 mais le TD3, donc si on copie une liste (par exemple si on la retourne de la fonction creerListe) elle sera incorrectement copiée/détruite.  Ici, creerListe a été converti en constructeur, qui évite ce problème.
ListeFilms::~ListeFilms()
{
	if (possedeLesFilms_)
		for (Film* film : enSpan())
			detruireFilm(film);
	delete[] elements;
}
//]


void afficherActeur(Acteur* ptrActeur)
{
	cout << "  " << ptrActeur->nom << ", " << ptrActeur->anneeNaissance << " " << ptrActeur->sexe << endl;
}

//TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
//[
void afficherFilm(const Film& film)
{
	cout << film << endl;

	cout << "Acteurs:" << endl;
	if (film.acteurs.elements != nullptr) {
		for (const shared_ptr<Acteur>& ptrActeur : spanListeActeurs(film.acteurs))
			afficherActeur(ptrActeur.get());
	}
	else {
		cout << "  Aucun acteur repertorie pour ce film\n" ;
	}

}
//]

void afficherListeFilms(const ListeFilms& listeFilms)
{
	//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = //[
		"\033[32m────────────────────────────────────────\033[0m\n";
		/*
		//]
		{};
	//[ */
	//]
	cout << ligneDeSeparation;
	//TODO: Changer le for pour utiliser un span.
	//[
	/*//]
	for (int i = 0; i < listeFilms.nElements; i++) {
		//[*/
	for (const Film* film : listeFilms.enSpan()) {
		//]
		//TODO: Afficher le film.
		//[
		afficherFilm(*film);
		//]
		cout << ligneDeSeparation;
	}
}
template <typename Comparateur, typename typeCritere>
Film ListeFilms::trouverFilm(const typeCritere& valeurComparaison, const Comparateur& verifierCritere) {
	
	bool estTrouvee = false;
	// Recherche de la valeur
	for (int index = 0; index < nElements; index++) {
		if (verifierCritere(index, valeurComparaison)) {
			return *elements[index];
		}
	}
	cout <<  "Aucun film qui verifie ce critere n'est present dans cette liste de filmes ! \n\n" ; 
	return Film();
}

//void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur)
//{
//	//TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
//	const shared_ptr<Acteur> acteur = //[
//		listeFilms.trouverActeur(nomActeur);
//		/* //]
//		nullptr;
//	//[ */
//	//]
//	if (acteur == nullptr)
//		cout << "Aucun acteur de ce nom" << endl;
//	else
//		afficherListeFilms(acteur->joueDans);
//}




int main()
{
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	//int* fuite = new int; //TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Fuite detectee" de "4 octets" affiché à la fin de l'exécution, qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	//TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	//TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).
	ListeFilms listeFilms("films.bin");

	//cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	//cout << ligneDeSeparation << endl;



	// Test Ostream
	cout << ligneDeSeparation << endl;
	cout << "Chapire 7 (Streams) : " << endl << endl;

	cout << ligneDeSeparation << endl;
	cout << listeFilms[0] << endl << listeFilms[2] << endl;
	cout << ligneDeSeparation << endl;

	// Test ofstream
	ofstream ofsTest("output_file_stream.txt");
	ofsTest << listeFilms[0] << endl << listeFilms[2] << endl;
	ofsTest.close();

	// Test sstream
	ostringstream stringFilme;
	stringFilme << listeFilms[0] << endl << listeFilms[2] << endl;
	cout << "Le string stream est : \n" << stringFilme.str() << endl;
	
	// Chapitre 7-8 
	cout << ligneDeSeparation << endl;
	cout << "Chapire 7 - 8 : " << endl << endl;

	Film skylien = listeFilms[0]; 

	skylien.titre = "Skylien"; 

	skylien.acteurs[0] = listeFilms[1].acteurs[0]; 

	skylien.acteurs[0]->nom = "Daniel Wroughton Craig"; 

	afficherFilm(skylien); // film alien avec Craig comme premier acteur
	cout << ligneDeSeparation << endl;
	afficherFilm(listeFilms[0]); // aucun changement
	cout << ligneDeSeparation << endl;
	afficherFilm(listeFilms[1]); // changement du nom de l'acteur en son nom COMPLET "Daniel Wroughton Craig"

	// Chapitre 9
	cout << ligneDeSeparation << endl << endl;
	cout << "Chapire 9: " << endl << endl;
	//Creation Liste1 
	// 
	int nTextes = 2;
	Liste<string> listeTextes(nTextes);

	*listeTextes[0] = "Minato Namikaze";
	*listeTextes[1] = "Hiruzen Sarutobi";

	// Copie Liste1 dans Liste2 
	Liste <string> listeTextes2 = listeTextes;

	// Avant les changements
	cout << "Avant les changements : " << endl << endl;
	cout << "Elements de listeTexte  : " << *listeTextes[0] << " , " << *listeTextes[1] << endl; // Naruto Uzumaki  , Shisui Uchiha
	cout << "Elements de listeTexte2 : " << *listeTextes2[0] << " , " << *listeTextes2[1] << endl << endl; // Minato Namikaze , Shisui Uchiha

	// Remplacer 1er element 
	listeTextes[0] = make_shared <string>("Naruto Uzumaki"); // pointeur vers un nouvel endroit dans la memoire

	// Modifier 2e element
	*listeTextes[1] = "Shisui Uchiha";

	// Apres les changements
	cout << "Apres les changements : " << endl << endl; 
	cout << "Elements de listeTexte  : " << *listeTextes[0] << " , " << *listeTextes[1] << endl; // Naruto Uzumaki  , Shisui Uchiha
	cout << "Elements de listeTexte2 : " << *listeTextes2[0] << " , " << *listeTextes2[1] << endl; // Minato Namikaze , Shisui Uchiha


	// Chapitre 10

	cout << ligneDeSeparation << endl << endl;
	cout << "Chapire 10 : " << endl;

	auto verifierCritere = [&](int indexFilm, auto valeurComparaison) -> bool {return (listeFilms[indexFilm].recette == valeurComparaison); };
	auto valeurCherchee = 955;
	Film filmTest = listeFilms.trouverFilm(valeurCherchee, verifierCritere);
	afficherFilm(filmTest);
	cout << ligneDeSeparation << endl << endl;

	//TODO: Afficher le premier film de la liste.  Devrait être Alien.
	//[
	//afficherFilm(*listeFilms.enSpan()[0]);
	//]

	//cout << ligneDeSeparation << "Les films sont:" << endl;
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.

	//afficherListeFilms(listeFilms);
	//]

	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	//[
	//listeFilms.trouverActeur("Benedict Cumberbatch")->anneeNaissance = 1976;
	//]

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	//TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	//[
	//afficherFilmographieActeur(listeFilms, "Benedict Cumberbatch");
	//]
	//
	////TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.
	////[
	//detruireFilm(listeFilms.enSpan()[0]);
	//listeFilms.enleverFilm(listeFilms.enSpan()[0]);

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;

	//afficherListeFilms(listeFilms);

	//TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme
	//listeFilms.enleverFilm(nullptr); // Enlever un film qui n'est pas dans la liste (clairement que nullptr n'y est pas).

	//afficherFilmographieActeur(listeFilms, "N'existe pas"); // Afficher les films d'un acteur qui n'existe pas.
	//]
}


