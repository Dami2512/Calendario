#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <fstream>

using namespace std;

struct Curso {
    string nombre = "LIBRE";
    string docente = "";
    string aula = "";
    int creditos = 0;
};
bool VerificarCelda(Curso** matriz, int filas, int columnas, int hora, int dia, string docente, string aula)
{

    if (matriz[hora][dia].nombre != "LIBRE") {
        cout << "ERROR: Ya hay un curso registrado en " << hora
            << " en el día " << dia << "." << endl;
        return true;
    }


    if (matriz[hora][dia].docente == docente) {
        cout << "ERROR: El docente ya tiene clase en ese horario." << endl;
        return true;
    }


    if (matriz[hora][dia].aula == aula) {
        cout << "ERROR: El aula ya esta ocupada en ese horario." << endl;
        return true;
    }

    return false;
}
bool validarAula(const string& texto) {
    if (texto.length() != 3) return false;
    if (!isalpha(texto[0])) return false;
    if (!isdigit(texto[1]) || !isdigit(texto[2])) return false;
    return true;
}

string minusculas(string texto) {
    for (char& c : texto) {
        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }
    }
    return texto;
}

string capitalizar(string s) {

    bool mensaje = true;

    for (int i = 0; i < s.length(); i++) {
        char& c = s[i];

        if (c == ' ') {
            mensaje = true;

        }
        else {
            if (mensaje && c >= 'a' && c <= 'z') {
                c = c - 32;
            }
            mensaje = false;
        }
    }

    return s;
}




void cargarDatos(Curso** matriz, int filas, int columnas) {
    ifstream archivo("Calendario.txt");

    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo." << endl;
        return;
    }

    string linea;
    getline(archivo, linea);

    int dia = 0;

    cout << R"(
.------------------------------------.
|             CARGA DATOS            |
'------------------------------------'
    )" << endl;

    while (getline(archivo, linea)) {


        cout << linea << endl;


        if (linea.rfind(">>>", 0) == 0) {
            if (linea.find("Lunes") != string::npos) dia = 1;
            else if (linea.find("Martes") != string::npos) dia = 2;
            else if (linea.find("Miercoles") != string::npos) dia = 3;
            else if (linea.find("Jueves") != string::npos) dia = 4;
            else if (linea.find("Viernes") != string::npos) dia = 5;
            else if (linea.find("Sabado") != string::npos) dia = 6;
            else if (linea.find("Domingo") != string::npos) dia = 7;
            continue;
        }

        if (linea.size() < 5) continue;


        size_t posHora = linea.find(':');
        size_t pos1 = linea.find('|');
        size_t pos2 = linea.find('|', pos1 + 1);
        size_t pos3 = linea.find('|', pos2 + 1);

        if (posHora == string::npos || pos1 == string::npos ||
            pos2 == string::npos || pos3 == string::npos)
            continue;

        string hora = linea.substr(3, posHora - 3);
        string nombre = linea.substr(posHora + 1, pos1 - posHora - 1);
        string docente = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string aula = linea.substr(pos2 + 1, pos3 - pos2 - 1);
        string creditosTexto = linea.substr(pos3 + 1);

        while (!nombre.empty() and nombre[0] == ' ') nombre.erase(0, 1);
        while (!docente.empty() and docente[0] == ' ') docente.erase(0, 1);
        while (!aula.empty() and aula[0] == ' ') aula.erase(0, 1);

        int creditos = stoi(creditosTexto);


        int h = 0;
        if (hora.find("08-09") != string::npos) h = 1;
        else if (hora.find("09-10") != string::npos) h = 2;
        else if (hora.find("10-11") != string::npos) h = 3;
        else if (hora.find("11-12") != string::npos) h = 4;
        else if (hora.find("12-01") != string::npos) h = 5;
        else if (hora.find("01-02") != string::npos) h = 6;


        if (h > 0 && dia > 0) {
            matriz[h][dia].nombre = nombre;
            matriz[h][dia].docente = docente;
            matriz[h][dia].aula = aula;
            matriz[h][dia].creditos = creditos;
        }
    }

    archivo.close();
    cout << "===== FIN DE DATOS =====" << endl;
    cout << endl;
}
void guardarHorario(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {
    ofstream archivo("Calendario.txt");

    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo para guardar." << endl;
        return;
    }

    cout << R"(
.------------------------------------.
|             GUARDAR DATOS          |
'------------------------------------'
    )" << endl;

    for (int d = 1; d < columnas; d++) {
        archivo << ">>> " << dias[d] << " <<<" << endl;

        for (int h = 1; h < filas; h++) {
            Curso& c = matriz[h][d];

            if (c.nombre == "LIBRE") continue;

            archivo << "  - " << horas[h] << ": "
                << c.nombre << " | "
                << c.docente << " | "
                << c.aula << " | "
                << c.creditos << " creditos" << endl;
        }

        archivo << "\n";
    }

    archivo.close();
    cout << "Horario guardado exitosamente." << endl;
    cout << endl;

}
void Consultar(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {
    string profe, curso, aula;
    bool encontrado = false;
    int opcion;
    const int ANCHO = 20;

    cout << R"(
.------------------------------------.
|              CONSULTAR             |
'------------------------------------'
    )" << endl;


    cout << "1. Buscar por nombre de profesor." << endl;
    cout << "2. Buscar por nombre del curso." << endl;
    cout << "3. Buscar por codigo de aula." << endl;
    cout << "-> Opcion: ";
    cin >> opcion;
    cin.ignore();

    if (opcion < 1 || opcion > 3) {
        cout << "Opcion invalida." << endl;
        return;
    }

    if (opcion == 1) {
        cout << "Nombre del profesor: ";
        getline(cin, profe);
        profe = minusculas(profe);
    }
    else if (opcion == 2) {
        cout << "Nombre del curso: ";
        getline(cin, curso);
        curso = minusculas(curso);
    }
    else {
        do {
            cout << "Aula: ";

            getline(cin, aula);

            aula = minusculas(aula);

            if (!validarAula(aula)) {
                cout << "Codigo de aula invalido. Intente nuevamente..." << endl;
            }
        } while (!validarAula(aula));;
    }




    cout << setw(ANCHO) << "DIA"
        << setw(ANCHO) << "HORA"
        << setw(ANCHO) << "CURSO"
        << setw(ANCHO) << "DOCENTE"
        << setw(ANCHO) << "AULA"
        << setw(ANCHO) << "CREDITOS" << endl;

    cout << string(ANCHO * 6, '-') << endl;


    for (int i = 1; i < filas; i++) {
        for (int j = 1; j < columnas; j++) {
            bool mostrar = false;

            if (opcion == 1 && minusculas(matriz[i][j].docente).find(profe) != string::npos)
                mostrar = true;
            else if (opcion == 2 && minusculas(matriz[i][j].nombre).find(curso) != string::npos)
                mostrar = true;
            else if (opcion == 3 && minusculas(matriz[i][j].aula).find(aula) != string::npos)
                mostrar = true;

            if (mostrar) {
                cout << setw(ANCHO) << dias[j]
                    << setw(ANCHO) << horas[i]
                    << setw(ANCHO) << matriz[i][j].nombre
                    << setw(ANCHO) << matriz[i][j].docente
                    << setw(ANCHO) << matriz[i][j].aula
                    << setw(ANCHO) << matriz[i][j].creditos << endl;

                encontrado = true;
            }
        }
    }

    if (!encontrado)
        cout << "No se encontraron resultados." << endl;
    cout << endl;

}

void buscarDocente(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {

    cout << R"(
.------------------------------------.
|            BUSCAR DOCENTE          |
'------------------------------------'
    )" << endl;
    string docente;
    cin.ignore();
    cout << "Docente: ";
    getline(cin, docente);
    docente = capitalizar(minusculas(docente));

    if (docente.empty()) {
        cout << "ERROR: El nombre del  no puede estar vacio." << endl;
        return;
    }

    for (int i = 1; i < filas; i++) {
        for (int j = 1; j < columnas; j++) {
            if (matriz[i][j].docente.find(docente) != string::npos) {
                cout << dias[j] << " " << horas[i] << ": " << matriz[i][j].nombre << endl;
            }
        }
    }

}

void contarHorasDocente(Curso** matriz, int filas, int columnas) {
    string docente;
    cin.ignore();

    cin.ignore();
    cout << R"(
  .------------------------------------.
  |     CONTAR HORAS DE UN DOCENTE     |
  '------------------------------------'
    )" << endl;
    cout << "Ingrese el nombre del docente: ";
    getline(cin, docente);
    docente = capitalizar(minusculas(docente));

    if (docente.empty()) {
        cout << "ERROR: El nombre del docente no puede estar vacio." << endl;
        return;
    }


    int contador = 0;

    for (int h = 1; h < filas; h++)
        for (int d = 1; d < columnas; d++)
            if (matriz[h][d].docente.find(docente) != string::npos)
                contador++;

    cout << "El docente '" << docente << "' dicta " << contador << " hora(s) semanales." << endl;

    cout << endl;

}

void contarHorasCurso(Curso** matriz, int filas, int columnas) {
    string curso;
    cin.ignore();

    cout << R"(
  .----------------------------------.
  |    CONTAR HORAS DE UN CURSO      |
  '----------------------------------'
)" << endl;
    cout << "Ingrese el curso: ";

    getline(cin, curso);
    curso = capitalizar(minusculas(curso));

    if (curso.empty()) {
        cout << "ERROR: El nombre del curso no puede estar vacio." << endl;
        return;
    }

    int contador = 0;

    for (int h = 1; h < filas; h++)
        for (int d = 1; d < columnas; d++)
            if (matriz[h][d].nombre.find(curso) != string::npos)
                contador++;

    cout << "El curso '" << curso << "' tiene " << contador << " hora(s) semanales.";

}
void eliminarCurso(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {
    int dia, hora;

    cout << R"(
  .----------------------------------.
  |        ElIMINAR UN CURSO         |
  '----------------------------------'
    )" << endl;


    do {
        cout << "1. Lunes | 2. Martes | 3. Miercoles | 4. Jueves | 5. Viernes | 6. Sabado | 7. Domingo" << endl;
        cout << "-> Dia: ";
        cin >> dia;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ERROR: Debe ingresar un numero." << endl;
            dia = 0;
            continue;
        }
        if (dia < 1 || dia > 7) {
            cout << "ERROR: Numero fuera del rango" << endl;
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    } while (true);


    do {
        cout << "Seleccione la hora:" << endl;
        for (int i = 1; i < filas; i++)
            cout << i << ". " << horas[i] << endl;

        cout << "-> Hora: ";
        cin >> hora;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ERROR: Debe ingresar un numero." << endl;
            hora = 0;
            continue;
        }
        if (hora < 1 || hora >= filas) {
            cout << "ERROR: Horario fuera del rango" << endl;
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    } while (true);

    Curso& c = matriz[hora][dia];

    if (c.nombre == "LIBRE") {
        cout << "No hay un curso registrado en este horario." << endl;
        return;
    }


    int duracion = 1;
    for (int h = hora + 1; h < filas; h++) {
        if (matriz[h][dia].nombre == c.nombre)
            duracion++;
        else
            break;
    }


    char confirmar;
    cout << "Desea eliminar el curso '" << c.nombre << "' que dura " << duracion << " hora(s) consecutivas? (S/N): ";
    cin >> confirmar;
    cin.ignore();
    confirmar = toupper(confirmar);

    if (confirmar != 'S') {
        cout << "Eliminación cancelada." << endl;
        return;
    }


    for (int i = 0; i < duracion; i++) {
        matriz[hora + i][dia].nombre = "LIBRE";
        matriz[hora + i][dia].docente = "";
        matriz[hora + i][dia].aula = "";
        matriz[hora + i][dia].creditos = 0;
    }

    cout << "Curso eliminado exitosamente." << endl << endl;
}

void modificarCurso(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {

    int dia, hora, opcion;

    cout << R"(
  .----------------------------------.
  |          MODIFICAR CURSO         |
  '----------------------------------'
    )" << endl;



    do {
        cout << "1. Lunes | 2. Martes | 3. Miercoles | 4. Jueves | 5. Viernes | 6. Sabado | 7. Domingo" << endl;
        cout << "-> Dia: ";
        cin >> dia;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ERROR: Debe ingresar un numero." << endl;
            dia = 0;
            continue;
        }

        if (dia < 1 or dia > columnas) {

            cout << "ERROR: Numero fuera del rango" << endl;
            continue;

        }
        cin.ignore(1000, '\n');
        break;

    } while (dia < 1 || dia > 7);


    do {
        cout << "Seleccione la hora:" << endl;
        for (int i = 1; i < filas; i++)
            cout << i << ". " << horas[i] << endl;

        cout << "-> Hora: ";
        cin >> hora;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ERROR: Debe ingresar un numero." << endl;
            hora = 0;
            continue;
        }
        if (hora < 1 or hora >= filas) {

            cout << "ERROR: Horario fuera del rango" << endl;
            continue;
        }
        cin.ignore(1000, '\n');
        break;

    } while (hora < 1 or hora >= filas);

    Curso& c = matriz[hora][dia];

    if (c.nombre == "LIBRE") {
        cout << "No hay un curso registrado en este horario." << endl;
        return;
    }

    int duracion = 1;
    for (int h = hora + 1; h < filas; h++) {
        if (matriz[h][dia].nombre == c.nombre)
            duracion++;
        else
            break;
    }

    cout << "Curso actual:" << endl;
    cout << "1. Nombre:   " << c.nombre << endl;
    cout << "2. Docente:  " << c.docente << endl;
    cout << "3. Aula:     " << c.aula << endl;
    cout << "4. Creditos: " << c.creditos << endl;



    cout << "Escoja que modificar" << endl;
    cout << "1. Nombre del curso" << endl;
    cout << "2. Docente" << endl;
    cout << "3. Aula" << endl;
    cout << "4. Creditos" << endl;
    cout << "5. Todo" << endl;
    cout << "-> Opcion: ";
    cin >> opcion;
    cin.ignore();

    string nuevoNombre, nuevoDocente, nuevaAula;
    int nuevosCreditos;


    switch (opcion) {
    case 1:
        cout << "Nuevo nombre: ";
        getline(cin, nuevoNombre);
        nuevoNombre = capitalizar(minusculas(nuevoNombre));

        for (int i = 0; i < duracion; i++)
            matriz[hora + i][dia].nombre = nuevoNombre;
        break;

        break;
    case 2:
        cout << "Nuevo docente: ";
        getline(cin, nuevoDocente);
        nuevoDocente = capitalizar(minusculas(nuevoDocente));
        for (int i = 0; i < duracion; i++)
            matriz[hora + i][dia].docente = nuevoDocente;
        break;
    case 3:
        do {
            cout << "Nueva aula: ";

            getline(cin, nuevaAula);

            nuevaAula = capitalizar(nuevaAula);

            if (!validarAula(nuevaAula)) {
                cout << "Codigo de aula invalido. Intente nuevamente..." << endl;
            }
        } while (!validarAula(nuevaAula));
        for (int i = 0; i < duracion; i++)
            matriz[hora + i][dia].aula = nuevaAula;
        break;
    case 4:
        do {
            cout << "Nuevos creditos: ";
            cin >> nuevosCreditos;
            cin.ignore();

            if (nuevosCreditos < 3 or nuevosCreditos > 6) {
                cout << "ERROR: Cantidad de creditos invalidos" << endl;
            }
        } while (nuevosCreditos < 3 or nuevosCreditos > 6);
        for (int i = 0; i < duracion; i++)
            matriz[hora + i][dia].creditos = nuevosCreditos;
        break;
    case 5:
        cout << "Nuevo nombre: ";
        getline(cin, nuevoNombre);
        nuevoNombre = capitalizar(minusculas(nuevoNombre));
        cout << "Nuevo docente: ";
        getline(cin, nuevoDocente);
        nuevoDocente = capitalizar(minusculas(nuevoDocente));
        do {
            cout << "Nueva aula: ";

            getline(cin, nuevaAula);

            c.aula = capitalizar(nuevaAula);

            if (!validarAula(nuevaAula)) {
                cout << "Codigo de aula invalido. Intente nuevamente..." << endl;
            }
        } while (!validarAula(nuevaAula));
        do {
            cout << "Nuevos creditos: ";
            cin >> nuevosCreditos;
            cin.ignore();

            if (nuevosCreditos < 3 or nuevosCreditos > 6) {
                cout << "ERROR: Cantidad de creditos invalidos" << endl;
            }
        } while (nuevosCreditos < 3 or nuevosCreditos > 6);
        break;
    default:
        cout << "ERROR: Opcion no valida." << endl;;
        return;
        for (int i = 0; i < duracion; i++) {
            matriz[hora + i][dia].nombre = nuevoNombre;
            matriz[hora + i][dia].docente = nuevoDocente;
            matriz[hora + i][dia].aula = nuevaAula;
            matriz[hora + i][dia].creditos = nuevosCreditos;
        }
        break;
    }

    cout << "Curso modificado exitosamente." << endl;;
    cout << endl;

}

void mostrarDia(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {
    int dia;
    const int ANCHO = 20;

    cout << R"(
.------------------------------------.
|       MOSTRAR DATOS DE UN DIA      |
'------------------------------------'
    )" << endl;



    do {
        cout << "1 = Lunes 2 = Martes 3 = Miercoles 4 = Jueves 5 = Viernes 6 = Sabado 7 = Domingo" << endl;
        cout << "Dia: ";
        cin >> dia;
        if (dia < 1 || dia > 7)
            cout << "ERROR: Dia fuera del rango." << endl;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ERROR: Debe ingresar un numero." << endl;
            dia = 0;
            continue;
        }
        if (dia < 1 || dia > 7) {
            cout << "ERROR: Numero fuera del rango" << endl;
            continue;
        }


    } while (dia < 1 || dia > 7);

    cout << "Horario del dia " << dias[dia] << ": " << endl;


    cout << setw(ANCHO) << "Hora"
        << setw(ANCHO) << "Curso"
        << setw(ANCHO) << "Docente"
        << setw(ANCHO) << "Aula"
        << setw(ANCHO) << "Creditos" << endl;

    cout << string(ANCHO * 5, '-') << endl;


    for (int h = 1; h < filas; h++) {
        Curso& c = matriz[h][dia];
        cout << setw(ANCHO) << horas[h]
            << setw(ANCHO) << (c.nombre.empty() ? "LIBRE" : c.nombre)
            << setw(ANCHO) << (c.docente.empty() ? "----" : c.docente)
            << setw(ANCHO) << (c.aula.empty() ? "----" : c.aula)
            << setw(ANCHO) << c.creditos << endl;
    }


    cout << endl;

}

void listarHorario(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {
    const int ANCHO = 20;
    string mostrarNombre, mostrarDocente, mostrarAula;
    cout << R"(
  .----------------------------------.
  |         HORARIO COMPLETO         |
  '----------------------------------'
    )" << endl;

    cout << setw(ANCHO) << "HORA";
    for (int d = 1; d < columnas; d++)
        cout << setw(ANCHO) << dias[d];
    cout << endl;

    cout << string(ANCHO * columnas, '-') << endl;

    for (int h = 1; h < filas; h++) {


        cout << setw(ANCHO) << horas[h];


        for (int d = 1; d < columnas; d++) {
            string nombre = matriz[h][d].nombre;


            if (nombre == "")
                mostrarNombre = "LIBRE";
            else
                mostrarNombre = nombre;

            cout << setw(ANCHO) << mostrarNombre;
        }
        cout << endl;


        cout << setw(ANCHO) << "";
        for (int d = 1; d < columnas; d++) {


            if (matriz[h][d].docente == "")
                mostrarDocente = "----";
            else
                mostrarDocente = matriz[h][d].docente;

            cout << setw(ANCHO) << ("Prof: " + mostrarDocente);
        }
        cout << endl;


        cout << setw(ANCHO) << "";
        for (int d = 1; d < columnas; d++) {


            if (matriz[h][d].aula == "")
                mostrarAula = "----";
            else
                mostrarAula = matriz[h][d].aula;

            cout << setw(ANCHO) << ("Aula: " + mostrarAula);
        }
        cout << endl;


        cout << setw(ANCHO) << "";
        for (int d = 1; d < columnas; d++) {
            cout << setw(ANCHO) << ("Cred: " + to_string(matriz[h][d].creditos));
        }
        cout << endl;

        cout << string(ANCHO * columnas, '-') << endl;
    }

    cout << endl;

}
void MulHoras(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {

    int dia, hora, duracion, horaA;


    do {
        cout << "1. Lunes | 2. Martes | 3. Miercoles | 4. Jueves | 5. Viernes | 6. Sabado | 7. Domingo" << endl;
        cout << "-> Dia: ";
        cin >> dia;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ERROR: Debe ingresar un numero." << endl;
            dia = 0;
            continue;
        }

        if (dia < 1 or dia > columnas) {

            cout << "ERROR: Numero fuera del rango" << endl;
            continue;

        }
        cin.ignore(1000, '\n');
        break;

    } while (dia < 1 || dia > 7);



    do {
        cout << "Seleccione la hora:" << endl;
        for (int i = 1; i < filas; i++)
            cout << i << ". " << horas[i] << endl;

        cout << "-> Hora: ";
        cin >> hora;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ERROR: Debe ingresar un numero." << endl;
            hora = 0;
            continue;
        }



        if (hora < 1 or hora >= filas) {

            cout << "ERROR: Horario fuera del rango" << endl;
            continue;

        }

        cin.ignore(1000, '\n');
        break;

    } while (hora < 1 or hora >= filas);

    cout << "Duracion (en cantidad de horas): ";
    cin >> duracion;
    cin.ignore();


    if (hora + duracion - 1 >= filas) {
        cout << "ERROR: La duración excede el horario disponible." << endl;
        return;
    }

    Curso c;

    cout << "Nombre del curso: ";
    getline(cin, c.nombre);
    c.nombre = capitalizar(minusculas(c.nombre));

    cout << "Docente: ";
    getline(cin, c.docente);
    c.docente = capitalizar(minusculas(c.docente));

    do {
        cout << "Aula (Ejemplo: A45): ";

        getline(cin, c.aula);

        c.aula = capitalizar(c.aula);

        if (!validarAula(c.aula)) {
            cout << "ERROR: Codigo de aula invalido. Intente nuevamente..." << endl;
        }

    } while (!validarAula(c.aula));

    do {
        cout << "Creditos: ";
        cin >> c.creditos;
        cin.ignore();

        if (c.creditos < 3 or c.creditos > 6) {
            cout << "ERROR: Cantidad de creditos invalidos" << endl;
        }

    } while (c.creditos < 3 or c.creditos > 6);


    for (int i = 0; i < duracion; i++) {

        horaA = hora + i;

        if (VerificarCelda(matriz, filas, columnas, horaA, dia, c.docente, c.aula)) {
            cout << "ERROR: Registro cancelado por conflicto." << endl;
            return;

            _getch();
        }
    }


    for (int i = 0; i < duracion; i++) {
        matriz[hora + i][dia] = c;
    }
    cout << "Curso registrado en " << duracion << " hora(s) consecutiva(s) correctamente.";
    cout << endl;


}

void completarCursos(Curso** matriz, int filas, int columnas, string dias[], string horas[]) {
    int dia, hora;
    char continuar = 'S', seguir = 'S';


    cout << R"(
  .----------------------------------.
  |          REGISTRO CURSO          |
  '----------------------------------'
    )" << endl;



    do {
        cout << "Desea registrar un curso en varias horas consecutivas? (S/N): ";
        cin >> seguir;
        seguir = toupper(seguir);


        if (seguir != 'S' and seguir != 'N') {
            cout << "ERROR: Opcion no valida." << endl;

        }

        cin.ignore(1000, '\n');

    } while (seguir != 'S' and seguir != 'N');


    if (seguir == 'S') {

        MulHoras(matriz, filas, columnas, dias, horas);

    }
    else
        while (continuar == 'S') {

            do {
                cout << "1. Lunes | 2. Martes | 3. Miercoles | 4. Jueves | 5. Viernes | 6. Sabado | 7. Domingo" << endl;
                cout << "-> Dia: ";
                cin >> dia;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "ERROR: Debe ingresar un numero." << endl;
                    dia = 0;
                    continue;
                }

                if (dia < 1 or dia > columnas) {

                    cout << "ERROR: Numero fuera del rango" << endl;
                    continue;

                }
                cin.ignore(1000, '\n');
                break;

            } while (dia < 1 || dia > 7);

            do {
                cout << "Seleccione la hora:" << endl;
                for (int i = 1; i < filas; i++)
                    cout << i << ". " << horas[i] << endl;

                cout << "-> Hora: ";
                cin >> hora;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "ERROR: Debe ingresar un numero." << endl;
                    hora = 0;
                    continue;
                }



                if (hora < 1 or hora >= filas) {

                    cout << "ERROR: Horario fuera del rango" << endl;
                    continue;

                }

                cin.ignore(1000, '\n');
                break;

            } while (hora < 1 or hora >= filas);



            Curso& c = matriz[hora][dia];

            cout << "Registrando curso en el Dia " << dias[dia] << "| Hora " << horas[hora] << endl;

            if (c.nombre != "LIBRE") {
                cout << "Ya existe un curso registrado" << endl;
                cout << "Curso: " << c.nombre << endl << "Docente: " << c.docente << "Aula: " << c.aula << endl << "Creditos: " << c.creditos << endl;

                char op;

                do {
                    cout << "Desea editar?  (N/S) ";
                    cin >> op;
                    cin.ignore(1000, '\n');
                    op = toupper(op);

                    if (op != 'S' and op != 'N') {

                        cout << "ERROR: Opcion no valida" << endl;
                        continue;
                    }
                    if (op == 'N') {

                        return;
                    }

                } while (op != 'S' and op != 'N');


            }


            cout << "DATO: 0 para cancelar: " << endl;
            cout << "Nombre del Curso: ";
            getline(cin, c.nombre);
            c.nombre = capitalizar(minusculas(c.nombre));

            if (c.nombre == "0") {
                c.nombre = "LIBRE";
                return;
            }

            cout << "Docente: ";
            getline(cin, c.docente);
            c.docente = capitalizar(minusculas(c.docente));




            do {
                cout << "Aula (Ejemplo: A45): ";

                getline(cin, c.aula);

                c.aula = capitalizar(c.aula);

                if (!validarAula(c.aula)) {
                    cout << "ERROR: Codigo de aula invalido. Intente nuevamente..." << endl;
                }

            } while (!validarAula(c.aula));

            do {
                cout << "Creditos: ";
                cin >> c.creditos;
                cin.ignore();

                if (c.creditos < 3 or c.creditos > 6) {
                    cout << "ERROR: Cantidad de creditos invalidos" << endl;
                }

            } while (c.creditos < 3 or c.creditos > 6);


            cout << "Curso registrado exitosamente." << endl;

            do {
                cout << "Desea registrar otro curso? (S/N): ";
                cin >> continuar;
                continuar = toupper(continuar);


                if (continuar != 'S' and continuar != 'N') {
                    cout << "ERROR: Opcion no valida." << endl;
                }
                cin.ignore(1000, '\n');

            } while (continuar != 'S' and continuar != 'N');
        }





    listarHorario(matriz, filas, columnas, dias, horas);

    cout << endl;
}

int main() {
    int op;
    const int ANCHO = 16;
    const int ancho = 28;
    const int filas = 7;
    const int columnas = 8;

    string dias[columnas] = { " ", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo" };
    string horas[filas] = { " ", "08-09", "09-10", "10-11", "11-12", "12-01", "01-02" };

    Curso** matriz = new Curso * [filas];
    for (int i = 0; i < filas; i++) {
        matriz[i] = new Curso[columnas];
    }




    do {

        cout << R"(
          __    _    _     _ ____ _   _ ____    _    ____  _  ___
         / _|  / \  | |   | |____| \ | |  _ \  / \  |  _ \| |/ _ \  
        | |   / _ \ | |   | |__| |  \| | | | |/ _ \ | |_| | | | | |
        | |_ / ___ \| |___| |____| |\  | |_| / ___ \|  _ <| | |_| | 
         \__/_/   \_\_____|_|____| | \_|____/_/   \_\_/ \_|_|\___/ 
        )" << endl;

        cout << setw(ANCHO) << "" << " __________________________________________" << endl;
        cout << setw(ANCHO) << "" << "|             MENU DE OPCIONES             |" << endl;
        cout << setw(ANCHO) << "" << " ------------------------------------------" << endl;
        cout << endl;
        cout << setw(ancho) << "1." << "Ingresar cursos." << endl;
        cout << setw(ancho) << "2." << "Listar cursos." << endl;
        cout << setw(ancho) << "3." << "Mostrar clases de un dia." << endl;
        cout << setw(ancho) << "4." << "Modificar Horario" << endl;
        cout << setw(ancho) << "5." << "Eliminar curso" << endl;
        cout << setw(ancho) << "6." << "Contar horas de un curso." << endl;
        cout << setw(ancho) << "7." << "Contar horas de un docente." << endl;
        cout << setw(ancho) << "8." << "Cosultar" << endl;
        cout << setw(ancho) << "9." << "Guardar horario en archivo" << endl;
        cout << setw(ancho) << "10." << "Cargar horario desde archivo" << endl;
        cout << setw(ancho) << "11." << "Finalizar programa." << endl;
        cout << "-> ELIJA UNA OPCION: ";
        cin >> op;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "ERROR: Ingrese un numero valido." << endl;
            _getch();
            system("cls");
            continue;
        }


        system("cls");

        cin.ignore(1000, '\n');

        switch (op) {
        case 1:
            completarCursos(matriz, filas, columnas, dias, horas);
            break;
        case 2:
            listarHorario(matriz, filas, columnas, dias, horas);
            break;
        case 3:
            mostrarDia(matriz, filas, columnas, dias, horas);
            break;
        case 4:
            modificarCurso(matriz, filas, columnas, dias, horas);
            break;

        case 5:
            eliminarCurso(matriz, filas, columnas, dias, horas);
            break;
        case 6:
            contarHorasCurso(matriz, filas, columnas);
            break;

        case 7:
            contarHorasDocente(matriz, filas, columnas);
            break;

        case 8:
            Consultar(matriz, filas, columnas, dias, horas);
            break;

        case 9:
            guardarHorario(matriz, filas, columnas, dias, horas);
            break;

        case 10:
            cargarDatos(matriz, filas, columnas);
            break;


        default:

            cout << "ERROR: Opcion invalida" << endl;

        }

        _getch();
        system("cls");




    } while (op != 11);

    cout << "PROGRAMA FINALIZADO." << endl;

    for (int i = 0; i < filas; i++) delete[] matriz[i];
    delete[] matriz;

    _getch();

    return 0;
}