#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct nodo {
    size_t n;              // indice progressivo
    int i,j;            // indici sulla griglia
    double x,y;         // coordinate spaziali
};

void GeneraGriglia(const int N, vector<nodo>& nodi, vector<vector<int>>& griglia) {
    const double h = 1.0 / (N+1);       // passo della griglia
    size_t ind_progr = 0;                  // indice n che numera i nodi

    for (int i = 1; i<=N; i++) {        // ciclo solo sui nodi interni
        for (int j = 1; j<=N; j++) {
            nodo nodo_corrente;
            nodo_corrente.n = ind_progr;
            nodo_corrente.i = i;
            nodo_corrente.j = j;
            nodo_corrente.x = i*h;
            nodo_corrente.y = j*h;

            nodi.push_back(nodo_corrente);  // aggiunto in fondo al vettore dei nodi 
            griglia[i][j] = ind_progr;

            ind_progr++;

        }
    }
};

void EsportaPuntiInterni(const int N, const vector<nodo>& nodi) {
    const string nome_file = "../dati/coords" + to_string(N) + ".txt";    // "../dati" torna indietro alla cartella radice
    ofstream file_out(nome_file);

    for (const auto& m : nodi) {
        file_out << m.n << " " << m.i << " " << m.j << " " << m.x << " " << m.y << "\n";

    }

    file_out.close();
};

void GeneraEsportaGrafo(const int N, const vector<vector<int>>& griglia, vector<vector<int>>& lista_adiacenza) {
    const string nome_file = "../dati/connectivity" + to_string(N) + ".txt";    
    ofstream file_out(nome_file);

    size_t e = 0;      // indice progressivo che numera gli archi

    for (int i = 1; i<=N; i++) {
        for (int j = 1; j<=N; j++) {
            const auto a = griglia[i][j];
            const auto b = griglia[i+1][j];   // vicino a destra
            const auto c = griglia[i][j+1];   // vicino in alto

            if (b != -1) {          // se il nodo (i+1,j) non è di bordo, aggiungo alla lista di adiacenza e scrivo l'arco su file
                lista_adiacenza[a].push_back(b);
                lista_adiacenza[b].push_back(a);

                file_out << e << " " << a << " " << b << "\n";
                e++;
            }

            if (c != -1) {
                lista_adiacenza[a].push_back(c);
                lista_adiacenza[c].push_back(a);

                file_out << e << " " << a << " " << c << "\n";
                e++;
            }
        }
    }

    file_out.close();
};



int main() {
    const int N = 32;

    vector<nodo> nodi;
    vector<vector<int>> griglia(N + 2, vector<int>(N + 2, -1));
    vector<vector<int>> lista_adiacenza(N * N);

    // GeneraGriglia(N, nodi, griglia);
    // EsportaPuntiInterni(N, nodi);
    // GeneraEsportaGrafo(N, griglia, lista_adiacenza);

    cout <<"aa"<<endl;

    return 0;
}