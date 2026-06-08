#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

struct nodo {
    size_t n;               // indice progressivo
    int i,j;                // indici sulla griglia
    double x,y;             // coordinate spaziali
};

void GeneraVettoreNodi(const int N, vector<nodo>& nodi) {
    const string file_da_leggere = "../dati/coords" + to_string(N) + ".txt";
    ifstream file_in(file_da_leggere);

    nodo m;
    while (file_in >> m.n >> m.i >> m.j >> m.x >> m.y) {          // salvo una riga del file in un nodo
        nodi.push_back(m);
    };

    file_in.close();
}

void GeneraNuovoOrdinamento(const vector<nodo>& nodi, vector<nodo>& nodi_ordinati, const int i_min, const int i_max, const int j_min, const int j_max) {
    const int N1 = i_max - i_min + 1;       // dimensioni del rettangolo che sto considerando
    const int N2 = j_max - j_min + 1;

    if (N1 <= 1 && N2 <= 1) {               // caso base: rettangolo non divisibile
        for (const auto& m : nodi) {
            nodi_ordinati.push_back(m);
        }
        return;
    }

    vector<nodo> V1, V2, VS;
    V1.reserve(nodi.size() / 2);
    V2.reserve(nodi.size() / 2);
    VS.reserve(min(N1,N2));
    
    if (N1 >= N2) {
        const int i_hat = i_min + N1 / 2;       // taglio verticale
        for (const auto& m : nodi) {
            if (m.i < i_hat) V1.push_back(m);
            else if (m.i > i_hat) V2.push_back(m);
            else VS.push_back(m);
        }

        GeneraNuovoOrdinamento(V1, nodi_ordinati, i_min, i_hat - 1, j_min, j_max);      // passo ricorsivo
        GeneraNuovoOrdinamento(V2, nodi_ordinati, i_hat + 1, i_max, j_min, j_max);
    }

    else {
        const int j_hat = j_min + N2 / 2;       // taglio orizzontale
        for (const auto& m : nodi) {
            if (m.j < j_hat) V1.push_back(m);
            else if (m.j > j_hat) V2.push_back(m);
            else VS.push_back(m);
        }

        GeneraNuovoOrdinamento(V1, nodi_ordinati, i_min, i_max, j_min, j_hat - 1);
        GeneraNuovoOrdinamento(V2, nodi_ordinati, i_min, i_max, j_hat + 1, j_max);
    }

    for (const auto& m : VS) {
        nodi_ordinati.push_back(m);
    }
}

void EsportaOrdinamento(const int N, const vector<nodo>& nodi_ordinati) {
    const string nome_file = "../dati/ordering" + to_string(N) + ".txt";
    ofstream file_out(nome_file);

    for (size_t i = 0; i< nodi_ordinati.size(); i++) {
        file_out << i << " " << nodi_ordinati[i].n << "\n";
    }

    file_out.close();
}

int main() {
    const int N = 1024;

    vector<nodo> nodi;
    nodi.reserve(N * N);
    vector<nodo> nodi_ordinati;
    nodi_ordinati.reserve(N * N);

    GeneraVettoreNodi(N, nodi);
    GeneraNuovoOrdinamento(nodi, nodi_ordinati, 1, N, 1, N);
    EsportaOrdinamento(N, nodi_ordinati);

    cout << "ok" << endl;

    return 0;
}