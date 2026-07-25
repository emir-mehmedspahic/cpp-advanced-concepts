#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>

class Obrok {
private:
    std::string naziv;
    double cijena;
    std::string narucioc;
public:
  Obrok(std::string naziv, double cijena, std::string narucioc) : naziv(naziv),
   cijena(cijena), narucioc(narucioc) {}

   virtual ~Obrok() {} // Polimorfizmu je neophodan virtualni destruktor!

   std::string DajNazivObroka() const { return naziv; } 
   double DajCijenuObroka() const { return cijena; }
   std::string DajNarucioca() const { return narucioc; }

   virtual double DajUkupnuCijenu() const { return cijena; }

   virtual void Ispisi() const {
    std::cout << narucioc << " je narucio " << naziv << " i treba platiti "
                  << DajUkupnuCijenu() << " KM.\n";
   }

   virtual Obrok* DajKopiju() const {
    return new Obrok(*this);
   }
};

class ObrokSaPicem : public Obrok {
private:
    std::string naziv_pica;
    double cijena_pica;

public:
   //U izvedenoj klasi, u konstruktoru, pozvali konstruktor iz prethodne klase.
    ObrokSaPicem(std::string naziv_obroka, double cijena_obroka, std::string narucioc,
                  std::string naziv_pica, double cijena_pica) :
                  Obrok(naziv_obroka, cijena_obroka, narucioc), naziv_pica(naziv_pica), cijena_pica(cijena_pica) {}

    std::string DajNazivPica() const { return naziv_pica; }
    double DajCijenuPica() const { return cijena_pica; }

    double DajUkupnuCijenu() const override {
        std::cout << DajCijenuObroka() + cijena_pica;
    }

    void Ispisi() const override {
        std::cout << DajNarucioca() << " je narucio" << DajNazivObroka() << " sa picencem "
        << naziv_pica << " i treba platiti " << DajUkupnuCijenu() << " KM.\n";
    }

    //POLIMORFNO KOPIRANJE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    Obrok* DajKopiju() const override {
        return new ObrokSaPicem(*this);
    }
};

class Narudzbe {
    private:
    int broj_narudzbi, kapacitet;
    Obrok **obroci;

    void Unisti() {
        if(obroci!=nullptr) {
            for(int i=0; i<broj_narudzbi; i++) delete obroci[i];
            delete[] obroci;
        }
        obroci=nullptr;
    }
public:
Narudzbe() : broj_narudzbi(0), kapacitet(20), obroci(new Obrok*[kapacitet]) {}

~Narudzbe() {
    Unisti();
}

Narudzbe(const Narudzbe &n) : broj_narudzbi(n.broj_narudzbi),
                              kapacitet(n.kapacitet),
                              obroci(new Obrok*[kapacitet]) {
     for(int i=0; i<broj_narudzbi; i++) obroci[i] = n.obroci[i]->DajKopiju();
   }

     //Pomjerajuci konstruktor
     Narudzbe(Narudzbe &&n) : broj_narudzbi(n.broj_narudzbi),
                              kapacitet(n.kapacitet),
                              obroci(n.obroci) {
     n.obroci=nullptr;
     n.broj_narudzbi=0;
     }


     //Kopirajuci operator dodjele
Narudzbe &operator=(const Narudzbe &n) {
    if(this==&n) return *this;

    Obrok** novi_obroci = new Obrok*[n.kapacitet];
    try {
        for(int i=0; i<n.broj_narudzbi; i++) {
            novi_obroci[i] = n.obroci[i] -> DajKopiju();
        }
    } catch(...) {
        delete[] novi_obroci;
        throw;
    }

    Unisti();

    obroci = novi_obroci;
    broj_narudzbi = n.broj_narudzbi;
    kapacitet = n.kapacitet;
    return *this;
}

     //Pomjerajuci operator dodjele

Narudzbe &operator=(Narudzbe &&n) {
    if(this==&n) return *this;
    Unisti();

    obroci = n.obroci;
    broj_narudzbi = n.broj_narudzbi;
    kapacitet = n.kapacitet;
    return *this;
}

//Modifikacija za auto realociranje
void NaruciObrok(std::string naziv, double cijena, std::string narucioc) {
    if(broj_narudzbi>=kapacitet) {
        kapacitet+=20;
        Obrok** novi_prostor = new Obrok*[kapacitet];
        std::copy(obroci, obroci + broj_narudzbi, novi_prostor);
        delete[] obroci;
        obroci = novi_prostor;
    }
    obroci[broj_narudzbi++] = new Obrok(naziv, cijena, narucioc);
}

void NaruciObrokSaPicem(std::string naziv_obroka, double cijena_obroka, 
                        std::string narucioc, std::string naziv_pica, 
                        double cijena_pica) {
 if(broj_narudzbi>=kapacitet) {
        kapacitet+=20;
        Obrok** novi_prostor = new Obrok*[kapacitet];
        std::copy(obroci, obroci + broj_narudzbi, novi_prostor);
        delete[] obroci;
        obroci = novi_prostor;
    }   
    obroci[broj_narudzbi++] = new ObrokSaPicem(naziv_obroka, cijena_obroka, narucioc,naziv_pica, cijena_pica);                        
}

const Obrok &operator[] (int redni_broj) const {
    if(redni_broj<0 || redni_broj >= broj_narudzbi) throw std::domain_error("Neispravan indeks!");
    return *obroci[redni_broj];
}

void ObradiNarudzbu() {
    if(broj_narudzbi==0) throw std::domain_error("Nema ih za obradu");

    obroci[0]->Ispisi();
    delete obroci[0];

    for(int i=0; i<broj_narudzbi-1; i++) obroci[i] = obroci[i+1];
    broj_narudzbi--;
}

bool operator !() const {
    return broj_narudzbi==0;
}

Narudzbe operator+(const Narudzbe &n) const {
    Narudzbe rez;

    rez.kapacitet = this->kapacitet + n.kapacitet;
    delete[] rez.obroci;
    rez.obroci = new Obrok*[rez.kapacitet];
    // Kopiranje iz prvog objekta
    for (int i = 0; i < this->broj_narudzbi; i++) {
        rez.obroci[rez.broj_narudzbi++] = this->obroci[i]->DajKopiju();
    }
    // Kopiranje iz drugog objekta
    for (int i = 0; i < n.broj_narudzbi; i++) {
        rez.obroci[rez.broj_narudzbi++] = n.obroci[i]->DajKopiju();
    }
    return rez;
}

double operator()(std::string ime_narucioca) const {
    double suma=0;
    for(int i=0; i<broj_narudzbi; i++) 
        if(obroci[i]->DajNarucioca() == ime_narucioca) suma += obroci[i]->DajUkupnuCijenu();

    return suma;
};
};


int main() {
    return 0;
}