#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    // 1. ESTRUCTURAS DE DATOS
    string nombresListas[7] = {"BLANCA", "VERDE", "ROJA", "AZUL", "ROJA Y BLANCA", "VERDE Y BLANCA", "AZUL Y BLANCA"};
    
    // Matriz de candidatos (7 listas x 10 candidatos)
    string candidatos[7][10];
    for(int i=0; i<7; i++) {
        for(int j=0; j<10; j++) {
            candidatos[i][j] = "Candidato " + to_string(j+1) + " L" + to_string(i+1);
        }
    }

    int votosListas[7] = {0}; 
    int votosEdades[7][4] = {0}; // 4 rangos de edad por cada una de las 7 listas
    int votosEnBlanco = 0, votosNulos = 0, totalValidos = 0;
    int bancasObtenidas[7] = {0};

    srand(time(0));

    // 2. LOTE DE PRUEBAS (Simulación de votos y edades)
    for (int i = 0; i < 3000; i++) {
        int voto = rand() % 10; 
        int edad = rand() % 60 + 16; // Edades entre 16 y 75

        if (voto == 0) {
            votosEnBlanco++;
        } else if (voto >= 1 && voto <= 7) {
            votosListas[voto - 1]++;
            totalValidos++;
            
            // Clasificación por edades
            if (edad <= 18) votosEdades[voto-1][0]++;
            else if (edad <= 30) votosEdades[voto-1][1]++;
            else if (edad <= 50) votosEdades[voto-1][2]++;
            else votosEdades[voto-1][3]++;
        } else {
            votosNulos++;
        }
    }

    // 3. SISTEMA D'HONT
    float pisoMinimo = totalValidos * 0.03; // Piso del 3%
    float cocientes[7][13] = {0};

    // Preparar cocientes solo para los que superan el 3%
    for (int i = 0; i < 7; i++) {
        if (votosListas[i] >= pisoMinimo) {
            for (int divisor = 1; divisor <= 13; divisor++) {
                cocientes[i][divisor-1] = (float)votosListas[i] / divisor;
            }
        }
    }

    // Repartir 13 bancas buscando el cociente mayor en cada ronda
    for (int banca = 0; banca < 13; banca++) {
        float maxCociente = -1;
        int listaGanadora = -1;
        int posDivisor = -1;

        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 13; j++) {
                if (cocientes[i][j] > maxCociente) {
                    maxCociente = cocientes[i][j];
                    listaGanadora = i;
                    posDivisor = j;
                }
            }
        }
        
        // Asignar banca y anular ese cociente para que no vuelva a ganar
        bancasObtenidas[listaGanadora]++;
        cocientes[listaGanadora][posDivisor] = -1; 
    }

    // 4. SALIDA POR PANTALLA
    cout << "--- RESULTADOS GENERALES ---" << endl;
    cout << "Total Validos: " << totalValidos << " | Blancos: " << votosEnBlanco << " | Nulos: " << votosNulos << endl;
    cout << "Piso del 3%: " << pisoMinimo << " votos." << endl << endl;

    cout << "--- DISTRIBUCION DE BANCAS (13 en total) ---" << endl;
    for (int i = 0; i < 7; i++) {
        cout << nombresListas[i] << " - Votos: " << votosListas[i] << " - Bancas: " << bancasObtenidas[i] << endl;
        if (bancasObtenidas[i] > 0) {
            cout << "  Candidatos que ingresan:" << endl;
            for (int b = 0; b < bancasObtenidas[i]; b++) {
                cout << "  - " << candidatos[i][b] << endl;
            }
        }
        cout << "------------------------" << endl;
    }

    return 0;
}
