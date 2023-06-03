#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <omp.h>
#define NOMBRE_ARCHIVO "PRUEBA_CHICA.csv"

using namespace std;

int main()
{
    ifstream archivo(NOMBRE_ARCHIVO);                                                   
    string linea;
    char delimitador = ';';                                                                         //Le damos nombre al separador.

    getline(archivo, linea);                                                                        // Leemos la primer línea para descartarla, pues es el encabezado.

    map<string, pair<int, int>> sumaMontoDescuentoPorMes;                                           // Mapa para almacenar la suma del monto y el descuento por mes

    while (getline(archivo, linea))                                                                 // While que lea (por linea) hasta que no haya nada mas.
    {
        stringstream stream(linea);                                                                 // Convertir la cadena a un stream
        string SKU, NOMBRE, FECHA, ESTADO, MONTO, DESCUENTO;                                        //Definimos los diferentes datos como string. 

        // Extraer todos los valores de esa fila
        getline(stream, SKU, delimitador);
        getline(stream, NOMBRE, delimitador);
        getline(stream, MONTO, delimitador);
        getline(stream, DESCUENTO, delimitador);
        getline(stream, FECHA, delimitador);
        getline(stream, ESTADO, delimitador);

        if (ESTADO == "AUTHORIZED" || ESTADO == "FINALIZED")                                        //Evaluamos el estado de la transaccion para saber si se contabiliza.
        {
            string mes = FECHA.substr(0, 7);                                                        //Dejamos la fecha solo con los valores de año y mes.
            int monto = stoi(MONTO);                                                                //Utilizamos el stoi para pasar de string a entero.
            int descuento = stoi(DESCUENTO);

            #pragma omp critical
            {
                if (sumaMontoDescuentoPorMes.find(mes) == sumaMontoDescuentoPorMes.end())           // Si no existe una entrada para el mes actual, crea una nueva entrada en el mapa.
                {
                    sumaMontoDescuentoPorMes[mes] = make_pair(monto, descuento);                    // Creamos una nueva entrada para el mes en el mapa
                }
                else                                                                                //Si existe otra entrada para el mes actual actualizamos valores.
                {
                    sumaMontoDescuentoPorMes[mes].first += monto;
                    sumaMontoDescuentoPorMes[mes].second += descuento;
                }
            }
        }
    }

    archivo.close();

    float ipc[12] = {0.0};                                                                          //Definimos ipc como un arreglo para ir guardando el IPC de cada mes
    string mes;
    float montobase = 0.0;                                                                          // Monto base para calcular el IPC

    for (const auto &par : sumaMontoDescuentoPorMes)                                                // Recorremos el mapa de sumaMontoDescuentoPorMes
    {
        mes = par.first;                                                                            // Obtenemos el mes actual
        int sumaMonto = par.second.first;                                                           // Obtenemos la suma del monto para el mes actual
        if (mes == "2022-01")
        {
            montobase = sumaMonto;
        }
        int sumaDescuento = par.second.second;
        ipc[stoi(mes.substr(5, 2))] = sumaMonto / montobase * 100;                                  // Calculamos el IPC y lo almacenamos en el arreglo ipc
    }

    float TI = 0.0;                                                                                 // Variable para almacenar el total de inflación acumulada
    #pragma omp parallel for reduction(+:TI)
    for (int i = 1; i <= 12; i++)
    {
        if (ipc[i] != 0.0)                                                                          // Verificamos si el IPC del mes actual es diferente de cero
        {
            TI += ((ipc[i + 1] - ipc[i]) / ipc[i]) * 100;                                           //Calculamos la Tasa de Inflacion
        }
    }

    cout << "Inflacion Acumulada: " << TI << "%" << endl;                                           //Para finalizar mostramos la inflacion acomulada.
    return 0;
}
