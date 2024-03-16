#pragma once
#include <string>
#include <memory>
#include <functional>
#include <cassert>
#include "gsl/span"
using namespace std;
using gsl::span;

struct Film;
struct Acteur;

class ListeFilms {
public:
    ListeFilms() = default;
    ListeFilms(const string& nomFichier);
    ListeFilms(const ListeFilms& l) = delete; // Suppression explicite
    ListeFilms& operator=(const ListeFilms&) = delete; // Suppression explicite de l'opérateur d'assignation de copie
    ~ListeFilms();
    void ajouterFilm(Film* film);
    void enleverFilm(const Film* film);
    Acteur* trouverActeur(const string& nomActeur) const;
    span<Film*> enSpan() const;
    void afficherListeFilms() const;
    Film* getFilmAt(int index) const;

    Film* operator[](int index) {
        if (index >= 0 && index < nElements) {
            return elements[index];
        }
        else {
            return nullptr; // ou lever une exception si vous préférez
        }
    }

    Film* trouverFilmSelonCritere(std::function<bool(const Film&)> critere) const {
        for (int i = 0; i < nElements; ++i) {
            if (critere(*elements[i])) {
                return elements[i];
            }
        }
        return nullptr; // Si aucun film ne correspond au critère
    }

    int size() const { return nElements; }

private:
    void changeDimension(int nouvelleCapacite);

    int capacite = 0, nElements = 0;
    Film** elements = nullptr;
    bool possedeLesFilms_ = false;
};


//struct ListeActeurs {
//    int capacite = 0, nElements = 0;
//    unique_ptr<shared_ptr<Acteur>[]> elements;
//
//    // Constructeur par défaut
//    ListeActeurs() : capacite(0), nElements(0), elements(nullptr) {}
//
//    // Constructeur avec un argument pour la capacité
//    ListeActeurs(int capacite_) : capacite(capacite_), nElements(0), elements(make_unique<shared_ptr<Acteur>[]>(capacite_)) {}
//};



#include <memory>

template <typename T>
struct Liste {
    int capacite, nElements;
    unique_ptr<shared_ptr<T>[]> elements;

    Liste() : capacite(0), nElements(0), elements(nullptr) {}

    Liste(int nElements) : Liste() {
        this->capacite = this->nElements = nElements;
        elements = make_unique<shared_ptr<T>[]>(nElements);
        for (int i = 0; i < nElements; ++i) {
            elements[i] = make_shared<T>();
        }
    }

    shared_ptr<T>& operator[](const int index) const {
        return elements[index];
    }

    Liste& operator=(const Liste& autre) {
        if (this == &autre) return *this;
        capacite = nElements = autre.nElements;
        elements = make_unique<shared_ptr<T>[]>(autre.nElements);
        for (int i = 0; i < nElements; ++i) {
            elements[i] = autre.elements[i];
        }
        return *this;
    }

    Liste(const Liste& autre) : Liste() {
        *this = autre;
    }
};



using ListeActeurs = Liste<Acteur>;



struct Film {
    string titre, realisateur;
    int recette = 0, anneeSortie = 0;
    ListeActeurs acteurs;

    Film() : titre("inconnu"), realisateur("inconnu"), anneeSortie(0), recette(0) {}

   
    Film(const Film& autre) : titre(autre.titre), realisateur(autre.realisateur),
        anneeSortie(autre.anneeSortie), recette(autre.recette),
        acteurs(autre.acteurs.nElements) {
        for (int i = 0; i < acteurs.nElements; ++i) {
            acteurs.elements[i] = make_shared<Acteur>(*autre.acteurs.elements[i]);
        }
    }

   
    Film& operator=(const Film& autre) {
        if (this != &autre) {
            titre = autre.titre;
            realisateur = autre.realisateur;
            anneeSortie = autre.anneeSortie;
            recette = autre.recette;
            acteurs = ListeActeurs(autre.acteurs.nElements);
            for (int i = 0; i < acteurs.nElements; ++i) {
                acteurs.elements[i] = make_shared<Acteur>(*autre.acteurs.elements[i]);
            }
        }
        return *this;
    }

    
    friend ostream& operator<<(ostream& os, const Film& film) {
        os << "Titre: " << film.titre << ", Réalisateur: " << film.realisateur
            << ", Année de sortie: " << film.anneeSortie << ", Recette: " << film.recette << "M$";
        return os;
    }
};


struct Acteur {
    string nom = "";
    int anneeNaissance = 0;
    char sexe = 'U';
//    ListeFilms joueDans;
};

