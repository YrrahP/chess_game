//*** Solutionnaire version 2, sans les //[ //] au bon endroit car le code est assez différent du code fourni.
#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"

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
//span<Film*> ListeFilms::enSpan() const { return span(elements, nElements); }

void ListeFilms::enleverFilm(const Film* film) {
	for (int i = 0; i < nElements; ++i) {
		if (elements[i] == film) {
			delete elements[i];
			elements[i] = elements[nElements - 1];
			nElements--;
			break;
		}
	}
}

//]

//TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
//[
// Voir la NOTE ci-dessous pourquoi Acteur* n'est pas const.  Noter que c'est valide puisque c'est la struct uniquement qui est const dans le paramètre, et non ce qui est pointé par la struct.

//NOTE: Doit retourner un Acteur modifiable, sinon on ne peut pas l'utiliser pour modifier l'acteur tel que demandé dans le main, et on ne veut pas faire écrire deux versions.
Acteur* ListeFilms::trouverActeur(const string& nomActeur) const {
	for (int i = 0; i < nElements; ++i) {
		auto film = elements[i];
		for (int j = 0; j < film->acteurs.nElements; ++j) {
			auto acteur = film->acteurs.elements[j];
			if (acteur && acteur->nom == nomActeur) {
				return acteur.get();
			}
		}
	}
	return nullptr;
}
//]

//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
Acteur* lireActeur(istream& fichier//[
, ListeFilms& listeFilms//]
)
{
	Acteur acteur = {};
	acteur.nom            = lireString(fichier);
	acteur.anneeNaissance = int(lireUintTailleVariable (fichier));
	acteur.sexe           = char(lireUintTailleVariable(fichier));
	//[
	Acteur* acteurExistant = listeFilms.trouverActeur(acteur.nom);
	if (acteurExistant != nullptr)
		return acteurExistant;
	else {
		cout << "Création Acteur " << acteur.nom << endl;
		return new Acteur(acteur);
	}
	//]
	return {}; //TODO: Retourner un pointeur soit vers un acteur existant ou un nouvel acteur ayant les bonnes informations, selon si l'acteur existait déjà.  Pour fins de débogage, affichez les noms des acteurs crées; vous ne devriez pas voir le même nom d'acteur affiché deux fois pour la création.
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms) {
	// Créez directement un Film* (un pointeur vers Film) avec 'new'.
	// Cela alloue un Film dans le tas sans tenter de le copier.
	Film* filmp = new Film();

	// Initialisez directement les membres de 'filmp'.
	filmp->titre = lireString(fichier);
	filmp->realisateur = lireString(fichier);
	filmp->anneeSortie = int(lireUintTailleVariable(fichier));
	filmp->recette = int(lireUintTailleVariable(fichier));
	filmp->acteurs.nElements = int(lireUintTailleVariable(fichier));

	cout << "Création Film " << filmp->titre << endl;

	// Allocation dynamique pour les acteurs du film.
	filmp->acteurs.elements = make_unique<shared_ptr<Acteur>[]>(filmp->acteurs.nElements);

	// Itération pour remplir les acteurs du film.
	for (int i = 0; i < filmp->acteurs.nElements; i++) {
		// Ici, vous utilisez 'lireActeur' pour obtenir un pointeur vers Acteur,
		// et vous l'enveloppez dans un shared_ptr<Acteur>.
		Acteur* acteurBrut = lireActeur(fichier, listeFilms);
		// Assurez-vous que 'lireActeur' retourne un pointeur valide avant de l'utiliser.
		if (acteurBrut != nullptr) {
			filmp->acteurs.elements[i] = shared_ptr<Acteur>(acteurBrut);
		}
	}

	// Retournez le pointeur vers le Film nouvellement alloué et initialisé.
	return filmp;
}


ListeFilms::ListeFilms(const string& nomFichier) : possedeLesFilms_(true)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	
	int nElements = int(lireUintTailleVariable(fichier));

	for ([[maybe_unused]] int i : range(nElements)) {
		ajouterFilm(
		lireFilm(fichier
		, *this));
	}
}

//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.
//[
void detruireActeur(Acteur* acteur)
{
	cout << "Destruction Acteur " << acteur->nom << endl;
	delete acteur;
}
//bool joueEncore(const Acteur* acteur)
//{
//	return acteur->joueDans.size() != 0;
//}
void detruireFilm(Film* film) {
	cout << "Destruction Film " << film->titre << endl;
	delete film;
}
//]

//TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.
//[
//NOTE: Attention que c'est difficile que ça fonctionne correctement avec le destructeur qui détruit la liste.  Mon ancienne implémentation utilisait une méthode au lieu d'un destructeur.  Le problème est que la matière pour le constructeur de copie/move n'est pas dans le TD2 mais le TD3, donc si on copie une liste (par exemple si on la retourne de la fonction creerListe) elle sera incorrectement copiée/détruite.  Ici, creerListe a été converti en constructeur, qui évite ce problème.
ListeFilms::~ListeFilms() {
	if (possedeLesFilms_) {
		for (int i = 0; i < nElements; ++i) {
			delete elements[i];
		}
	}
	delete[] elements;
}
//]

void afficherActeur(const Acteur& acteur)
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

//TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
//[
void afficherFilm(const Film& film)
{
	cout << "Titre: " << film.titre << endl;
	cout << "  Réalisateur: " << film.realisateur << "  Année :" << film.anneeSortie << endl;
	cout << "  Recette: " << film.recette << "M$" << endl;

	cout << "Acteurs:" << endl;
}
//]

std::ostream& operator<<(std::ostream& os, const Film& film) {
	os << "Titre: " << film.titre << '\n';
	os << "  Réalisateur: " << film.realisateur << "  Année: " << film.anneeSortie << '\n';
	os << "  Recette: " << film.recette << "M$" << '\n';
	os << "Acteurs:\n";
	for (int i = 0; i < film.acteurs.nElements; ++i) {
		// Ici, nous supposons que `elements` est un tableau de pointeurs vers des objets Acteur gérés par shared_ptr.
		shared_ptr<Acteur> acteur = film.acteurs.elements[i];
		if (acteur) { // Vérifiez que le pointeur partagé contient bien quelque chose avant de déréférencer.
			os << "  " << acteur->nom << ", " << acteur->anneeNaissance << " " << acteur->sexe << '\n';
		}
	}
	return os;
}


Film* ListeFilms::getFilmAt(int index) const {
	if (index >= 0 && index < nElements) {
		return elements[index];
	}
	return nullptr;
}

void ListeFilms::afficherListeFilms() const {
	static const string ligneDeSeparation = "\033[32m────────────────────────────────────────\033[0m\n";
	cout << ligneDeSeparation;
	for (int i = 0; i < this->nElements; i++) {
		const Film* film = this->elements[i];
		afficherFilm(*film); // Assurez-vous que afficherFilm est accessible
		cout << ligneDeSeparation;
	}
}

//void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur)
//{
//	//TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
//	const Acteur* acteur = //[
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

	int* fuite = new int; //TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Fuite detectee" de "4 octets" affiché à la fin de l'exécution, qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	//TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	//TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).
	ListeFilms listeFilms("films.bin");

	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	//TODO: Afficher le premier film de la liste.  Devrait être Alien.
	//[
	if (listeFilms.size() > 0) { // Vérifiez qu'il y a au moins un film dans la liste.
		const Film* premierFilm = listeFilms.getFilmAt(0); // Obtenez le premier film.
		if (premierFilm != nullptr) { // Vérifiez que le pointeur n'est pas nul.
			afficherFilm(*premierFilm); // Utilisez afficherFilm pour afficher le premier film.
		}
	}
	//]

	cout << ligneDeSeparation << "Les films sont:" << endl;
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.
	//[
	listeFilms.afficherListeFilms();

	//]

	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	//[
	listeFilms.trouverActeur("Benedict Cumberbatch")->anneeNaissance = 1976;
	//]

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	//TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	//[
	/*afficherFilmographieActeur(listeFilms, "Benedict Cumberbatch");*/
	//]

	//chap 7-8

	Film skylien = *listeFilms[0];


	skylien.titre = "Skylien";


	shared_ptr<Acteur> premierActeurSkyfall = listeFilms[1]->acteurs.elements[0];
	skylien.acteurs.elements[0] = premierActeurSkyfall;


	skylien.acteurs.elements[0]->nom = "Daniel Wroughton Craig";


	cout << skylien << *(listeFilms[0]) << *(listeFilms[1]);




	//TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.
	//[
	if (listeFilms.size() > 0) { // Vérifiez qu'il y a au moins un film dans la liste.
		Film* premierFilm = listeFilms.getFilmAt(0); // Obtenez le premier film.
		// Puisque enleverFilm va déjà supprimer le film, aucun besoin d'appeler detruireFilm ici.
		listeFilms.enleverFilm(premierFilm); // Enlevez le premier film de la liste.
	}

	//]

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	//TODO: Afficher la liste des films.
	//[
	listeFilms.afficherListeFilms();






	//chap 10

	/*ListeFilms listeFilms;*/

	Film* film955M = listeFilms.trouverFilmSelonCritere([](const Film& film) {
		return film.recette == 955; // Critère : recette de 955 millions de dollars
		});

	if (film955M != nullptr) {
		std::cout << "Film trouvé : " << film955M->titre << std::endl;
	}
	else {
		std::cout << "Aucun film trouvé avec une recette de 955M$." << std::endl;
	}

	return 0;


}





//chap 9





