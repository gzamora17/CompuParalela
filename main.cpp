#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

struct Transaction {
    std::string sku;
    std::string nombre;
    int monto;
    int descuento;
    std::string fecha;
    std::string estado;
};

double calcularInflacion(const std::vector<Transaction>& transactions) {
    std::unordered_map<std::string, int> productosMesBase;
    std::unordered_map<std::string, int> productosMesActual;

    // Inicializar los productos del mes base
    for (const Transaction& transaction : transactions) {
        if (transaction.fecha.substr(5, 2) == "01") {  // Mes base: enero
            productosMesBase[transaction.sku]++;
        }
    }

    // Calcular los productos del mes actual
    for (const Transaction& transaction : transactions) {
        if (transaction.fecha.substr(5, 2) != "01") {  // Excluir mes base: enero
            productosMesActual[transaction.sku]++;
        }
    }

    // Calcular el IPC y la inflación acumulada
    int productosBaseTotal = productosMesBase.size();
    int productosActualTotal = productosMesActual.size();
    int productosCoincidentes = 0;

    for (const auto& producto : productosMesBase) {
        if (productosMesActual.count(producto.first)) {
            productosCoincidentes++;
        }
    }

    double inflacionAcumulada = static_cast<double>(productosActualTotal - productosCoincidentes) / productosBaseTotal * 100.0;

    return inflacionAcumulada;
}

int main() {
    std::ifstream file("ventas.txt"); // Reemplaza "ventas.txt" por el nombre y ruta de tu archivo de ventas

    if (!file) {
        std::cerr << "Error al abrir el archivo de ventas." << std::endl;
        return 1;
    }

    std::vector<Transaction> transactions;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Transaction transaction;
        std::string temp;

        // Leer cada campo separado por comas y asignarlos a los miembros de Transaction
        std::getline(ss, transaction.sku, ',');
        std::getline(ss, transaction.nombre, ',');
        std::getline(ss, temp, ',');
        transaction.monto = std::stoi(temp);
        std::getline(ss, temp, ',');
        transaction.descuento = std::stoi(temp);
        std::getline(ss, transaction.fecha, ',');
        std::getline(ss, transaction.estado, ',');

        transactions.push_back(transaction);
    }

    file.close();

    double inflacion = calcularInflacion(transactions);
    std::cout.precision(4);
    std::cout << "Inflacion acumulada: " << std::fixed << inflacion << " %" << std::endl;

    return 0;
}
