-- ===============================================================================
-- ============================= SELECT DOTAZY ==================================
-- ===============================================================================

-- Jména všech čtenářů, kteří mají nějakou rezervaci
SELECT DISTINCT ctenar.jmeno
FROM ctenar INNER JOIN rezervace 
    ON (ctenar.id_ctenar=rezervace.id_ctenar);

-- Jména všech zaměstnanců, kteří vytvořili nějakou výpůjčku 
SELECT DISTINCT zamestnanec.jmeno
FROM zamestnanec INNER JOIN vypujcka 
    ON (zamestnanec.id_zamestnanec=vypujcka.id_zamestnanec_vytvoril);

-- Názvy všech titulů jejichž autor je 'George Orwell'
SELECT titul.nazev
FROM autor INNER JOIN autor_titul 
    ON (autor.id_autor=autor_titul.id_autor) 
    INNER JOIN titul 
    ON (titul.id_titul=autor_titul.id_titul)
WHERE autor.jmeno='George' AND autor.prijmeni='Orwell';

-- Kolik mají průměrně stran tituly od jednotlivých autorů
SELECT autor.jmeno, autor.prijmeni, AVG(titul.pocet_stran) AS prumerny_pocet_stran
FROM autor INNER JOIN autor_titul 
    ON (autor.id_autor=autor_titul.id_autor) 
    INNER JOIN titul 
    ON (titul.id_titul=autor_titul.id_titul)
GROUP BY autor.jmeno, autor.prijmeni;

-- Kolik napsali jednotlivý autoři titulů
SELECT autor.jmeno, autor.prijmeni, COUNT(autor_titul.id_titul) AS pocet_titulu
FROM autor LEFT OUTER JOIN autor_titul 
    ON (autor.id_autor=autor_titul.id_autor) 
GROUP BY autor.jmeno, autor.prijmeni;

-- Tituly které nebyly nikdy vypůčeny 
SELECT DISTINCT titul.nazev
FROM titul INNER JOIN exemplar 
    ON (titul.id_titul=exemplar.id_titul)
WHERE NOT EXISTS (
    SELECT * 
    FROM vypujcka
    WHERE vypujcka.id_exemplar = exemplar.id_exemplar
)    ;

-- Kterým čtenářům vytvořil výpůjčku uživatel 'xlogin42'
SELECT ctenar.jmeno, ctenar.prijmeni
FROM ctenar
WHERE id_ctenar IN (
    SELECT DISTINCT id_ctenar 
    FROM vypujcka INNER JOIN zamestnanec
        ON (vypujcka.id_zamestnanec_vytvoril=zamestnanec.id_zamestnanec)
    WHERE zamestnanec.login = 'xlogin42'
);