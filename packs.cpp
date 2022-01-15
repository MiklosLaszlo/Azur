
#include <SQLAPI.h>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "packs.h"

using namespace std;

//HACER
void InformarNovedades(vector<string> listaPacks, SAConnection* con){
  SACommand telclientes;
  SACommand packs;
  telclientes.setConnection(con);
  packs.setConnection(con);
  vector<String> packsEnviar;
  ofstream simulo_correos;

  if(simulo_correos.open("Correos_enviados.txt")){
    telclientes.setCommandText(_TSA("SELECT correo FROM clientes WHERE telefono IN (SELECT * FROM clientesActivos)")); // Obtengo el correo de todos los clientes activos
    telclientes.Execute();
    // Para cada elemento de lista Packs, mirar si esta en PAcks Activos y luego enviar el correo...


  }

  else{
    cerr << "No se ha podido abrir el canal para escribir correos" << endl
  }
};

//REVISAR LUEGO
//Se tiene que modificar el pack y luego activarlo o no se hace nada
void ModificarPack(vector<unsigned> idPeliculas, double precio ,string idPack ,SAConnection* con);{
  SACommand guardado, modificar;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT modifcarpack"));

  try{
    guardado.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }

  SAString auxiliar(idPack.c_str());
  modificar.setConnection(con);
  modificar.setCommandText(_TSA("SELECT * FROM PACK WHERE idPack = :1"));
  modificar.Param(1).setAsString() = auxiliar;
  modificar.Execute();
  if(!(modificar.FetchNext() )){
    cerr << "El pack que se quiere modificar no pertenece a la base de datos" << endl;
  }
  else{
    // Modifica los datos internos del Pack
    modificar.setCommandText(_TSA("UPDATE PACK set precio=:1 WHERE idPack=:2")); // con un trigger se puede controlar el insert en packs activos
    modificar.Param(2).setAsString() = auxiliar;
    modificar.Param(1).setAsDouble() = precio;
    try{
      modificar.Execute();
    }
    catch(SAException &x){
      cerr << "Error al modificar el precio del pack, se cancelaran todos los cambios" << endl;
      int i = x.ErrNativeCode();
      // Manejador de Excepciones
      switch(i){
        case 20001:
          cout << "El precio tiene que ser mayor o igual que 0" << endl;
          break;

        default:
          cout << "Excepción no controlada" << endl;
          ccout<<x.ErrText().GetMultiByteChars()<<endl;
          break;
      }
      guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modifcarpack"));
      guardado.Execute();
      return;
    }

    //Borra todas las peliculas que estaban ligadas al PAck antes
    modificar.setCommandText(_TSA("DELETE * FROM Peliculapack WHERE idPack = :1"));
    modificar.Param(1).setAsString() = auxiliar;
    try{
      modificar.Execute();
    }
    catch(SAException &x){
      cerr << "Error al borrar las peliculas antiguas del Pack, se cancelaran todos los cambios" << endl;
      cerr <<x.ErrText().GetMultiByteChars() << endl;
      guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modifcarpack"));
      guardado.Execute();
      return;
    }
    //Añade todas las peliculas en el pack, relacionandolas
    try{
      for(int i=0; i < idPeliculas.size(); i++){
        modificar.setCommandText(_TSA("INSERT INTO Peliculapack VALUES(:1 , :2)")); //Inserta primera columna la peli y luego el pack
        modificar.Param(1).setAsUInt64() = idPeliculas[i];
        modificar.Param(2).setAsString() = auxiliar;
        modificar.Execute();
      };
    }

    catch(SAException &x){
      cerr << "Error al añadir las peliculas en el Pack, se cancelaran todos los cambios" << endl;
      guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modifcarpack"));
      guardado.Execute();
      return;
    }

    // Activa el Pack, es decir lo añade en PAcksActivos en caso de que no estuviera
    cout << "Activando el pack..." << endl;
    modificar.setCommandText(_TSA("INSERT INTO packactivos VALUES(:1)"));
    modificar.Param(1).setAsString() = auxiliar;
    try{
      modificar.Execute();
      cout << "Se ha activado el Pack" << endl;
    }
    catch(SAException &x){
      int i = x.ErrNativeCode();
      switch(i){
        case 1:
          cout << "El pack ya estaba activado" << endl;
          break;

        default:
          cout << "Excepción no controlada" << endl;
          ccout<<x.ErrText().GetMultiByteChars()<<endl;
          guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT modifcarpack"));
          guardado.Execute();
          break;
      }
    }
  }
  con->commit();

};

//REVISAR LUEGO
// O se crea todo y se activa el pack o no se hace nada
void CrearPack(vector<unsigned> idPeliculas, double precio ,string idPack ,SAConnection* con){
  SACommand guardado, crear;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT crearpack"));

  try{
    guardado.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }

  SAString auxiliar(idPack.c_str());
  crear.setConnection(con);
  crear.setCommandText(_TSA("INSERT INTO pack VALUES(:1 , :2)")); // Con un TRIGGER se puede controlar que insert en packs activos
  crear.Param(1).setAsString() = auxiliar;
  crear.Param(2).setAsDouble() = precio;

  try{
    crear.Execute();
  }
  catch(SAException &x){
    int i = x.ErrNativeCode();
    switch(i){
      case 1:
        cout << "YA existia un pack con ese nombre, use ModificarPack si quiere cambiar el nombre" << endl;
        break;

      case 20001:
        cout << "EL precio tiene que ser mayor o igual que 0" << endl;
        break;

      default:
        cout << "Excepción no controlada" << endl;
        cout<<x.ErrText().GetMultiByteChars()<<endl;
        break;
    }

    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT crearpack"));
    guardado.Execute();
    return ;
  }

  try{
    for(int i=0; i < idPeliculas.size(); i++){
      crear.setCommandText(_TSA("INSERT INTO Peliculapack VALUES(:1 , :2)")); //Inserta primera columna la peli y luego el pack (el insert se puede controlar para que sean de peliculas activas)
      crear.Param(1).setAsUInt64() = idPeliculas[i];
      crear.Param(2).setAsString() = auxiliar;
      crear.Execute();
    };
  }

  catch(SAException &x){
    cerr << "Error al añadir las peliculas en el Pack, se cancelaran todos los cambios" << endl;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT crearpack"));
    guardado.Execute();
    out<<x.ErrText().GetMultiByteChars()<<endl;
    return ;
  }

  // Activa el Pack, es decir lo añade en PAcksActivos en caso de que no estuviera
  cout << "Activando el pack..." << endl;
  crear.setCommandText(_TSA("INSERT INTO packactivos VALUES(:1)"));
  crear.Param(1).setAsString() = auxiliar;
  try{
    crear.Execute();
    cout << "Se ha activado el Pack" << endl;
  }
  catch(SAException &x){ // QUE ERROR PUEDE DAR????
    ccout<<x.ErrText().GetMultiByteChars()<<endl;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT crearpack"));
    guardado.Execute();
  }

  con->commit();

};

//REVISAR LUEGO
void DesactivarPack(string idPack ,SAConnection* con){
  SACommand guardado, desactivar;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT desactivarpack"));

  try{
    guardado.Execute();
  }
  catch(SAException &x){
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }

  SAString auxiliar(idPack.c_str());
  desactivar.setConnection(con);
  desactivar.setCommandText(_TSA("DELETE FROM PACKACTIVO WHERE nombrePack=:1"));
  desactivar.Param(1).setAsString() = auxiliar;

  try{
    desactivar.Execute();
  }
  catch(SAException &x){
    cerr << "Error a la hora de desactivar el pack" << endl;
    cerr<<x.ErrText().GetMultiByteChars()<<endl;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT desactivarpack"));
    guardado.Execute();
  }

  con->commit();
};

//HACER
void RecibirRecomendaciones(vector< pair <unsigned, unsigned >> clienteRPeliculas ,SAConnection* con){
  SACommand recomendaciones;
  recomendaciones.setConnection(con);
  recomendaciones.setCommandText(_TSA("INSERT INTO recomendacion VALUES(:1,:2)"));

  for(int i = 0; i < clienteRPeliculas.size(); i++){
    recomendaciones.Param(1).setAsUInt64() = clienteRPeliculas[i].first;
    recomendaciones.Param(2).setAsUInt64() = clienteRPeliculas[i].first;
    try{
      recomendaciones.Execute();
    }
    catch(SAException &x){
      cerr << "Error al insertar la " << i << "-esima insercion." << endl;
      switch (x.ErrNativeCode()) {
        case 20002:

          break;
        case 20003:

          break;

        default
          cerr << "Execepcion no manejada" << endl;
          cerr<<x.ErrText().GetMultiByteChars()<<endl;
          break;
      }
    }
  }

};
