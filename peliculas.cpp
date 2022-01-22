#include <SQLAPI.h>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "peliculas.h"


void MostrarCatalogo(SAConnection* con){
	SACommand comando;
    comando.setConnection(con);
    cout<<"Nuestras Películas: "<<endl;
    comando.setCommandText(_TSA("SELECT idPelicula,titulo,director,anio,productora FROM PELICULAACTIVA NATURAL JOIN SUMINISTRAPELICULA"));
    try{comando.Execute();}
    catch(SAException &x){
    	cout<<x.ErrText().GetMultiByteChars()<<endl;
    }
    cout<<" IdPelicula\tTitulo\tDirector\tAño\tProductora"<<endl;
    while(comando.FetchNext()) {
        cout<<" "<<comando[1].asInt64()<<"\t\t"<<comando[2].asString().GetMultiByteChars()<<"\t\t"<<comando[3].asString().GetMultiByteChars()<<"\t\t"<<comando[4].asInt64()<<"\t\t"<<comando[5].asString().GetMultiByteChars()<<endl;
    }
}


void InhabilitarPelicula(int idP, SAConnection* con){
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

  inhabilitar.setConnection(con);
  inhabilitar.setCommandText(_TSA("DELETE FROM PELICULAACTIVA WHERE idPelicula=:1"));
  inhabilitar.Param(1).setAsInt64() = idP;

  try{
    inhabilitar.Execute();
  }
  catch(SAException &x){
    cerr << "Error a la hora de inhabilitar la pelicula" << endl;
    cerr << x.ErrText().GetMultiByteChars() << endl;
    guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT inhabilitarpelicula"));
    guardado.Execute();
  }

  con->Commit();
}


void SuministrarPelicula(string t, int a, string d, string p, int CIF, SAConnection* con){
	SACommand guardado, suministrar, selectID, suministrarId;
	guardado.setConnection(con);
	selectID.setConnection(con);
	suministrarId.setConnection(con);
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
  	SAString auxd(d.c_str());
  	SAString auxp(p.c_str());

  	suministrar.setConnection(con);
  	suministrar.setCommandText(_TSA("INSERT INTO SUMINISTRAPELICULA (titulo,director,anio,productora,CIF) VALUES (:1,:2,:3,:4,:5)"));
  	suministrar.Param(1).setAsString() = auxt;
  	suministrar.Param(2).setAsString() = auxd;
  	suministrar.Param(3).setAsInt64() = a;
  	suministrar.Param(4).setAsString() = auxp;
	suministrar.Param(5).setAsInt64() = CIF;


  	try{
    	suministrar.Execute();
  	}
  	catch(SAException &x){
    	cerr << "Error a la hora de suministrar la pelicula" << endl;
    	cerr << x.ErrText().GetMultiByteChars() << endl;
    	guardado.setCommandText(_TSA("ROLLBACK TO SAVEPOINT suministrarpelicula"));
    	guardado.Execute();
  	}

	/*selectID.setCommandText(_TSA("SELECT secuencia_suministrarIdP.currval FROM dual"));
 	 try{
    	selectID.Execute();
    	selectID.FetchNext(); //Dispongo la información del select
  	}
  	catch(SAException &x){
    	cerr<<x.ErrText().GetMultiByteChars()<<endl;
	}
  	int id = selectID.Param(1).asInt64(); NO HACE FALTA*/

  	con->Commit();
}


void BuscarTituloCatalogo(string t, SAConnection *con){
	SACommand comando;
	SAString auxt(t.c_str());

    comando.setConnection(con);
    cout<<"Películas con el título buscado: "<<endl;
    comando.setCommandText(_TSA("SELECT titulo FROM PELICULAACTIVA NATURAL JOIN SUMINISTRAPELICULA WHERE titulo = :1"));
    comando.Param(1).setAsString() = auxt;

    try{comando.Execute();}
    catch(SAException &x){
    	cout<<x.ErrText().GetMultiByteChars()<<endl;
    }

    cout<<" IdPelicula\tTitulo\tDirector\tAño\tProductora"<<endl;
    while(comando.FetchNext()) {
        cout<<" "<<comando[1].asInt64()<<"\t\t"<<comando[2].asString().GetMultiByteChars()<<"\t\t"<<comando[3].asString().GetMultiByteChars()<<"\t\t"<<comando[4].asInt64()<<"\t\t"<<comando[5].asString().GetMultiByteChars()<<endl;
    }
	con -> Commit();
}


void MostrarRecomendaciones(int telefono, SAConnection *con){
	SACommand comando;

	comando.setConnection(con);
	cout << "Películas recomendadas para ti" << endl;
	comando.setCommandText((_TSA("SELECT idPelicula FROM RECOMENDACION NATURAL JOIN PELICULAACTIVA NATURAL JOIN SUMINISTRAPELICULA WHERE telefono = :1")));
	comando.Param(1).setAsInt64() = telefono;

	try{comando.Execute();}
    catch(SAException &x){
    	cout<<x.ErrText().GetMultiByteChars()<<endl;  //¿Qué pasaría si no hubiese recomendaciones para esa persona? CREO QUE TENGO QUE CREAR TRIGGER
    }

    cout<<" IdPelicula\tTitulo\tDirector\tAño\tProductora"<<endl;
    while(comando.FetchNext()) {
        cout<<" "<<comando[1].asInt64()<<"\t\t"<<comando[2].asString().GetMultiByteChars()<<"\t\t"<<comando[3].asString().GetMultiByteChars()<<"\t\t"<<comando[4].asInt64()<<"\t\t"<<comando[5].asString().GetMultiByteChars()<<endl;
    }

	con -> Commit();
}


/*AHORA MISMO SOLO BUSCO (O INTENTO) SI EL IDPELICULA ESTA EN EL PACK DEL CLIENTE*/
void VerPelicula(int idSesion, int idPel, SAConnection *con){
	SACommand busqueda;

	busqueda.setConnection(con);

	busqueda.setCommandText(_TSA("SELECT idPelicula FROM SESIONCLIENTESESION NATURAL JOIN CONTRATOCLIENTE NATURAL JOIN PACKACTIVO NATURAL JOIN PELICULAPACK WHERE idSesion = :1 AND idPelicula = :2"));
	busqueda.Param(1).setAsInt64() = idSesion;
	busqueda.Param(2).setAsInt64() = idPel;

	try{busqueda.Execute();}
    catch(SAException &x){
    	cout<<x.ErrText().GetMultiByteChars()<<endl;
    }

	con->Commit();

}
