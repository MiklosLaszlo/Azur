#include <SQLAPI.h>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "peliculas.h"


void Peliculas::MostrarCatalogo(SAConnection* con){
	SACommand comando;
    comando.setConnection(con);
    cout<<"Nuestras Películas: "<<endl;
    comando.setCommandText(_TSA("SELECT titulo,director,anio,productora FROM PELICULAACTIVA"));
    try{comando.Execute();}
    catch(SAException &x){
    	cout<<x.ErrText().GetMultiByteChars()<<endl;
    }
    cout<<" Titulo\tDirector\tAño\tProductora"<<endl;
    while(comando.FetchNext()) {
        cout<<" "<<comando[1].asString()<<"\t\t"<<comando[2].asString()<<"\t\t"<<comando[3].asInt64()<<"\t\t"<<comando[4].asString()<<endl;
    }
}


void Peliculas::InhabilitarPelicula(int idP, SAConnection* con)
  SACommand guardado, inhabilitar;
  guardado.setConnection(con);
  guardado.setCommandText(_TSA("SAVEPOINT inhabilitarpelicula"));

  try{
    guardado.Execute();
  }
  catch(SAException &x){
    cerr << x.ErrText().GetMultiByteChars() << endl;
    cerr << "Error a la hora de crear el SAVEPOINT" << endl;
    return ;
  }

  SANumeric auxiliar(idP);
  inhabilitar.setConnection(con);
  inhabilitar.setCommandText(_TSA("DELETE FROM PELICULAACTIVA WHERE idPelicula=:1"));
  inhabilitar.Param(1).SetAsInt() = auxiliar;

  try{
    inhabilitar.Execute();
  }
  catch(SAException &x){
    cerr << "Error a la hora de inhabilitar la pelicula" << endl;
    cerr << x.ErrText().GetMultiByteChars() << endl;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT inhabilitarpelicula"));
    guardado.Execute();
  }

  con->commit();
}


void Pelicula::SuministrarPelicula(string t, int a, string d, string p, SAConnection* con){
	SACommand guardado, suministrar, busquedaId;
	guardado.setConnection(con);
	guardado.setCommandText(_TSA("SAVEPOINT suministrarpelicula"));
	
	try{
    	guardado.Execute();
  	}
  	catch(SAException &x){
    	cerr<<x.ErrText().GetMultiByteChars()<<endl;
    	cerr<<"Error a la hora de crear el SAVEPOINT" << endl;
    	return ;
  	}
  	
  	SAString auxt(t.c_str());
  	SANumeric auxa(a);
  	SAString auxd(d.c_str());
  	SAString auxp(p.c_str());
  	
  	busquedaId.setConnection(con);
  	busquedaId.setCommandText(_TSA("SELECT MAX(idPelicula) FROM pelicula"));
  	busquedaId.Execute();
  	
  	int auxId = busquedaId[1].asInt64() + 1;  //CREO QUE HAY PROBLEMAS EN LA PRIMERA INSERCIÓN (NO SE SI DEVUELVE 0 O NULL) CREO QUE TENGO QUE CREAR TRIGGER
  	
  	suministrar.setConnection(con);
  	suministrar.setCommandText(_TSA("INSERT INTO pelicula (idPelicula,titulo,director,anio,productora) VALUES (:1,:2,:3,:4,:5)"));
  	suministrar.Param(1).setAsInt64() = auxId;
  	suministrar.Param(2).setAsString() = auxt;
  	suministrar.Param(3).setAsString() = auxd;
  	suministrar.Param(4).setAsInt64() = auxa;
  	suministrar.Param(5).setAsString() = auxp;
  	
  	try{
    	suministrar.Execute();
  	}
  	catch(SAException &x){
    	cerr << "Error a la hora de suministrar la pelicula" << endl;
    	cerr << x.ErrText().GetMultiByteChars() << endl;
    	guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT suministrarpelicula"));
    	guardado.Execute();
  	}

  	con->commit();
}


void Pelicula::BuscarTituloCatalogo(string t, SAConnection *con){
	SACommand comando;
	SAString auxt(t.c_str());
	
    comando.setConnection(con);
    cout<<"Películas con el título buscado: "<<endl;
    comando.setCommandText(_TSA("SELECT titulo FROM PELICULAACTIVA WHERE titulo = :1")); 
    comando.Param(1).setAsString() = auxt;
    
    try{comando.Execute();}
    catch(SAException &x){
    	cout<<x.ErrText().GetMultiByteChars()<<endl;
    }
    
    cout<<" Titulo\tDirector\tAño\tProductora"<<endl;
    while(comando.FetchNext()) {
        cout<<" "<<comando[1].asString()<<"\t\t"<<comando[2].asString()<<"\t\t"<<comando[3].asInt64()<<"\t\t"<<comando[4].asString()<<endl;
    }
}	


void Pelicula::MostrarRecomendaciones(int telefono, SAConnection *con){
	SACommand comando;
	SANumeric auxtel(telefono);
	
	comando.setConnection(con);
	cout << "Películas recomendadas para ti" << endl;
	comando.setCommandText((_TSA("SELECT idPelicula FROM RECOMENDACION NATURAL JOIN PELICULAACTIVA WHERE telefono = :1")));
	comando.Param(1).setAsInt64() = auxtel;
	
	try{comando.Execute();}
    catch(SAException &x){
    	cout<<x.ErrText().GetMultiByteChars()<<endl;  //¿Qué pasaría si no hubiese recomendaciones para esa persona? CREO QUE TENGO QUE CREAR TRIGGER
    }
    
    cout<<" Titulo\tDirector\tAño\tProductora"<<endl;
    while(comando.FetchNext()) {
        cout<<" "<<comando[1].asString()<<"\t\t"<<comando[2].asString()<<"\t\t"<<comando[3].asInt64()<<"\t\t"<<comando[4].asString()<<endl;
    }
}


/*AHORA MISMO SOLO BUSCO (O INTENTO) SI EL IDPELICULA ESTA EN EL PACK DEL CLIENTE*/
void Pelicula::VerPelicula(int idSesion, int idPel, SAConnection *con){
	SACommand busqueda;
	SANumeric auxidSes(idSesion);
	SANumeric auxidPel(idPel);
	
	busqueda.setConnection(con);
	
	busqueda.setCommandText(_TSA("SELECT idPelicula FROM CLIENTE NATURAL JOIN CONTRATOCLIENTE NATURAL JOIN PACKACTIVO NATURAL JOIN CLIENTESESION WHERE idSesion = :1 AND idPelicula = :2"));
	busqueda.Param(1).setAsInt64() = auxidSes;
	busqueda.Param(2).setAsInt64() = auxidPel;
	
	try{busqueda.Execute();}
    catch(SAException &x){
    	cout<<x.ErrText().GetMultiByteChars()<<endl;  
    }
	
	
}


