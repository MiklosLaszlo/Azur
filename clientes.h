#ifndef CLIENTES_H
#define CLIENTES_H

class Cliente{
  protected:
  string nombre;
  string contrasena;
  int telefono;
  string correo;
  char sexo;
  string fecha_nacimiento;
  int tarjeta;
  
  public:
  
  Cliente(string n, string c, int telf, string cor, char s, string f, int t);
  
  void IniciarSesion(int telf);
  void FinalizarSesion(int id);
  void DarBajaCliente();
  void ModificarCliente();
};


#endif // ESCENA_H
