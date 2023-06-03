#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#define NOMBRE_ARCHIVO "PRUEBA_CHICA.csv"

using namespace std;

int main()
{
    ifstream archivo(NOMBRE_ARCHIVO);
    string linea;
    char delimitador = ';';                                                             //Le damos nombre al separador.
    
    getline(archivo, linea);                                                            // Leemos la primer línea para descartarla, pues es el encabezado.
    
    map<string, pair<int, int>> sumaMontoDescuentoPorMes;                               // Declarar el mapa para almacenar la suma del monto y el descuento por mes.      

    
    while (getline(archivo, linea))                                                     // While que lea (por linea) hasta que no haya nada mas.
    {
        stringstream stream(linea);                                                     // Convertir la cadena a un stream.                                                 REVISAR
        string SKU, NOMBRE, FECHA, ESTADO, MONTO, DESCUENTO;                            //Definimos los diferentes datos como string. 

        // Extraer todos los valores de esa fila
        getline(stream, SKU, delimitador);
        getline(stream, NOMBRE, delimitador);
        getline(stream, MONTO, delimitador);
        getline(stream, DESCUENTO, delimitador);
        getline(stream, FECHA, delimitador);
        getline(stream, ESTADO, delimitador);

        if (ESTADO == "AUTHORIZED" || ESTADO == "FINALIZED")                            //Evaluamos el estado de la transaccion para saber si se contabiliza.
        {
            string mes = FECHA.substr(0, 7);                                            //Dejamos la fecha solo con los valores de año y mes.
            int monto = stoi(MONTO);                                                    //Utilizamos el stoi para pasar de string a entero.                                                                            
            int descuento = stoi(DESCUENTO);                                                                                                     

            // Actualizar la suma del monto y el descuento por mes en el mapa
            if (sumaMontoDescuentoPorMes.find(mes) == sumaMontoDescuentoPorMes.end())   // Si no existe una entrada para el mes actual, crea una nueva entrada en el mapa.
            {
                
                sumaMontoDescuentoPorMes[mes] = make_pair(monto, descuento);
            }
            else                                                                        //Si existe otra entrada para el mes actual actualizamos valores.
            {
                
                sumaMontoDescuentoPorMes[mes].first += monto;
                sumaMontoDescuentoPorMes[mes].second += descuento;
            }
        }
    }

    archivo.close();

    int i=1;                                                                            //Definimos i como la variable que utilizaremos para recorrer el arreglo de los meses.
    float ipc[12];

    
    for (const auto &par : sumaMontoDescuentoPorMes)                                    //Calculamos el ipc para cada caso e imprimimos la suma del monto y el descuento por mes.
    {
        float montobase;
        string mes = par.first;
        int sumaMonto = par.second.first;
        if(mes=="2022-01")
        {
            montobase= sumaMonto;
        }
        int sumaDescuento = par.second.second;
        ipc[i]=sumaMonto/montobase*100;                                                 //En la posicion i del arreglo ipc se calculara el ipc para el respectivo mes.
        //cout << "Mes: " << mes << endl;
        //cout << "IPC:" << ipc[i] << endl;
        i++;
        //cout << "Suma del monto: " << sumaMonto << endl;
        //cout << "Suma del descuento: " << sumaDescuento << endl;
        //cout << "Valor monto sin dcto:" << sumaMonto-sumaDescuento << endl;
        //cout << "==================" << endl;
    }

    float TI=0;
    for(i=1;i<=12;i++)                                                                  //Realizamos un for que recorra los meses y vaya calculando la tasa de inflacion. 
    {
        TI=TI+(((ipc[i+1]-ipc[i])/ipc[i])*100);
        cout << ipc[i] << endl;
        cout << TI << "%" << endl;
    }
    cout << "Inflacion Acumulada:" << TI << "%" << endl;
    return 0;
}
