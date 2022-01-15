#ifndef CLIENTES_H
#define CLIENTES_H

using namespace std;

class Cliente{
  protected:
  string nombre;
  string contrasena;
  unsigned int telefono;
  string correo;
  char sexo;
  string fecha_nacimiento;
  unsigned int tarjeta;
  
  public:
  
  Cliente(string n, string c, unsigned int telf, string cor, char s, string f, unsigned int t);
  
  void IniciarSesion(unsigned int telf);
  void FinalizarSesion(unsigned int id);
  void DarBajaCliente();
  void ModificarCliente(string n, string c, string cor, char s, string f, unsigned int t);
  
  bool TieneNDigitos();
};


#endif // ESCENA_H
