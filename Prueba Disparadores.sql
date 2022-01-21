SELECT table_name FROM user_tables;

DROP TRIGGER precio_correcto;

DROP TABLE PACK;

-- CREAR tablas --

CREATE TABLE CLIENTE(
    nombreCliente varchar2(50) NOT NULL,
    contrasena varchar2(20) NOT NULL,
    telefono INT PRIMARY KEY,
    correo varchar2(50) NOT NULL,
    sexo CHAR,
    fechanacimiento DATE,
    tarjeta INT NOT NULL
);

CREATE TABLE CLIENTEACTIVO(
    telefono INT,
    FOREIGN KEY (telefono) REFERENCES cliente(telefono),
    PRIMARY KEY(telefono)
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

CREATE TABLE RECOMENDACION {
  telefono INT,
  idPelicula INT,
  FOREIGN KEY (telefono) REFERENCES cliente(telefono),
  FOREIGN KEY (idPelicula) REFERENCES pelicula(idPelicula),
  PRIMARY KEY(telefono,idPelicula)
};

CREATE TABLE FIRMACLIENTECONTRATOCLIENTE(
    idContratoCliente INT PRIMARY KEY,
    fechainicio DATE NOT NULL,
    fechafin DATE NOT NULL,
    precio REAL NOT NULL ADD CONSTRAINT precioPositivo CHECK precio>=0,
    telefono INT,
    FOREIGN KEY telefono REFERENCES CLIENTE(telefono)
);

CREATE SEQUENCE secuencia_suministrarIdP; 

CREATE TABLE SUMINISTRAPELICULA(
	idPelicula INT PRIMARY KEY NOT NULL,
	titulo VARCHAR2(50) UNIQUE NOT NULL,
	anio INT, 
	director varchar2(50),
	productora varchar2(50),
	CIF INT
	
	FOREIGN KEY (CIF) REFERENCES PROVEEDOR(CIF)
);

CREATE TABLE PELICULAACTIVA(
    idPelicula INT PRIMARY KEY NOT NULL,
	
    FOREIGN KEY (idPelicula) REFERENCES PELICULA(idPelicula),
    PRIMARY KEY(idPelicula)
);

CREATE TABLE VE(
	idSesion int NOT NULL,
	idPelicula int NOT NULL,
	
	FOREIGN KEY (idSesion) REFERENCES SESIONCLIENTESESION,
	FOREIGN KEY (idPelicula) REFERENCES SUMINISTRAPELICULA,
	PRIMARY KEY (idSesion,idPelicula)
);

--Genera el id de la tabla de contrato-cliente
CREATE SEQUENCE secuencia_contratoCliente; 

CREATE TABLE CONTIENEN(
    idContratoCliente INT,
    nombrepack varchar(20),
    FOREIGN KEY idContratoCliente REFERENCES FIRMACLIENTECONTRATOCLIENTE(idContratoCliente),
    FOREIGN KEY nombrepack REFERENCES PACK(nombrepack),
);

CREATE TABLE PROVEEDOR(
    cif INT PRIMARY KEY,
    nombreempresa varchar(20) NOT NULL,
    telefonoempresa INT NOT NULL,
    correoempresa varchar(20) NOT NULL
);

CREATE TABLE FIRMAPROVEEDORCONTRATOPROVEEDOR(
    idContratoProveedor INT PRIMARY KEY,
    fechainicio DATE NOT NULL,
    fechafin DATE NOT NULL,
    precio REAL NOT NULL ADD CONSTRAINT precioPositivo CHECK precio>=0,
    cif INT,
    FOREIGN KEY cif REFERENCES PROVEEDOR(cif)
);

--Genera el id de la tabla de contrato-proveedor
CREATE SEQUENCE secuencia_contratoProveedor; 

CREATE TABLE FACTURACLIENTEPAGA(
    idfacturac INT PRIMARY KEY,
    fecha DATE NOT NULL,
    precio REAL NOT NULL ADD CONSTRAINT precioPositivo CHECK precio>=0,
    telefono INT,
    FOREIGN KEY telefono REFERENCES CLIENTE(telefono)
);

--Genera el id de la tabla de factura-cliente
CREATE SEQUENCE secuencia_facturaCliente; 

CREATE TABLE FACTURAPROVEEDORRECIBEDINERO(
    idfacturap INT PRIMARY KEY,
    fecha DATE NOT NULL,
    precio REAL NOT NULL ADD CONSTRAINT precioPositivo CHECK precio>=0,
    cif INT,
    FOREIGN KEY cif REFERENCES PROVEEDOR(cif)
);

--Genera el id de la tabla de factura-proveedor
CREATE SEQUENCE secuencia_facturaProveedor; 

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
BEFORE INSERT ON PACK
FOR EACH ROW WHEN(to_string(new.telefono).size() != 9)
BEGIN
    RAISE_APPLICATION_ERROR(-20010, 'TELEFONO INVALIDO');
END telefono_correcto;
/

-- Comprueba que la tarjeta tiene 16 dígitos --
CREATE OR REPLACE TRIGGER tarjeta_correcta
BEFORE INSERT OR UPDATE ON CLIENTE
FOR EACH ROW WHEN(to_string(new.tarjeta).size() != 16)
BEGIN
    RAISE_APPLICATION_ERROR(-20011, 'TARJETA INVALIDA');
END tarjeta_correcta;
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

-- Comprueba que el precio no sea negativo --
CREATE OR REPLACE TRIGGER precio_correcto
BEFORE INSERT ON PACK
FOR EACH ROW WHEN(new.precio < 0)
BEGIN
    RAISE_APPLICATION_ERROR(-20001, 'PRECIO INVALIDO');
END precio_correcto;
/

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

<<<<<<< HEAD

/*SELECT id_pelicula FROM peliculasactivas
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
  );*/
=======
--Crea id contrato cliente y comprueba que el cliente es activo
CREATE OR REPLACE TRIGGER insertar_ContratoCliente
  before insert on firmaClienteContratoCliente
  for each row
    DECLARE
      cli INTEGER
    BEGIN
      --el tlf que quiero insertar
      SELECT COUNT(*) INTO cli FROM ClienteActivo WHERE telefono=new.telefono; 
      IF (cli<1) THEN
        RAISE_APPLICATION_ERROR(-20100, 'INTENTO DE CREAR CONTRATO A UN CLIENTE NO ACTIVO')
      END IF
      SELECT secuencia_contratoCliente.nextval INTO :new.idContratoCliente FROM dual;
    END insertar_ContratoCliente;
/

--Crea id contrato proveedor
CREATE OR REPLACE TRIGGER insertar_ContratoProveedor 
  before insert on firmaProveedorContratoProveedor
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

CREATE OR REPLACE TRIGGER comprobarPeliculaInsertada
BEFORE INSERT ON PELICULA
FOR EACH ROW
DECLARE
	idP INTEGER;
BEGIN
	SELECT COUNT(*) INTO idP FROM PELICULA WHERE idPelicula = :new.idPelicula;
	
	IF (idP >= 1) THEN
		RAISE_APPLICATION_ERROR(-20015, 'PELÍCULA YA SUMINISTRADA');
	END IF;
END comprobarPeliculaInsertada;
/


CREATE OR REPLACE TRIGGER insertarNuevoIdPelicula
BEFORE INSERT ON PELICULA
FOR EACH ROW
BEGIN
	SELECT secuencia_suministrarIdP.nextval INTO :new.idPelicula FROM dual;
END insertarNuevoIdPelicula;
/


CREATE OR REPLACE TRIGGER desactivarPacks
BEFORE DELETE ON PACKS
FOR EACH ROW
DECLARE
	
>>>>>>> ee09d22d9910514f83b1075345ee3d350f257373
