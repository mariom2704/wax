SELECT   ROUND(SUM(rohrleitung.Laenge * rohrleitung.anzahl / 1000)) AS 'Länge in m', IF (werkauftrag.beschtyp > 100,  werkauftrag.beschtyp  - 65280, werkauftrag.beschtyp) AS beschtyp 
 FROM werkauftrag, rohrleitung, status where 
werkauftrag.waid = rohrleitung.waid 
 
and
werkauftrag.waid = 
 status.waid
and   status.freigabedatum >= 420050000
and   status.freigabedatum <= 423690000
group by werkauftrag.beschtyp 


SELECT status.wanr,  rohrleitung.waid,  ROUND( rohrleitung.Laenge * rohrleitung.anzahl / 1000) AS 'Länge in m',  werkauftrag.beschtyp  FROM werkauftrag, rohrleitung, status where 
werkauftrag.waid = rohrleitung.waid 
 
and
werkauftrag.waid = 
 status.waid
and   
status.freigabedatum >= 420050000
and   status.freigabedatum <= 420100000
