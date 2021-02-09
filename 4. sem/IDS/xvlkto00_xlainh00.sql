DROP TRIGGER ISBN_Check;
DROP TRIGGER ID_Zamestnanec_Check;
DROP TRIGGER ID_Vypujcka_Check;

DROP TABLE autor CASCADE CONSTRAINTS;
DROP TABLE autor_titul CASCADE CONSTRAINTS;
DROP TABLE ctenar CASCADE CONSTRAINTS;
DROP TABLE exemplar CASCADE CONSTRAINTS;
DROP TABLE rezervace CASCADE CONSTRAINTS;
DROP TABLE titul CASCADE CONSTRAINTS;
DROP TABLE vypujcka CASCADE CONSTRAINTS;
DROP TABLE zamestnanec CASCADE CONSTRAINTS;

DROP SEQUENCE ID_Zamestnanec_Seq;
DROP SEQUENCE ID_Vypujcka_Seq;

DROP MATERIALIZED VIEW ctenar_MView;

-- =============================================================
-- =============================================================
CREATE TABLE autor (
  id_autor int NOT NULL,
  jmeno varchar(256) NOT NULL,
  prijmeni varchar(256) NOT NULL,
  datum_narozeni date NOT NULL,
  datum_umrti date DEFAULT NULL,
  
  PRIMARY KEY (id_autor)
);

CREATE TABLE titul (
  id_titul int NOT NULL,
  nazev varchar(256) NOT NULL,
  ISBN varchar(256) NOT NULL,
  typ varchar(16) NOT NULL CHECK( typ='kniha' OR typ='audiokniha' ),
  pocet_stran int DEFAULT NULL,
  pocet_minut int DEFAULT NULL,
   
  PRIMARY KEY (id_titul)
);

CREATE TABLE autor_titul (
  id_autor_titul int NOT NULL,
  id_autor int NOT NULL,
  id_titul int NOT NULL,
  
  PRIMARY KEY (id_autor_titul)
);

CREATE TABLE exemplar (
  id_exemplar int NOT NULL,
  id_titul int NOT NULL,
  
  PRIMARY KEY (id_exemplar)
);

CREATE TABLE ctenar (
  id_ctenar int NOT NULL,
  jmeno varchar(256) NOT NULL,
  prijmeni varchar(256) NOT NULL,
  datum_narozeni date NOT NULL,
  adresa varchar(256) NOT NULL,
  telefon varchar(16) NOT NULL,
  login varchar(32) NOT NULL,
  heslo varchar(256) NOT NULL,
  
  PRIMARY KEY (id_ctenar)
);


CREATE TABLE rezervace (
  id_rezervace int NOT NULL,
  datum date NOT NULL,
  id_ctenar int NOT NULL,
  id_titul int NOT NULL,
  
  PRIMARY KEY (id_rezervace)
);

CREATE TABLE vypujcka (
  id_vypujcka int NOT NULL,
  datum_od date NOT NULL,
  datum_do date NOT NULL,
  id_zamestnanec_vytvoril int NOT NULL,
  id_zamestnanec_uzavrel int DEFAULT NULL,
  id_ctenar int NOT NULL,
  id_exemplar int NOT NULL,
  
  PRIMARY KEY (id_vypujcka)
);

CREATE TABLE zamestnanec (
  id_zamestnanec int,
  jmeno varchar(256) NOT NULL,
  prijmeni varchar(256) NOT NULL,
  datum_narozeni date NOT NULL,
  funkce varchar(256) NOT NULL,
  login varchar(256) NOT NULL,
  heslo varchar(32) NOT NULL,
  role varchar(16) NOT NULL CHECK( role='user' OR role='admin' ),
  
  PRIMARY KEY (id_zamestnanec)
);

ALTER TABLE autor_titul ADD FOREIGN KEY (id_autor) REFERENCES autor;
ALTER TABLE autor_titul ADD FOREIGN KEY (id_titul) REFERENCES titul;
ALTER TABLE exemplar ADD FOREIGN KEY (id_titul) REFERENCES titul;
ALTER TABLE rezervace ADD FOREIGN KEY (id_ctenar) REFERENCES ctenar;
ALTER TABLE rezervace ADD FOREIGN KEY (id_titul) REFERENCES titul;
ALTER TABLE vypujcka ADD FOREIGN KEY (id_zamestnanec_vytvoril) REFERENCES zamestnanec;
ALTER TABLE vypujcka ADD FOREIGN KEY (id_zamestnanec_uzavrel) REFERENCES zamestnanec;
ALTER TABLE vypujcka ADD FOREIGN KEY (id_ctenar) REFERENCES ctenar;
ALTER TABLE vypujcka ADD FOREIGN KEY (id_exemplar) REFERENCES exemplar;

CREATE SEQUENCE ID_Zamestnanec_Seq
START WITH 1
INCREMENT BY 1
CACHE 10;

CREATE SEQUENCE ID_Vypujcka_Seq
START WITH 1
INCREMENT BY 1
CACHE 10;

--==============================================================================
--========================== PROCEDURE Nova_Vypujcka ===========================
--==============================================================================
-- Vytvori novu vypujcku na zaklade uz vytvorenej rezervacie (rezervaciu zrusi)
CREATE OR REPLACE PROCEDURE Nova_Vypujcka (IN IDZamestnanec int, IDRezervacie IN int)
IS
  cursor volneExemplare is SELECT id_exemplar FROM exemplar INNER JOIN rezervace ON (exemplar.id_titul=rezervace.id_titul) 
                            WHERE NOT EXISTS (SELECT NULL FROM vypujcka WHERE vypujcka.id_exemplar = exemplar.id_exemplar)
                            AND rezervace.id_rezervace=IDRezervacie;
                         -- WHERE id_exemplar NOT IN (SELECT id_exemplar FROM vypujcka)
  exempl              volneExemplare%ROWTYPE; 
  currDate            DATE := SYSDATE;
  rezervCtenarID      int;
  NOEXEMPLFOUND       EXCEPTION;
BEGIN
  OPEN volneExemplare;

    FETCH volneExemplare INTO exempl;
    IF volneExemplare%NOTFOUND THEN
      raise NOEXEMPLFOUND;
    END IF;
    dbms_output.put_line('Dostupny exempl: '||exempl.id_exemplar);

    -- vyber ctenara s rezervaciou
    SELECT id_ctenar INTO rezervCtenarID FROM rezervace WHERE id_rezervace=IDRezervacie;
    -- vloz novu vypujcku
    INSERT INTO vypujcka (id_vypujcka, datum_od, datum_do, id_zamestnanec_vytvoril, id_zamestnanec_uzavrel, id_ctenar, id_exemplar)
    VALUES (ID_Vypujcka_Seq.nextval, currDate, ADD_MONTHS(currDate, 1), IDZamestnanec, NULL, rezervCtenarID, exempl.id_exemplar); 
    -- zmaz rezervaciu
    DELETE FROM rezervace WHERE (id_rezervace=IDRezervacie);

  CLOSE volneExemplare;

EXCEPTION
  WHEN NOEXEMPLFOUND THEN
    Raise_Application_Error (-20500,'Ziadny exemplar nie je dostupny k vypujcke.');
    --dbms_output.put_line('Ziadny exemplar nie je dostupny k vypujcke');
  WHEN NO_DATA_FOUND THEN
    Raise_Application_Error (-20200, 'Nenaslo sa id_ctenar');
  WHEN OTHERS THEN
    Raise_Application_Error (-20206, 'Error neznama chyba');
END;
/

--================================================================================
--========================== PROCEDURE Prosla_Vypujcka ===========================
--================================================================================
-- dotaz ci ma prosly_id_ctenar prosle vypujcky
CREATE OR REPLACE PROCEDURE Prosla_Vypujcka (prosly_id_ctenar IN int)
IS
  cursor vypujcky is SELECT * FROM vypujcka WHERE prosly_id_ctenar=id_ctenar;
  countprosle     number := 0;
  currDate        DATE := SYSDATE;
  ctenarExist     ctenar.id_ctenar%TYPE;
  vypujc          vypujcky%ROWTYPE;
  EMPTYVYPUJC     EXCEPTION;
BEGIN 
  SELECT id_ctenar INTO ctenarExist FROM ctenar WHERE prosly_id_ctenar=id_ctenar;
  OPEN vypujcky;
    FETCH vypujcky INTO vypujc;
    IF vypujcky%NOTFOUND THEN
      raise EMPTYVYPUJC;
    END IF;
  CLOSE vypujcky;

  dbms_output.put_line ('Ctenar ID['||prosly_id_ctenar||']');
  dbms_output.put_line ('-----------------------');
  FOR eachVypujcka in vypujcky
  LOOP
    IF eachVypujcka.datum_do < currDate THEN  -- vypujcka je uz po dnesnom datume
      countprosle := countprosle + 1;
      dbms_output.put_line ('Vypujcka ID['||eachVypujcka.id_vypujcka||'] je prosla');      
    END IF;
  END LOOP;
  
  IF countprosle = 0 THEN
    dbms_output.put_line('Zadne prosle vypujcky');
  END IF;

EXCEPTION
  WHEN NO_DATA_FOUND THEN
    Raise_Application_Error(-20100,'Ctenar s ID['||prosly_id_ctenar||'] neexistuje!');
  WHEN EMPTYVYPUJC THEN
    Raise_Application_Error(-20100,'Ctenar s ID['||prosly_id_ctenar||'] nema ziadnu vypujcku'); -- raise
  WHEN OTHERS THEN
    Raise_Application_Error (-20206, 'Error neznama chyba');
END;
/
-- ==============================================================================
-- =========================== TRIGGER ID_Vypujcka_Check ========================
-- ==============================================================================
-- kontroluje Sekvenciu ID_Vypujcka_Seq pre duplikatne hodnoty ID
CREATE OR REPLACE TRIGGER ID_Vypujcka_Check
BEFORE INSERT ON vypujcka FOR EACH ROW
DECLARE
  id_found vypujcka.id_vypujcka%TYPE;
BEGIN
  LOOP -- Ak sa najde vypujcka s rovnakym ID
    SELECT id_vypujcka INTO id_found FROM vypujcka WHERE :new.id_vypujcka=id_vypujcka;

    :new.id_vypujcka := ID_Vypujcka_Seq.nextval;  --increment new id
  END LOOP;

EXCEPTION
  WHEN NO_DATA_FOUND THEN -- Nove ID je unikatne
    RETURN;
END;
/

-- ==============================================================================
-- =========================== TRIGGER ID_Zamestnanec_Check =====================
-- ==============================================================================
-- Kontroluje pripad ( id_zamestnanec == NULL )
CREATE OR REPLACE TRIGGER ID_Zamestnanec_Check
BEFORE INSERT OR UPDATE ON ZAMESTNANEC FOR EACH ROW
DECLARE
  id_found zamestnanec.id_zamestnanec%TYPE;
BEGIN
  IF :new.id_zamestnanec IS NULL THEN
    :new.id_zamestnanec := id_zamestnanec_seq.nextval;
    
    LOOP -- Ak sa najde zamestnanec s rovnakym ID
      BEGIN
        SELECT id_zamestnanec INTO id_found FROM zamestnanec WHERE :new.id_zamestnanec=id_zamestnanec;
      EXCEPTION
        WHEN NO_DATA_FOUND THEN -- Nove ID je unikatne
          EXIT; -- Exit loop
      END;

      :new.id_zamestnanec := id_zamestnanec_seq.nextval;  --increment new id
    END LOOP;
  
  ELSE 
    RETURN;
  END IF;
END;
/

-- ==============================================================================
-- ============================ ISBN_Check TRIGGER =============================
-- ==============================================================================
CREATE OR REPLACE TRIGGER ISBN_Check
BEFORE INSERT OR UPDATE ON TITUL FOR EACH ROW
DECLARE
  numberint int;
  number_string VARCHAR2(50);
BEGIN
  SELECT REPLACE(:new.ISBN,'-',NULL) INTO numberint FROM dual;  -- '80-7309-999-3' ==> '8073099993'
  number_string := LTRIM(TO_CHAR(numberint,'9999999999999'));  
  numberint := 0;
  ----------------------------------------------------------------------
  IF LENGTH(number_string) = 13 THEN -- ISBN-13, since 2007
    FOR i IN 1 .. LENGTH(number_string)-1 LOOP
      IF MOD(i,2) = 0 THEN  --  even
        numberint := numberint + TO_NUMBER(SUBSTR(number_string,i,1))*3;
      ELSE
        numberint := numberint + TO_NUMBER(SUBSTR(number_string,i,1))*1;
      END IF;
    END LOOP;

    IF ( MOD(numberint,10) + TO_NUMBER(SUBSTR(number_string,13,1)) <> 10 ) THEN
       RAISE_APPLICATION_ERROR(-20100,'Invalid ISBN-13');
    END IF;
  ----------------------------------------------------------------------
  ELSIF LENGTH(number_string) = 10 THEN  -- old ISBN-10 
    FOR i IN 1 .. LENGTH(number_string) LOOP
      numberint := numberint + TO_NUMBER(SUBSTR(number_string,i,1))*i;
    END LOOP;

    IF MOD(numberint,11) > 0 THEN
      RAISE_APPLICATION_ERROR(-20100,'Invalid ISBN-10');
    END IF;
  -----------------------------------------------------------------------
  ELSE
    RAISE_APPLICATION_ERROR(-20100,'Invalid ISBN length');
  END IF;
EXCEPTION
  WHEN NO_DATA_FOUND THEN
    RETURN;
  WHEN OTHERS THEN
    Raise_Application_Error (-20206, 'Error neznama chyba');
END;
/

--   #################################################################################################
--  ///////////////////////////////////////// DATA INSERT ///////////////////////////////////////////
-- #################################################################################################
INSERT INTO autor
VALUES (1, 'George', 'Orwell', TO_DATE('1903-06-25','YYYY-MM-DD'), TO_DATE('1950-01-21','YYYY-MM-DD'));
INSERT INTO autor
VALUES (2, 'John', 'Steinbeck', TO_DATE('1902-02-27','YYYY-MM-DD'), TO_DATE('1968-12-20','YYYY-MM-DD'));
INSERT INTO autor
VALUES (3, 'Paulo', 'Coelho', TO_DATE('1947-08-24','YYYY-MM-DD'), '');

INSERT INTO titul
VALUES (1, 'O myších a lidech', '80-7218-497-0', 'kniha', '110', NULL); 
INSERT INTO titul
VALUES (2, 'Farma zvířat', '80-7299-021-7', 'kniha', '110', NULL);
INSERT INTO titul
VALUES (3, '1984', '80-7309-999-3', 'kniha', '264', NULL);
INSERT INTO titul 
VALUES (4, 'The Alchemist', '978-0061122415', 'kniha', '197', NULL);

INSERT INTO autor_titul
VALUES (1, 2, 1); 
INSERT INTO autor_titul
VALUES (2, 1, 2);
INSERT INTO autor_titul
VALUES (3, 1, 3);
INSERT INTO autor_titul
VALUES (4, 3, 4);

INSERT INTO exemplar
VALUES (1, 2); 
INSERT INTO exemplar
VALUES (2, 2);
INSERT INTO exemplar
VALUES (3, 3);
INSERT INTO exemplar
VALUES (4, 1);
INSERT INTO exemplar
VALUES (5, 1);
INSERT INTO exemplar
VALUES (6, 1);

INSERT INTO ctenar
VALUES (1, 'Josef', 'Novák', TO_DATE('1990-02-27','YYYY-MM-DD'), 'Božetěchova 2, 612 66 Brno', '123456789', 'xlogin00', '1a1dc91c907325c69271ddf0c944bc72'); 
INSERT INTO ctenar
VALUES (2, 'Petr', 'Novák', TO_DATE('1992-04-29','YYYY-MM-DD'), 'Kolejní 2905/2, 612 00 Brno', '123123123', 'xlogin01', '5f4dcc3b5aa765d61d8327deb882cf99'); 
INSERT INTO ctenar
VALUES (3, 'Tomáš', 'Novák', TO_DATE('1994-06-30','YYYY-MM-DD'), 'Kolejní 2, 612 00 Brno', '456456456', 'xlogin02', '955db0b81ef1989b4a4dfeae8061a9a6'); 
INSERT INTO ctenar
VALUES (4, 'Karel', 'Novák', TO_DATE('1996-08-02','YYYY-MM-DD'), 'Kolejní 2, 612 00 Brno', '789789789', 'xlogin03', '0d107d09f5bbe40cade3de5c71e9e9b7'); 

INSERT INTO rezervace
VALUES (1, TO_DATE('2016-03-16','YYYY-MM-DD'), 2, 1); 
INSERT INTO rezervace
VALUES (2, TO_DATE('2016-03-18','YYYY-MM-DD'), 1, 2);
INSERT INTO rezervace
VALUES (3, TO_DATE('2016-04-02','YYYY-MM-DD'), 1, 3);

INSERT INTO zamestnanec
VALUES (1, 'Jakub', 'Novák', TO_DATE('1989-02-27','YYYY-MM-DD'), 'knihovník', 'xlogin42', '1a1dc91c907325c69271ddf0c944bc72', 'user'); 
INSERT INTO zamestnanec
VALUES (2, 'Pavel', 'Novák', TO_DATE('1968-04-29','YYYY-MM-DD'), 'ředitel', 'xlogin24', '5f4dcc3b5aa765d61d8327deb882cf99', 'admin'); 
                                                                                         
INSERT INTO vypujcka
VALUES (1, TO_DATE('2016-03-16','YYYY-MM-DD'), TO_DATE('2016-04-16','YYYY-MM-DD'), 1, 2, 2, 1); 
INSERT INTO vypujcka
VALUES (2, TO_DATE('2016-03-18','YYYY-MM-DD'), TO_DATE('2017-03-18','YYYY-MM-DD'), 1, 2, 3, 2);
INSERT INTO vypujcka
VALUES (3, TO_DATE('2016-04-02','YYYY-MM-DD'), TO_DATE('2017-04-02','YYYY-MM-DD'), 1, 2, 3, 4);

COMMIT;
--    ################################################################################################ --
--   /---------------------------------------- EXPLAIN PLAN ----------------------------------------/  --
--  /------------------------------------------ A INDEX -------------------------------------------/   --
-- ################################################################################################    --

EXPLAIN PLAN FOR
SELECT titul.nazev, COUNT(exemplar.id_exemplar) AS pocet_exemplarov, COUNT(vypujcka.id_vypujcka) AS pocet_vypujcek
FROM titul LEFT OUTER JOIN exemplar ON (titul.id_titul=exemplar.id_titul) 
           LEFT OUTER JOIN vypujcka ON (vypujcka.id_exemplar=exemplar.id_exemplar)
GROUP BY titul.nazev
ORDER BY pocet_vypujcek DESC;

CREATE INDEX index_titul ON titul (nazev);
CREATE INDEX index_vypujcka ON vypujcka (id_exemplar);
--CREATE INDEX index_exemplar ON exemplar (id_titul);

EXPLAIN PLAN FOR
SELECT /*+ INDEX(titul index_titul) INDEX(vypujcka index_vypujcka) */ nazev, COUNT(exemplar.id_exemplar) AS pocet_exemplarov, COUNT(vypujcka.id_vypujcka) AS pocet_vypujcek
FROM titul LEFT OUTER JOIN exemplar ON (titul.id_titul=exemplar.id_titul) 
           LEFT OUTER JOIN vypujcka ON (vypujcka.id_exemplar=exemplar.id_exemplar)
GROUP BY titul.nazev
ORDER BY pocet_vypujcek DESC;

-- ==============================================================================================
--  )-)-)-)-)-)-)-)-)-)-)-)-)-)-)-)-) MATERIALIZOVANY POHLAD (-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(-(
-- ==============================================================================================
CREATE MATERIALIZED VIEW LOG ON ctenar WITH PRIMARY KEY, ROWID(prijmeni) INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW ctenar_MView
CACHE 
BUILD IMMEDIATE -- naplni pohlad hned po vytvoreni
REFRESH FAST ON COMMIT 
ENABLE QUERY REWRITE -- bude pouzivany optimalizatorom
AS SELECT C.prijmeni, count(C.prijmeni) as CountPrijmeni
FROM ctenar C
GROUP BY C.prijmeni;

-- =============================================================================================
--  ((((((((((((((((((((((((((((((((((((((( PRISTUPOVE PRAVA ))))))))))))))))))))))))))))))))))
-- =============================================================================================
GRANT ALL ON autor TO xlainh00;
GRANT ALL ON titul TO xlainh00;
GRANT ALL ON autor_titul TO xlainh00;
GRANT ALL ON exemplar TO xlainh00;
GRANT ALL ON ctenar TO xlainh00;
GRANT ALL ON rezervace TO xlainh00;
GRANT ALL ON vypujcka TO xlainh00;
GRANT ALL ON zamestnanec TO xlainh00;

GRANT EXECUTE ON Nova_Vypujcka TO xlainh00;
GRANT EXECUTE ON Prosla_Vypujcka TO xlainh00;

GRANT ALL ON ctenar_MView TO xlainh00;

--   ###########################################################################################
--  ///////////////////////////////// Ukazkova demonstracia ///////////////////////////////////
-- ###########################################################################################
-- Ukazka procedury Prosla_Vypujcka
set SERVEROUTPUT ON;
exec Prosla_Vypujcka(2);

-- Ukazka procedury Nova_Vypujcka
SELECT * FROM vypujcka;
SELECT * FROM rezervace;
exec Nova_Vypujcka(2, 1);
SELECT * FROM vypujcka;
SELECT * FROM rezervace;

-- Ukazka materializovaneho pohladu ctenar_MView
SELECT * FROM ctenar_MView;
INSERT INTO ctenar
VALUES (5, 'Nina', 'Nenovákova', TO_DATE('1995-10-04','YYYY-MM-DD'), 'Kolejní 2, 612 00 Brno', '789789789', 'xlogin04', '09gj7d0ff5bbe405ade3asdc71e8h4j7');
COMMIT;
SELECT * FROM ctenar_MView;
