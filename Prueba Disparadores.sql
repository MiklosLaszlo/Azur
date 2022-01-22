SELECT table_name FROM user_tables;

CREATE TABLE CLIENTE(
    nombreCliente varchar2(50) NOT NULL,
    contrasena varchar2(20) NOT NULL,
    telefono INT PRIMARY KEY,
    correo varchar2(50) NOT NULL,
    sexo varchar2(20),
    fechanacimiento DATE,
    tarjeta INT NOT NULL
);

CREATE TABLE CLIENTEACTIVO(
    telefono INT,
    FOREIGN KEY (telefono) REFERENCES cliente(telefono),
    PRIMARY KEY(telefono)
);

CREATE SEQUENCE secuencia_suministrarIdSesion;

CREATE TABLE SESIONCLIENTESESION(
    idSesion INT PRIMARY KEY,
    telefono INT,
    horaInicio DATE NOT NULL,
    horaFin DATE,
    FOREIGN KEY (telefono) REFERENCES cliente(telefono)
);

CREATE TABLE SESIONACTIVA(
    idSesion INT,
    FOREIGN KEY (idSesion) REFERENCES SESIONCLIENTESESION(idSesion),
    PRIMARY KEY (idSesion)
);

CREATE TABLE PACK(
    nombrePack varchar2(20) PRIMARY KEY,
    precio FLOAT NOT NULL
);

CREATE TABLE PACKACTIVO (
    nombrePack varchar2(20),
    FOREIGN KEY (nombrePack) REFERENCES pack(nombrePack),
    PRIMARY KEY(nombrePack)
);

CREATE TABLE PROVEEDOR(
    cif INT PRIMARY KEY,
    nombreempresa varchar(20) NOT NULL,
    telefonoempresa INT NOT NULL,
    correoempresa varchar(20) NOT NULL
);

CREATE SEQUENCE secuencia_suministrarIdP;

CREATE TABLE SUMINISTRAPELICULA(
	idPelicula INT PRIMARY KEY NOT NULL,
	titulo VARCHAR2(50) UNIQUE NOT NULL,
	anio INT,
	director varchar2(50),
	productora varchar2(50),
	CIF INT,
	FOREIGN KEY (CIF) REFERENCES PROVEEDOR(CIF)
);

CREATE TABLE PELICULAACTIVA(
    idPelicula INT PRIMARY KEY NOT NULL,
    FOREIGN KEY (idPelicula) REFERENCES SUMINISTRAPELICULA(idPelicula)
);

CREATE TABLE VE(
	idSesion int NOT NULL,
	idPelicula int NOT NULL,

	FOREIGN KEY (idSesion) REFERENCES SESIONCLIENTESESION(idSesion),
	FOREIGN KEY (idPelicula) REFERENCES SUMINISTRAPELICULA(idPelicula),
	PRIMARY KEY (idSesion,idPelicula)
);

CREATE SEQUENCE secuencia_contratoCliente;

CREATE TABLE FIRMACLIENTECONTRATOCLIENTE(
    idContratoCliente INT PRIMARY KEY,
    fechainicio DATE NOT NULL,
    fechafin DATE NOT NULL,
    precio REAL NOT NULL CONSTRAINT precioPositivo CHECK (precio>=0),
    telefono INT,
    FOREIGN KEY (telefono) REFERENCES CLIENTE(telefono)
);

CREATE TABLE CONTIENEN(
    idContratoCliente INT,
    nombrepack varchar(20),
    FOREIGN KEY (idContratoCliente) REFERENCES FIRMACLIENTECONTRATOCLIENTE(idContratoCliente),
    FOREIGN KEY (nombrePack) REFERENCES PACK(nombrePack),
    PRIMARY KEY (idContratoCliente,nombrePack)
);

CREATE TABLE RECOMENDACION (
  telefono INT,
  idPelicula INT,
  FOREIGN KEY (telefono) REFERENCES CLIENTE(telefono),
  FOREIGN KEY (idPelicula) REFERENCES SUMINISTRAPELICULA(idPelicula),
  PRIMARY KEY (telefono,idPelicula)
);

--Genera el id de la tabla de contrato-proveedor
CREATE SEQUENCE secuencia_contratoProveedor;
CREATE TABLE FIRMACP(
    idContratoProveedor INT PRIMARY KEY,
    fechainicio DATE NOT NULL,
    fechafin DATE NOT NULL,
    precio REAL NOT NULL CONSTRAINT precioPos CHECK (precio>=0),
    cif INT,
    FOREIGN KEY (cif) REFERENCES PROVEEDOR(cif)
);

CREATE TABLE ACTIVA(
  idPelicula INT,
  idContratoProveedor INT,
  FOREIGN KEY (idPelicula) REFERENCES SUMINISTRAPELICULA(idPelicula),
  FOREIGN KEY (idContratoProveedor) REFERENCES FIRMACP(idContratoProveedor),
  PRIMARY KEY (idPelicula,idContratoProveedor)
);

CREATE TABLE PELICULAPACK(
  idPelicula INT,
  nombrepack varchar(20),
  FOREIGN KEY (idPelicula) REFERENCES SUMINISTRAPELICULA(idPelicula),
  FOREIGN KEY (nombrepack) REFERENCES PACK(nombrepack),
  PRIMARY KEY (idPelicula,nombrepack)
);

--Genera el id de la tabla de factura-cliente
CREATE SEQUENCE secuencia_facturaCliente;
CREATE TABLE FACTURACLIENTEPAGA(
    idfacturac INT PRIMARY KEY,
    fecha DATE NOT NULL,
    precio REAL NOT NULL CONSTRAINT pPositivo CHECK (precio>=0),
    telefono INT,
    FOREIGN KEY (telefono) REFERENCES CLIENTE(telefono)
);

--Genera el id de la tabla de factura-proveedor
CREATE SEQUENCE secuencia_facturaProveedor;
CREATE TABLE FACTURAPROVEEDORRECIBEDINERO(
    idfacturap INT PRIMARY KEY,
    fecha DATE NOT NULL,
    precio REAL NOT NULL CHECK (precio>=0),
    cif INT,
    FOREIGN KEY (cif) REFERENCES PROVEEDOR(cif)
);

-- Disparadores --

-- Cuando se de de alta un cliente, este se activa --
CREATE OR REPLACE TRIGGER crearClienteActivo
AFTER INSERT ON CLIENTE
FOR EACH ROW
BEGIN
    INSERT INTO CLIENTEACTIVO VALUES(:new.telefono);
END crearClienteActivo;
/

-- Comprueba que el telefono tiene 9 dígitos --
CREATE OR REPLACE TRIGGER telefono_correto
BEFORE INSERT ON CLIENTE
FOR EACH ROW
DECLARE
    tam INTEGER;
BEGIN
    SELECT length(to_char(:new.telefono)) INTO tam from dual;
    if(tam != 9) THEN
        RAISE_APPLICATION_ERROR(-20010, 'TELEFONO INVALIDO');
    END IF;
END telefono_correcto;
/

-- Comprueba que la tarjeta tiene 16 dígitos --
CREATE OR REPLACE TRIGGER tarjeta_correcta
BEFORE INSERT OR UPDATE ON CLIENTE
FOR EACH ROW
DECLARE
    tam INTEGER;
BEGIN
    SELECT length(to_char(:new.tarjeta)) INTO tam from dual;
    if(tam != 5) THEN
        RAISE_APPLICATION_ERROR(-20011, 'TARJETA INVALIDA');
    END IF;
END tarjeta_correcta;
/

INSERT INTO CLIENTE VALUES('Pepe', 'Solomillo', 123456789, 'Pepeguit', 'O', TO_DATE('22/10/2005','dd/mm/yyyy'), 1234567890123456);
INSERT INTO CLIENTE VALUES('Pepa', 'Solomillo', 888888888, 'Pepa@g', 'M', TO_DATE('22/10/1980','dd/mm/yyyy'), 1234567890123466);
INSERT INTO CLIENTE VALUES('Jose Manuel', 'Arroz', 111111111, 'jm@g', 'H', TO_DATE('22/10/2000','dd/mm/yyyy'), 1234567890123556);
INSERT INTO CLIENTE VALUES('Rafela', 'Fileta', 222222222, 'Rafaela@g', 'O', TO_DATE('1/1/1990','dd/mm/yyyy'), 1234567890124456);
INSERT INTO CLIENTE VALUES('Ressu', 'Almagro', 333333333, 'Ressu@g', 'M', TO_DATE('22/12/2001','dd/mm/yyyy'), 1234567890123486);
INSERT INTO CLIENTE VALUES('Perro', 'Jamon', 444444444, 'Pepeguit', 'H', TO_DATE('22/11/1900','dd/mm/yyyy'), 1234567890123459);

SELECT * FROM CLIENTE;
SELECT * FROM CLIENTEACTIVO;

-- Crea id sesion --
CREATE OR REPLACE TRIGGER insertar_IdSesion
BEFORE insert on  SESIONCLIENTESESION
FOR EACH ROW
BEGIN
    SELECT secuencia_suministrarIdSesion.nextval INTO :new.idSesion FROM dual;
END insertar_IdSesion;
/

-- Cuando el cliente inicia sesion, se crea sesionActiva --
CREATE OR REPLACE TRIGGER crearSesionActiva
AFTER INSERT ON SESIONCLIENTESESION
FOR EACH ROW
BEGIN
    INSERT INTO SESIONACTIVA VALUES(:new.idSesion);
END crearSesionActiva;
/

-- Comprueba que un cliente solo tiene una sesion activa --
CREATE OR REPLACE TRIGGER sesion_unica
BEFORE insert on SESIONCLIENTESESION
FOR EACH ROW
DECLARE
    ses INTEGER;
BEGIN
    SELECT COUNT(*) INTO ses FROM SESIONCLIENTESESION WHERE telefono=:new.telefono AND (idSesion IN (SELECT idSesion FROM SESIONACTIVA) );
    IF(ses>0) THEN
    	RAISE_APPLICATION_ERROR(-20012, 'SESION UNICA');
    END IF;
END sesion_unica;
/

-- Cuando el cliente se da de baja, se elimina sesionActiva --
CREATE OR REPLACE TRIGGER finSesionActiva
AFTER DELETE ON CLIENTEACTIVO
FOR EACH ROW
BEGIN
    UPDATE SESIONCLIENTESESION SET horaFin=CURRENT_TIMESTAMP WHERE idSesion IN (SELECT idSesion FROM SESIONACTIVA idSesion IN (SELECT idSesion FROM SESIONCLIENTESESION WHERE telefono=:old.telefono));
    DELETE FROM SESIONACTIVA WHERE idSesion IN (SELECT idSesion FROM SESIONCLIENTESESION WHERE telefono=:old.telefono);
END finSesionActiva;
/

-- Cuando el cliente se da de baja, se elimina sesionActiva --
CREATE OR REPLACE TRIGGER finSesionAct
AFTER DELETE ON SESIONACTIVA
FOR EACH ROW
BEGIN
    UPDATE SESIONCLIENTESESION SET horaFin=CURRENT_TIMESTAMP WHERE idSesion=:old.idSesion;
END finSesionActiva;
/

-- Comprueba que el precio sea correcto en los pack --
CREATE OR REPLACE TRIGGER precio_correcto
BEFORE INSERT OR UPDATE ON PACK
FOR EACH ROW WHEN(new.precio < 0)
BEGIN
    RAISE_APPLICATION_ERROR(-20001, 'PRECIO INVALIDO');
END precio_correcto;
/

-- Cuando se modifique o se cree un pack este se activa, en caso de que no este activado ya --
CREATE OR REPLACE TRIGGER crearPackActivo
AFTER INSERT OR UPDATE ON PACK
FOR EACH ROW
DECLARE
    cuantos INTEGER;
BEGIN
    SELECT COUNT(*) INTO cuantos FROM PACKACTIVO WHERE nombrepack= :new.nombrepack;
    IF(cuantos < 1) THEN
        INSERT INTO PACKACTIVO VALUES(:new.nombrepack);
    END IF;
END crearPackActivo;
/

INSERT INTO PACK VALUES('Miedo',20.1);
SELECT * FROM PACK;
SELECT * FROM PACKACTIVO;

-- Comprueba que cuando se inserta una recomendacion esta esta inicialmente recomendada a un cliente activo y a una pelicula activa --
CREATE OR REPLACE TRIGGER comprobarRecomendacionInsertar
BEFORE INSERT ON RECOMENDACION
FOR EACH ROW
DECLARE
    cli INTEGER;
    ple INTEGER;
BEGIN
    SELECT COUNT(*) INTO cli FROM CLIENTEACTIVO WHERE telefono= :new.telefono;
    SELECT COUNT(*) INTO ple FROM PELICULAACTIVA WHERE idPelicula= :new.idPelicula;
    IF(cli < 1) THEN
      RAISE_APPLICATION_ERROR(-20002, 'CLIENTE NO ACTIVO RECIBIENDO RECOMENDACION');
    ELSIF(ple < 1) THEN
      RAISE_APPLICATION_ERROR(-20003, 'PELICULA NO ACTIVA ASOCIANDOSE A RECOMENDACION');
    END IF;
END comprobarRecomendacionInsertar;
/

CREATE OR REPLACE TRIGGER insertar_ContratoCliente
  before insert on firmaClienteContratoCliente
  for each row
    DECLARE
      cli INTEGER;
    BEGIN
      --el tlf que quiero insertar
      SELECT COUNT(*) INTO cli FROM ClienteActivo WHERE telefono=:new.telefono;
      IF (cli<1) THEN
        RAISE_APPLICATION_ERROR(-20100, 'INTENTO DE CREAR CONTRATO A UN CLIENTE NO ACTIVO');
      END IF;
      SELECT secuencia_contratoCliente.nextval INTO :new.idContratoCliente FROM dual;
    END insertar_ContratoCliente;
/

--Crea id contrato proveedor
CREATE OR REPLACE TRIGGER insertar_ContratoProveedor
  before insert on FIRMACP
  for each row
    BEGIN
      SELECT secuencia_contratoProveedor.nextval INTO :new.idContratoProveedor FROM dual;
    END insertar_ContratoProveedor;
/

--
CREATE OR REPLACE TRIGGER fechacontrato_completa
BEFORE INSERT ON FIRMACP
FOR EACH ROW
BEGIN
  IF(:new.fechafin<:new.fechainicio) THEN
    RAISE APPLICATION_ERROR(-20104, 'FECHA DE FIN ANTERIOR A INICIO');
  END IF;
END fechacontrato_completa
/

--Crea id factura cliente
CREATE OR REPLACE TRIGGER insertar_FacturaCliente
  before insert on facturaClientePaga
  for each row
    BEGIN
      SELECT secuencia_facturaCliente.nextval INTO :new.idfacturac FROM dual;
    END insertar_FacturaCliente;
/

--Crea id factura proveedor
CREATE OR REPLACE TRIGGER insertar_FacturaProveedor
  before insert on facturaProveedorRecibeDinero
  for each row
    BEGIN
      SELECT secuencia_facturaProveedor.nextval INTO :new.idfacturap FROM dual;
    END insertar_FacturaProveedor;
/

/*
DROP TRIGGER comprobarPeliculaInsertada;
CREATE OR REPLACE TRIGGER comprobarPeliculaInsertada
BEFORE INSERT ON SUMINISTRAPELICULA
FOR EACH ROW
DECLARE
	idP INTEGER;
BEGIN
	SELECT COUNT(*) INTO idP FROM SUMINISTRAPELICULA WHERE idPelicula = :new.idPelicula;

	IF (idP >= 1) THEN
		RAISE_APPLICATION_ERROR(-20015, 'PELÍCULA YA SUMINISTRADA');
	END IF;
END comprobarPeliculaInsertada;
/
*/

-- Crea el id de la pelicula --
CREATE OR REPLACE TRIGGER insertarNuevoIdPelicula
BEFORE INSERT ON SUMINISTRAPELICULA
FOR EACH ROW
BEGIN
	SELECT secuencia_suministrarIdP.nextval INTO :new.idPelicula FROM dual;
END insertarNuevoIdPelicula;
/

-- Cuando se desactiva la pelicula se desactiva el pack --
CREATE OR REPLACE TRIGGER desactivarPacks
AFTER DELETE ON PELICULAACTIVA
FOR EACH ROW
BEGIN
	DELETE FROM PACKACTIVO WHERE nombrepack IN (SELECT nombrepack FROM PELICULAPACK WHERE idPelicula = :old.idPelicula);
END desactivarPacks;
/

-- Asegura que el cliente que ve quiere ver la peli con idpelicula verifique que esta en esta un pack del contrato activo del cliente --
CREATE OR REPLACE TRIGGER confirmave
BEFORE INSERT ON VE
FOR EACH ROW
DECLARE
  ple INTEGER;
BEGIN
	  SELECT COUNT(*) INTO ple FROM PELICULAACTIVA WHERE idPelicula=:new.idPelicula AND (idPelicula IN
      (SELECT idPelicula FROM PELICULAPACK WHERE nombrePack IN
        (SELECT nombrePack FROM CONTIENEN WHERE idContratoCliente IN(
          SELECT idContratoCliente
            FROM FIRMACLIENTECONTRATOCLIENTE WHERE (fechafin < CURRENT_TIMESTAMP) AND (telefono IN (
                SELECT telefono FROM SESIONCLIENTESESION WHERE idSesion IN (
                  SELECT idSesion FROM SESIONACTIVA WHERE idSesion=:new.idSesion
                )
            )
            )
          order by fechainicio desc
      )
    );
    IF(ple>0) THEN
    	RAISE_APPLICATION_ERROR(-20050, 'La pelicula o no existe o no la tiene contratada actualmente');
    END IF;
END confirmave;
/
       
--
CREATE OR REPLACE TRIGGER activarpeli
AFTER INSERT ON ACTIVA
FOR EACH ROW
DECLARE
  ple INTEGER;
BEGIN
  SELECT COUNT(*) INTO ple FROM PELICULAACTIVA WHERE idPelicula=:new.idPelicula;
  IF (ple < 1) THEN
    INSERT INTO PELICULAACTIVA VALUES(:new.idPelicula);
  END IF;
END activarpeli;
/
       
--
Itrilor
#6772
Miklos Laszlo, Geri, holygoo, GabiOO

Miklos Laszlo — 08/01/2022
ehh si eso podemos ir corrigiendo cosas? O hacer que os funcione sqlapi
Itrilor — 08/01/2022
Yo llevo aqui un rato
Miklos Laszlo
 ha iniciado una llamada que ha durado 3 horas.
 — 08/01/2022
Miklos Laszlo — 08/01/2022
Muy buenas soy Nicolás y le quería preguntar que para realizar la defensa del trabajo del tema 4, basta con coger una cita de tutoría o si hay que hacer algo más?.
Geri — 08/01/2022
Muy buenas. Soy Nicolás y le quería preguntar que si para realizar la defensa del trabajo del tema 4, ¿basta con coger una cita de tutoría o hay que hacer algo más?
GabiOO — 08/01/2022
disparador para comprobar lo de las pelis que no estén contratadas a la vez
Si una peli figura
en un contrato vigente, no se puede volver a contratar hasta que dicho contrato caduque
GabiOO — 08/01/2022
Restricción semántica, un contrato con un proveedor solo involucra pelis suministradas por él
GabiOO — 08/01/2022
hay que modelar lo que hacen los triggers? (El trigger que solo escribe recom de activos)
Itrilor
 ha iniciado una llamada que ha durado 6 horas.
 — 15/01/2022
GabiOO — 15/01/2022
estoy en breves
Miklos Laszlo — 15/01/2022
https://www.sqlapi.com/ApiDoc/class_s_a_date_time.html
SADateTime | SQLAPI++
SQLAPI++ is C++ library for accessing SQL databases (Oracle, SQL Server, Sybase, DB2, InterBase, SQLBase, Informix, MySQL, Postgre, ODBC, SQLite, SQL Anywhere).
It provides unified API for accessing different database while also allowing access to native features.
Imagen
Miklos Laszlo — 15/01/2022
-- Comprueba que el precio sea correcto en los pack
CREATE OR REPLACE TRIGGER precio_correcto
BEFORE INSERT OR UPDATE ON PACK
FOR EACH ROW WHEN(new.precio < 0)
BEGIN
    RAISE_APPLICATION_ERROR(-20001, 'PRECIO INVALIDO');
END precio_correcto;
/

-- Cuando se modifique o se cree un pack este se activa, en caso de que no este activado ya -- 
CREATE OR REPLACE TRIGGER crearPackActivo
AFTER INSERT OR UPDATE ON PACK
FOR EACH ROW
DECLARE
    cuantos INTEGER;
BEGIN
    SELECT COUNT(*) INTO cuantos FROM PACKACTIVO WHERE nombrepack= :new.nombrepack;
    IF(cuantos < 1) THEN
        INSERT INTO PACKACTIVO VALUES(:new.nombrepack);
    END IF;
END crearPackActivo;
/
Geri — 15/01/2022
Gerardo2000
Miklos Laszlo — 15/01/2022
SELECT * FROM SESIONACTIVA NATURAL JOIN CLIENTESESION  WHERE telefono=:1 and idSesion=:2
Miklos Laszlo — 16/01/2022
https://github.com/MiklosLaszlo/Azur
GitHub
GitHub - MiklosLaszlo/Azur: Practica 3
Practica 3. Contribute to MiklosLaszlo/Azur development by creating an account on GitHub.

Tienes una llamada perdida de 
Miklos Laszlo
 que ha durado 5 minutos.
 — ayer a las 16:59
GabiOO — ayer a las 17:05
holis
Miklos Laszlo — ayer a las 17:06
t
Tienes una llamada perdida de 
Miklos Laszlo
 que ha durado unos segundos.
 — ayer a las 17:06
GabiOO — ayer a las 17:06
amos a esperar aue vengasn los otros
Miklos Laszlo — ayer a las 17:06
kay
Geri — ayer a las 17:07
¡Hola!
GabiOO — ayer a las 17:07
falta irene
s
Esq para esto del main
Todo tenemos que ver qué hacer y ver qué formato darle
Miklos Laszlo — ayer a las 17:08
voy a probar a llamarla que me dijo que estaba KO, en el peor de los casos avanzamos sin ella (me gustaria estar todos) pero quiero acabar hoy la práctica
GabiOO — ayer a las 17:08
ouh,ok
GabiOO — ayer a las 17:22
parece que f
Miklos Laszlo — ayer a las 17:22
yep
La he intetado llamar, me lo ha cogido pero parece ser que se volvio a acostar
He probado decirle a Victor algo pero confirma que esta muy KO
Si eso, podemos ir perfilando cosas y la parte pues le ayudo luego por tarde-noche que supongo que ha esa hora estara despierta
GabiOO — ayer a las 17:24
Pero cuando te  lo cogio te dijo que sí que venía?
Miklos Laszlo — ayer a las 17:24
Respondio monotono, pero supongo que estaba cansada
GabiOO — ayer a las 17:24
ok
GabiOO
 ha iniciado una llamada que ha durado 9 horas.
 — ayer a las 17:25
GabiOO — ayer a las 17:26
se me escucha?
se me escucha mejor?
Igual de mal o peor?
Miklos Laszlo — ayer a las 17:56
Cambiar en requisititos titulo peli -> Clave candidata
Miklos Laszlo — ayer a las 18:08
/SELECT id_pelicula FROM peliculasactivas
  where id_peliucla=:1 and (id_pelicula in ( Select id_pelicula From PAckPElicula
      where nombrepakc in (Select nombrePack from PackActivo
        where nombrePAck IN (Select nombrePack From Contienen
          Where idContrato IN (Select idContrato From ContratoCliente
            Where idContrato IN (Select idContrato From ClienteContrato
              Where tle IN (Select tle From SesionCliente Where idSesion = :2)
            )
          )
        )
      )
    )
  );/
Miklos Laszlo — ayer a las 20:39
SELECT COUNT(*) INTO ncll FROM SESIONCLIENTESESION WHERE telefono=:new.telefono and (idSesion IN SELECT idSesion FROM SESIONACTIVA)
Miklos Laszlo — ayer a las 21:53
CURRENT_TIMESTAMP
Miklos Laszlo — ayer a las 22:41
CHECK (estadocivil IN ('soltero', 'casado', 'divorciado', 'viudo')
Itrilor — hoy a las 0:19
SANumeric no hace falta usarlo
Miklos Laszlo — hoy a las 0:46
reinsertar triggers finSesionActiva y finSesionAct
Itrilor — hoy a la 1:11
Añadir un bucle en el submenu de cliente (antes del cin)
Miklos Laszlo — hoy a las 19:44
Voy a retrasarme un poco
GabiOO — hoy a las 19:45
ouh ok no problem
Y los demás estamos activos?
como veo que sí, pues vamos a esperar a Nico
Miklos Laszlo — hoy a las 19:50
ya estoy
Miklos Laszlo
 ha iniciado una llamada.
 — hoy a las 19:50
Miklos Laszlo — hoy a las 19:50
Estoy con Irene
GabiOO — hoy a las 20:04
holo
x7485923
Miklos Laszlo — hoy a las 20:25
TOP 100 PERCENT
Itrilor — hoy a las 21:40
ALTER TABLE CLIENTE ALTER COLUMN sexo varchar2(20);
GabiOO — hoy a las 21:46
wenas
GabiOO — hoy a las 22:14
requisito semántico 5 dígitos
GabiOO — hoy a las 23:02
modificar cliente
Geri — hoy a las 23:12
Disparador peliculaPack
GabiOO — hoy a las 23:12
disparador pelipack
Miklos Laszlo — hoy a las 23:21
CREATE OR REPLACE TRIGGER activarpeli
AFTER INSERT ON ACTIVA
DECLARE
  ple INTEGER;
BEGIN:
  SELECT COUNT(*) INTO ple FROM PELICULAACTIVA WHERE idPelicula=:new.idPelicula;
  IF (ple < 1) THEN
    INSERT INTO PELICULAACTIVA VALUES(:new.idPelicula);
  END IF;
END activarpeli;
/

--
CREATE OR REPLACE TRIGGER activarpeli
AFTER INSERT ON ACTIVA
FOR EACH ROW
DECLARE
  ple INTEGER;
BEGIN
  SELECT COUNT(*) INTO ple FROM PELICULAACTIVA WHERE idPelicula=:new.idPelicula;
  IF (ple < 1) THEN
    INSERT INTO PELICULAACTIVA VALUES(:new.idPelicula);
  END IF;
END activarpeli;
/

--
CREATE OR REPLACE TRIGGER comprobarpeliactiva
AFTER INSERT ON PELICULAPACK
FOR EACH ROW
DECLARE
  ple INTEGER;
BEGIN
  SELECT COUNT(*) INTO ple FROM PELICULAACTIVA WHERE idPelicula=:new.idPelicula;
  IF (ple < 1) THEN
    RAISE_APPLICATION_ERROR(-20004,'Pelcula no activa')
  END IF;
END activarpeli;
/
