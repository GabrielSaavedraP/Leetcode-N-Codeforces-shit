#include <iostream>
#include <vector>
#include <string>
using namespace std;

template <typename TipoClave, typename TipoValor>
struct my_map {

    struct Par {
        TipoClave clave;
        TipoValor valor;
        Par(const TipoClave& k, const TipoValor& v) : clave(k), valor(v) {}
    };

    int num_cubetas;
    int total;
    vector<vector<Par>> cubetas;

    my_map(int cubetas_iniciales = 8) : num_cubetas(cubetas_iniciales), total(0) {
        cubetas.resize(num_cubetas);
    }

    // ========================================================
    // HASH (sobrecargado: el compilador elige según el tipo de clave)
    // ========================================================

    // Claves numéricas (int, long long, char)
    int _hash(long long clave) const {
        const int BASE = 311;
        const int MOD = 1e9 + 7;

        long long k = clave < 0 ? -clave : clave;
        int h = 0;

        if (k == 0) h = 1;
        while (k > 0) {
            int digito = k % 10;
            h = (1LL * h * BASE + (digito + 1)) % MOD;
            k /= 10;
        }

        if (clave < 0) {
            h = (1LL * h * BASE + 7) % MOD;
        }

        int indice = h % num_cubetas;
        if (indice < 0) indice += num_cubetas;
        return indice;
    }

    // Claves string
    int _hash(const string& clave) const {
        const int BASE = 311;
        const int MOD = 1e9 + 7;

        int h = 0;
        for (char c : clave) {
            h = (1LL * h * BASE + (c + 1)) % MOD;
        }

        int indice = h % num_cubetas;
        if (indice < 0) indice += num_cubetas;
        return indice;
    }

    // ========================================================
    // OPERACIONES
    // ========================================================

    // m[x] = v;  m[x]++;  m[x] += v;
    // Crea la clave con valor por defecto si no existía.
    TipoValor& operator[](const TipoClave& clave) {
        if ((double)(total + 1) / num_cubetas > 0.75) {
            _resize(num_cubetas * 2);
        }

        int i = _hash(clave);
        int j = 0;

        while (j < (int)cubetas[i].size() && cubetas[i][j].clave != clave) {
            ++j;
        }

        if (j == (int)cubetas[i].size()) {
            cubetas[i].emplace_back(clave, TipoValor());
            ++total;
        }

        return cubetas[i][j].valor;
    }

    // Elimina la clave (útil en sliding window). Para poner en 0 basta m[x] = 0.
    void borrar(const TipoClave& clave) {
        int i = _hash(clave);
        int j = 0;

        while (j < (int)cubetas[i].size() && cubetas[i][j].clave != clave) {
            ++j;
        }

        if (j != (int)cubetas[i].size()) {
            if (j + 1 < (int)cubetas[i].size()) {
                swap(cubetas[i][j], cubetas[i].back());
            }
            cubetas[i].pop_back();
            --total;
        }
    }

    // ¿Existe la clave? NO la crea (a diferencia de m[x] > 0).
    bool existe(const TipoClave& clave) const {
        int i = _hash(clave);
        int j = 0;

        while (j < (int)cubetas[i].size() && cubetas[i][j].clave != clave) {
            ++j;
        }

        return j != (int)cubetas[i].size();
    }

    // Interna: se dispara sola desde operator[]. Llamarla a mano solo para reservar espacio.
    void _resize(int nuevo_num_cubetas) {
        vector<vector<Par>> cubetas_viejas = cubetas;

        num_cubetas = nuevo_num_cubetas;
        cubetas.clear();
        cubetas.resize(num_cubetas);
        total = 0;

        for (int i = 0; i < (int)cubetas_viejas.size(); ++i) {
            for (int j = 0; j < (int)cubetas_viejas[i].size(); ++j) {
                (*this)[cubetas_viejas[i][j].clave] = cubetas_viejas[i][j].valor;
            }
        }
    }

    // Cantidad de claves distintas
    int size() const { return total; }

    bool empty() const { return total == 0; }

    // Solo para depurar
    void print() {
        for (int i = 0; i < num_cubetas; ++i) {
            cout << "Bucket " << i << ":\n";
            for (auto& par : cubetas[i]) {
                cout << par.clave << " --> " << par.valor << "\n";
            }
            cout << "End bucket\n";
        }
    }
};



class Solution {
public:
    int romanToInt(string s) {
        int total=0;
        my_map<char, int> numeros(2*sizeof(s));
        numeros['I'] = 1;
        numeros['V'] = 5;
        numeros['X'] = 10;
        numeros['L'] = 50;
        numeros['C'] = 100;
        numeros['D'] = 500;
        numeros['M'] = 1000;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (numeros[s[i]] < numeros[s[i+1]]) {
                total-= numeros[s[i]];
            }
            else {
                total+=numeros[s[i]];
            }
        }
        return total;
    }
};