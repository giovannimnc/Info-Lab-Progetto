#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <functional>

using namespace std;

void GeneraIndiciBordo(const int N, vector<vector<int>>& mappa) {
    // associo indici tra -1 e -(4N+4) ai punti del bordo della griglia (N+2)x(N+2)
    
    for (int j=0; j <= N+1; j++) {      // lato sinistro
        mappa[0][j] = -j - 1;
    };

    for (int i=1; i <= N+1; i++) {      // lato in alto
        mappa[i][N+1] = - i - (N+2);
    };

    for (int j=0; j <= N; j++) {        // lato destro
        mappa[N+1][j] = j - 3*N - 4;
    };

    for (int i=1; i <= N; i++) {        // lato in basso
        mappa[i][0] = i - 4*N - 5;
    };
}

void GeneraMappaNaturale(const int N, vector<vector<int>>& mappa_nat, vector<pair<int,int>>& mappa_nat_inv) {
    const string file_da_leggere = "../dati/coords" + to_string(N) + ".txt";
    ifstream file_in(file_da_leggere);

    int n, i, j;
    double d;

    while (file_in >> n >> i >> j >> d >> d) {       // mappa tra (i,j) interni e n, e sua inversa
        mappa_nat[i][j] = n;
        mappa_nat_inv.push_back({i,j});
    };

    file_in.close();

    GeneraIndiciBordo(N, mappa_nat);
}

void GeneraMappaOrdinata(const int N, 
    vector<vector<int>>& mappa_ord, 
    vector<pair<int,int>>& mappa_ord_inv, 
    const vector<pair<int,int>>& mappa_nat_inv) {
        
    const string file_da_leggere = "../dati/ordering" + to_string(N) + ".txt";
    ifstream file_in(file_da_leggere);

    int m, n;

    while (file_in >> m >> n) {
        auto [i,j] = mappa_nat_inv[n];
        mappa_ord[i][j] = m;
        mappa_ord_inv.push_back({i,j});
    };

    file_in.close();

    GeneraIndiciBordo(N, mappa_ord);
}

void GeneraEsportaMatriceRhs(const int N,
    const string& tipo_ordinamento,         // "nat" o "ord"
    const vector<vector<int>>& mappa, 
    const vector<pair<int,int>>& mappa_inv,
    const function<double(double,double)>& f,       // sorgente di calore
    const double k = 0.01,                          // diffusività
    const vector<double>& valori_bordo = {}) {

    vector<double> u0 = valori_bordo;        // di default i valori sul bordo sono 0
    if (u0.empty()) {
        u0.resize(4*N+4, 0.0);
    };

    const string nome_file_A = "../dati/A" + to_string(N) + tipo_ordinamento + ".txt";
    ofstream file_A(nome_file_A);

    const string nome_file_rhs = "../dati/rhs" + to_string(N) + tipo_ordinamento + ".txt";
    ofstream file_rhs(nome_file_rhs);

    const double h = 1.0 / (N+1);       // passo della griglia 

    for (int l = 0; l <= N*N - 1; l++) {        // ciclo sulle righe del sistema
        auto [i,j] = mappa_inv[l];
        double b = -f(i*h, j*h);        // primo addendo del termine noto

        file_A << l << " " << l << " " << -4.0 * k / (h*h) << "\n";       // entrata (l,l) di A

        const int d_i[4] = {1,-1,0,0};
        const int d_j[4] = {0,0,1,-1};

        for (int v = 0; v <= 3; v++) {
            int m = mappa[i + d_i[v]][j + d_j[v]];
            if (m >= 0) {
                file_A << l << " " << m << " " << k / (h*h) << "\n";      // è un nodo interno, quindi va in A (entrata (l,m))
            }
            else {
                b = b - k * u0[-m - 1] / (h*h);             // nodo di bordo, va in rhs
            }
        }

        file_rhs << b << "\n";      // coefficiente l del rhs

    }

    file_A.close();
    file_rhs.close();
}

int main() {
    const int N = 4;

    auto sorgente = [](double x, double y) {
        return exp(-10.0 * (x * x + y * y));
    };

    vector<vector<int>> mappa_nat(N + 2, vector<int>(N + 2, -1));
    vector<pair<int,int>> mappa_nat_inv;
    mappa_nat_inv.reserve(N * N);

    vector<vector<int>> mappa_ord(N + 2, vector<int>(N + 2, -1));
    vector<pair<int,int>> mappa_ord_inv;
    mappa_ord_inv.reserve(N * N);

    GeneraMappaNaturale(N, mappa_nat, mappa_nat_inv);
    GeneraMappaOrdinata(N, mappa_ord, mappa_ord_inv, mappa_nat_inv);

    GeneraEsportaMatriceRhs(N, "nat", mappa_nat, mappa_nat_inv, sorgente);    
    GeneraEsportaMatriceRhs(N, "ord", mappa_ord, mappa_ord_inv, sorgente);

    cout << "ok"<<endl;

    return 0;
}