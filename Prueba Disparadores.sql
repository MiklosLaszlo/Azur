SELECT table_name FROM user_tables;

CREATE TABLE CLIENTE(
    nombreCliente varchar2(50) NOT NULL,
    contrasena varchar2(20) NOT NULL,
    telefono INT PRIMARY KEY,
    correo varchar2(50) NOT NULL,
    sexo CHAR CHECK (sexo IN ('H', 'M', 'O' )),
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
    if(tam != 16) THEN
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
