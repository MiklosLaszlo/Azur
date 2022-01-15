#ifndef CLIENTES_H
#define CLIENTES_H

class Cliente{
  Cliente(string n, string c, int telf, string correo, char s, string fecha, int t);
  
  void IniciarSesion(int telf);
  void FinalizarSesion(int id);
  void DarBajaCliente();
  void ModificarCliente();
};


#endif // ESCENA_H
