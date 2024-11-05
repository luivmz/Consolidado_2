#include <iostream>
#include <string>

using namespace std;

// Estructura para representar un barco
struct Barco {
    string nombre;
    int prioridad;
    bool deseaAtraque;
    Barco* siguiente;
};

// Estructura para representar una cola de barcos en una terminal
struct Cola {
    Barco* frente;
    Barco* final;
};

// Estructura para representar una terminal de carga
struct Terminal {
    Cola colaAtraque;
    Cola colaPartida;
    Terminal* siguiente;
    string nombre;
};

// Prototipos de funciones
void inicializarCola(Cola &c);
void encolarBarco(Cola &c, string nombre, int prioridad, bool deseaAtraque);
void desencolarBarco(Cola &c);
void mostrarCola(const Cola &c, bool esAtraque);
void agregarTerminal(Terminal*& puerto, string nombre);
void reconocimiento(Terminal* puerto, string nombreTerminal, string nombreBarco, int prioridad, bool deseaAtraque);
void accion(Terminal* puerto, string nombreTerminal);
void mostrarEstadoPuerto(Terminal* puerto);
void mostrarEstadoTerminal(Terminal* terminal);
void liberarPuerto(Terminal*& puerto);
void menu();
void pedirDatosBarco(string& nombre, int& prioridad, bool& deseaAtraque);

int main() {
    Terminal* puerto = NULL;
    int opcion;
    string nombreTerminal, nombreBarco;
    int prioridad;
    bool deseaAtraque;

    agregarTerminal(puerto, "Terminal Norte");
    agregarTerminal(puerto, "Terminal Sur");

    do {
        menu();
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cout << "---------------------------------\n";
        cin.ignore();  // Limpiar el buffer

        switch (opcion) {
            case 1:
                cout << "\n\tTerminales actuales:\n";
                cout << "---------------------------------\n";
                mostrarEstadoPuerto(puerto);
                cout << "Ingrese el nombre de la nueva terminal: ";
                getline(cin, nombreTerminal);
                agregarTerminal(puerto, nombreTerminal);
                break;
            
            case 2:
                cout << "\n\tTerminales disponibles:\n";
                cout << "---------------------------------\n";
                mostrarEstadoPuerto(puerto);
                cout << "Ingrese el nombre de la terminal donde desea encolar el barco: ";
                getline(cin, nombreTerminal);
                pedirDatosBarco(nombreBarco, prioridad, deseaAtraque);
                reconocimiento(puerto, nombreTerminal, nombreBarco, prioridad, deseaAtraque);
                break;
            
            case 3:
                cout << "\n\tTerminales disponibles:\n";
                cout << "---------------------------------\n";
                mostrarEstadoPuerto(puerto);
                cout << "Ingrese el nombre de la terminal donde desea procesar un barco: ";
                getline(cin, nombreTerminal);
                accion(puerto, nombreTerminal);
                break;
            
            case 4:
                mostrarEstadoPuerto(puerto);
                break;
            
            case 5:
                cout << "\tSaliendo del programa...\n";
                liberarPuerto(puerto);
                break;
            
            default:
                cout << "\tOpcion no válida. Intente de nuevo.\n";
        }

    } while (opcion != 5);

    return 0;
}

// Funcion para mostrar el menú de opciones
void menu() {
    cout << "\n--------------------------------------------------------\n";
    cout << "|        Gestion del Puerto Horizonte infinito        |\n";
    cout << "-------------------------------------------------------\n";
    cout << "| [1] => Agregar nueva terminal                       |\n";
    cout << "| [2] => Encolar barco en una terminal                |\n";
    cout << "| [3] => Procesar barco en una terminal               |\n";
    cout << "| [4] => Mostrar estado del puerto                    |\n";
    cout << "| [5] => Salir                                        |\n";
    cout << "-------------------------------------------------------\n";
}

// Funcion para pedir los datos de un barco al usuario
void pedirDatosBarco(string& nombre, int& prioridad, bool& deseaAtraque) {
    cout << "Ingrese el nombre del barco: ";
    getline(cin, nombre);
    cout << "Ingrese la prioridad del barco (1 => Alta, 2 => Media, 3 => Baja): ";
    cin >> prioridad;
    int opcion;
    cout << "El barco desea (1) => Atraque o (2) => Partida: ";
    cin >> opcion;
    deseaAtraque = (opcion == 1);
    cin.ignore();  // Limpiar el buffer
}

// Inicializar una cola vacía
void inicializarCola(Cola &c) {
    c.frente = c.final = NULL;
}

// Encolar barco en la cola correspondiente (atraque o partida)
void encolarBarco(Cola &c, string nombre, int prioridad, bool deseaAtraque) {
    Barco* nuevo = new Barco{nombre, prioridad, deseaAtraque, NULL};
    if (c.final != NULL) {
        c.final->siguiente = nuevo;
    } else {
        c.frente = nuevo;
    }
    c.final = nuevo;
    cout << "\tEl barco " << nombre << " se ha encolado para " << (deseaAtraque ? "atraque" : "partida") << " con prioridad " << prioridad << ".\n";
}

// Desencolar barco de la cola correspondiente (atraque o partida)
void desencolarBarco(Cola &c) {
    if (c.frente != NULL) {
        Barco* aux = c.frente;
        c.frente = aux->siguiente;
        cout << "\tProcesando barco " << aux->nombre << " con prioridad " << aux->prioridad << ".\n";
        delete aux;
        if (c.frente == NULL) {
            c.final = NULL;
        }
    } else {
        cout << "\tNo hay barcos en espera.\n";
    }
}

// Mostrar la cola de barcos (atraque o partida)
void mostrarCola(const Cola &c, bool esAtraque) {
    Barco* actual = c.frente;
    // cout << "\tCola de " << (esAtraque ? "Atraque" : "Partida") << ": ";
    cout << (esAtraque ? "Atraque" : "Partida") << ": ";
    while (actual != NULL) {
        // cout << "\t" << actual->nombre << " (Prioridad " << actual->prioridad << ") => ";
        cout << actual->nombre << " (Prioridad " << actual->prioridad << ") => ";
        actual = actual->siguiente;
    }
    cout << "NULL\n";
}

// Agregar una terminal al puerto
void agregarTerminal(Terminal*& puerto, string nombre) {
    Terminal* nueva = new Terminal{{NULL, NULL}, {NULL, NULL}, NULL, nombre};
    nueva->siguiente = puerto;
    puerto = nueva;
    cout << "\n\tSe ha agregado la terminal: " << nombre << "\n";
}

// Encolar barco en la terminal especificada
void reconocimiento(Terminal* puerto, string nombreTerminal, string nombreBarco, int prioridad, bool deseaAtraque) {
    Terminal* actual = puerto;
    while (actual != NULL && actual->nombre != nombreTerminal) {
        actual = actual->siguiente;
    }
    if (actual != NULL) {
        encolarBarco(deseaAtraque ? actual->colaAtraque : actual->colaPartida, nombreBarco, prioridad, deseaAtraque);
    } else {
        cout << "\tTerminal no encontrada.\n";
    }
}

// Procesar barco de la terminal especificada
void accion(Terminal* puerto, string nombreTerminal) {
    Terminal* actual = puerto;
    while (actual != NULL && actual->nombre != nombreTerminal) {
        actual = actual->siguiente;
    }
    if (actual != NULL) {
        if (actual->colaAtraque.frente != NULL) {
            desencolarBarco(actual->colaAtraque);
        } else if (actual->colaPartida.frente != NULL) {
            desencolarBarco(actual->colaPartida);
        } else {
            cout << "\tNo hay barcos en espera para procesar.\n";
        }
    } else {
        cout << "\tTerminal no encontrada.\n";
    }
}

// Mostrar el estado de todas las terminales en el puerto
void mostrarEstadoPuerto(Terminal* puerto) {
    Terminal* actual = puerto;
    while (actual != NULL) {
        mostrarEstadoTerminal(actual);
        actual = actual->siguiente;
    }
}

// Mostrar el estado de una terminal
void mostrarEstadoTerminal(Terminal* terminal) {
    cout << "\tTerminal: " << terminal->nombre << "\n";
    mostrarCola(terminal->colaAtraque, true);
    mostrarCola(terminal->colaPartida, false);
    cout << "---------------------------------\n";
}

// Liberar memoria de todas las terminales y barcos en el puerto
void liberarPuerto(Terminal*& puerto) {
    while (puerto != NULL) {
        Terminal* aux = puerto;
        while (aux->colaAtraque.frente != NULL) desencolarBarco(aux->colaAtraque);
        while (aux->colaPartida.frente != NULL) desencolarBarco(aux->colaPartida);
        puerto = puerto->siguiente;
        delete aux;
    }
}
