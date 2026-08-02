#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int cantidadListas = 7;
const int cantidadCandidatos = 10;
const int cantidadBancas = 13;
const int cantidadSimulados = 3000;
const int cantidadEdades = 4;

void cargarListas(string nombresListas[], string candidatos[][cantidadCandidatos])
{
    string nombres[cantidadListas] = {"BLANCA", "VERDE", "ROJA", "AZUL","ROJA Y BLANCA", "VERDE Y BLANCA", "AZUL Y BLANCA"};
    for (int i = 0; i < cantidadListas; i++)
    {
        nombresListas[i] = nombres[i];
        for (int j = 0; j < cantidadCandidatos; j++)
        {
            string tipo = (j < 5) ? "titular" : "suplente";
            candidatos[i][j] = "Candidato " + to_string(j + 1) + " L" + to_string(i + 1) + " (" + tipo + ")";
        }
    }
}
//CHEQUEAR EL PROBLEMA DE LA EDAD
void generarVotos(int votosListas[], int votosEdades[][cantidadEdades], int &votosEnBlanco, int &votosNulos, int &totalValidos)
{
    srand(time(0));
    for (int i = 0; i < cantidadListas; i++)
        votosListas[i] = 0;
    for (int i = 0; i < cantidadListas; i++)
        for (int j = 0; j < cantidadEdades; j++)
            votosEdades[i][j] = 0;
            votosEnBlanco = 0;
            votosNulos = 0;
            totalValidos = 0;
    for (int i = 0; i < cantidadSimulados; i++)
    {
        int voto = rand() % 10;
        int edad = rand() % 60 + 16;

        if (voto == 0)
        {
            votosEnBlanco++;
        }
        else if (voto >= 1 && voto <= cantidadListas)
        {
            votosListas[voto - 1]++;
            totalValidos++;
            if (edad <= 18)
                votosEdades[voto - 1][0]++;
            else if (edad <= 30)
                votosEdades[voto - 1][1]++;
            else if (edad <= 50)
                votosEdades[voto - 1][2]++;
            else
                votosEdades[voto - 1][3]++;
        }
        else
        {
            votosNulos++;
        }
    }
}

void mostrarTablaGeneral(string nombresListas[], int votosListas[], float porcentajes[], int indices[], int votosEnBlanco, int votosNulos)
{
    cout << "=== TABLA GENERAL DE RESULTADOS (ordenada de mayor a menor) ===" << endl;
    cout << "N.Lista\tNombre\t\tVotos\t%Validos" << endl;
    for (int i = 0; i < cantidadListas; i++)
    {
        int idx = indices[i];
        cout << (idx + 1) << "\t" << nombresListas[idx] << "\t\t"
             << votosListas[idx] << "\t" << porcentajes[idx] << "%" << endl;
    }

    cout << "\nVotos en blanco: " << votosEnBlanco << endl;
    cout << "Votos nulos: " << votosNulos << endl
         << endl;
}

void calcularCocientes(int votosListas[], float porcentajes[], float cocientes[][cantidadBancas])
{
    for (int i = 0; i < cantidadListas; i++)
    {
        for (int j = 0; j < cantidadBancas; j++)
        {
            cocientes[i][j] = -1; // ARREGLADOO
        }
        if (porcentajes[i] >= 3.0)
        {
            for (int divisor = 1; divisor <= cantidadBancas; divisor++)
            {
                cocientes[i][divisor - 1] = (float)votosListas[i] / divisor;
            }
        }
    }
}

void mostrarGrillaCocientes(string nombresListas[], float cocientes[][cantidadBancas], float porcentajes[])
{
    cout << "=== GRILLA DE COCIENTES D'HONT ===" << endl;
    for (int i = 0; i < cantidadListas; i++)
    {
        cout << nombresListas[i] << ": ";
        if (porcentajes[i] < 3.0)
        {
            cout << "DESCARTADA (menos del 3%)" << endl;
            continue;
        }
        for (int j = 0; j < cantidadBancas; j++)
        {
            cout << cocientes[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void repartirBancas(float cocientes[][cantidadBancas], int bancasObtenidas[])
{
    for (int i = 0; i < cantidadListas; i++)
        bancasObtenidas[i] = 0;
    for (int banca = 0; banca < cantidadBancas; banca++)
    {
        float maxCociente = -1;
        int listaGanadora = -1;
        int posDivisor = -1;
        for (int i = 0; i < cantidadListas; i++)
        {
            for (int j = 0; j < cantidadBancas; j++)
            {
                if (cocientes[i][j] > maxCociente)
                {
                    maxCociente = cocientes[i][j];
                    listaGanadora = i;
                    posDivisor = j;
                }
            }
        }
        if (listaGanadora == -1)
            break; 
        bancasObtenidas[listaGanadora]++;
        cocientes[listaGanadora][posDivisor] = -1;
    }
}

void mostrarBancas(string nombresListas[], string candidatos[][cantidadCandidatos],
                   int votosListas[], int bancasObtenidas[])
{
    cout << "=== DISTRIBUCION DE BANCAS (" << cantidadBancas << " en total) ===" << endl;
    for (int i = 0; i < cantidadListas; i++)
    {
        cout << nombresListas[i] << " - Votos: " << votosListas[i]
             << " - Bancas obtenidas: " << bancasObtenidas[i] << endl;

        if (bancasObtenidas[i] > 0)
        {
            cout << "  Titulares que ingresan:" << endl;
            int titularesDisponibles = 5;
            int cantAMostrar = (bancasObtenidas[i] < titularesDisponibles)
                                   ? bancasObtenidas[i]
                                   : titularesDisponibles;

            for (int b = 0; b < cantAMostrar; b++)
            {
                cout << "  - " << candidatos[i][b] << endl;
            }
            if (bancasObtenidas[i] > titularesDisponibles)
            {
                cout << "  (obtuvo mas bancas que titulares disponibles: revisar caso)" << endl;
            }
        }
        cout << "------------------------" << endl;
    }
    cout << endl;
}


int main()
{
    string nombresListas[cantidadListas];
    string candidatos[cantidadListas][cantidadCandidatos];
    int votosListas[cantidadListas];
    int votosEdades[cantidadListas][cantidadEdades];
    int votosEnBlanco, votosNulos, totalValidos;
    float porcentajes[cantidadListas];
    int indices[cantidadListas];
    float cocientes[cantidadListas][cantidadBancas];
    int bancasObtenidas[cantidadListas];

    cargarListas(nombresListas, candidatos);
    generarVotos(votosListas, votosEdades, votosEnBlanco, votosNulos, totalValidos);
    // Calcular porcentajes
for (int i = 0; i < cantidadListas; i++)
{
    if (totalValidos > 0)
        porcentajes[i] = (float)votosListas[i] * 100 / totalValidos;
    else
        porcentajes[i] = 0;
}

// Ordenar listas por cantidad de votos
for (int i = 0; i < cantidadListas; i++)
    indices[i] = i;

for (int i = 0; i < cantidadListas - 1; i++)
{
    for (int j = 0; j < cantidadListas - i - 1; j++)
    {
        if (votosListas[indices[j]] < votosListas[indices[j + 1]])
        {
            int aux = indices[j];
            indices[j] = indices[j + 1];
            indices[j + 1] = aux;
        }
    }
}

    mostrarTablaGeneral(nombresListas, votosListas, porcentajes, indices, votosEnBlanco, votosNulos);
    calcularCocientes(votosListas, porcentajes, cocientes);
    mostrarGrillaCocientes(nombresListas, cocientes, porcentajes);
    repartirBancas(cocientes, bancasObtenidas);
    mostrarBancas(nombresListas, candidatos, votosListas, bancasObtenidas);

    cout << "\n=== VOTOS POR RANGO DE EDAD ===\n";
    cout << "Lista\tHasta18\tHasta30\tHasta50\tMas50\n";

    for (int i = 0; i < cantidadListas; i++)
    {
    cout << nombresListas[i] << "\t";
    for (int j = 0; j < cantidadEdades; j++)
    {
        cout << votosEdades[i][j] << "\t";
    }
    cout << endl;
    }
return 0;
}