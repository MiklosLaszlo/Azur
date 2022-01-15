#ifndef CLIENTES_H
#define CLIENTES_H

#include <SQLAPI.h>
using namespace std;

/*class Cliente
  protected:
  string nombre;
  string contrasena;
  unsigned int telefono;
  string correo;
  char sexo;
  string fecha_nacimiento;
  unsigned int tarjeta;
  
  public:*/
  
void DarAltaCliente(string n, string c, unsigned int telf, string cor, char s, string f, unsigned int t, SAConnection* con);
  
void IniciarSesion(unsigned int telf, SAConnection* con);

void FinalizarSesion(unsigned int id, SAConnection* con);

void DarBajaCliente(unsigned int telf, SAConnection* con);

void ModificarCliente(string n, string c, string cor, char s, string f, unsigned int t,SAConnection* con);
//};


#endif // ESCENA_H
