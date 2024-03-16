#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include "cppitertools/range.hpp"

using gsl::span;
using namespace iter ;
using namespace std; 

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront définis après.

class ListeFilms {
public:
	ListeFilms();
	ListeFilms(const string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	Film& operator[] (const int index) const { return *elements[index]; }
	
    template <typename Comparateur , typename typeCritere>
	Film trouverFilm(const typeCritere& valeurComparaison, const Comparateur& verifierCritere);


private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

template <typename T> 
struct Liste {
	// Attributs de la struct
	int capacite , nElements ;
	unique_ptr <shared_ptr<T>[]> elements;

	// Constructeurs 
	Liste() {
		capacite = 0; 
		nElements = 0;
		elements = nullptr;
	};


	Liste(int nElements): Liste()
	{
		this->capacite = this->nElements = nElements;
		elements = make_unique <shared_ptr<T>[]>(nElements);
		for (int i : range(nElements)) {
			elements[i] = make_shared <T> (T())  ;
		}
	}
	
	shared_ptr<T>& operator[] (const int index) const { return elements[index] ;}

	Liste& operator=(const Liste<T>& autre){
		capacite = nElements = autre.nElements; 
		elements = make_unique <shared_ptr<T>[]>(autre.nElements) ;
		for (int i : range(nElements)) {
			elements[i] = autre[i]; // copie du shared ptr (pas de nouvelle allocation)
		}
		return *this; 
	}

	Liste(Liste<T>& autre) { // constructeur de copie 
		*this = autre;
	}
};

using ListeActeurs = Liste<Acteur>;

const string inconnu = "INCONNU"; 
const int chiffreInconnu = 0;

struct Film

{
	string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie{0}, recette{0}; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;

	Film() :
		titre(inconnu), realisateur(inconnu), anneeSortie(chiffreInconnu), recette(chiffreInconnu)
	{}
	// constructeur de copie
	Film(const Film& autre) {
		*this = autre;
	}

	// operateur "="
	Film& operator=(const Film& autre) {
		if (this != &autre) {

			// initialiser les types primitifs
			titre = autre.titre; realisateur = autre.realisateur; 
			anneeSortie = autre.anneeSortie; recette = autre.recette;
			/*Deep copy de Liste Acteurs*/
			int nActeurs = autre.acteurs.nElements ;
			this->acteurs = ListeActeurs(nActeurs); // allocation memoire pour un nouveau tableau de shared ptr (pas de copie de unique_ptr)
			
			unique_ptr <shared_ptr<Acteur>[]>& tableauPtrActeursMonObjet = (this->acteurs).elements; // nouveau tableau de shared ptrs
			const unique_ptr <shared_ptr<Acteur>[]>& tableauPtrActeursAutre = autre.acteurs.elements; // tableau de shared ptrs de l'autre film

			// Allouer de la memoire pour un nouvel objet acteur pour chaque acteur de l'autre objet
			for (int i : range(nActeurs) ){
				Acteur& acteurAutre = *(tableauPtrActeursAutre[i]);
				tableauPtrActeursMonObjet[i] = make_shared <Acteur>(acteurAutre); // allocation memoire pour le meme acteur autre part en memoire
			}
		}
		return *this; 
	}
	friend ostream& operator<< (ostream& os, const Film& film);


};

ostream& operator<< (ostream& os, const Film& film) {
	os << "Titre: " << film.titre << endl;
	os << "  Realisateur: " << film.realisateur << "  Annee :" << film.anneeSortie << endl;
	os << "  Recette: " << film.recette << "M$";
	return os;
}


struct Acteur
{
	string nom = ""; int anneeNaissance{ 0 }; char sexe = 'X';

	//ListeFilms joueDans;
};
