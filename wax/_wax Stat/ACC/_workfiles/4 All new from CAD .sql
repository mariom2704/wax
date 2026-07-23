/*
-- Daten vom Werkauftrag holen
*/
DROP TEMPORARY TABLE IF EXISTS test.joergstrang;
CREATE TEMPORARY TABLE IF NOT EXISTS test.joergstrang AS (
SELECT wanr, s.waid, 'StrangrohrData' AS classid, w.sdauftragsnr, w.sdpositionsnr, w.sdmaterialnr, w.kcauftragsnr, w.kcposnr, w.pspelement,  w.posnr, 
       'Nut' AS Rohrende,
       w.anzahl AS AnzahlRohre, w.beschbez
FROM waxserver.werkauftrag w,
     waxserver.status s
where   w.waid=s.waid
-- and   s.storniert=0
-- and   w.classid like 'CadWerkauftragData'
-- and   w.sdmaterialnr = 400020
-- and   s.freigabedatum > 414560000
-- and   s.freigabedatum < 416390000 
and s.wanr in
(
)
order by wanr,waid);


/*2222222222222*****************************
-- Daten aus vk3  fuer das Stdelement Bohrung Muffe
*/
DROP TEMPORARY TABLE IF EXISTS test.joergstrangfromav;
CREATE TEMPORARY TABLE IF NOT EXISTS test.joergstrangfromav AS( 
SELECT  vk.*,
   case vk.eleid 
        when 24 then 'Bohrung (24)'
        when 6 then 'RobbiMuffe (6)'
        when 7 then 'RobbiMuffe gek(7)'
        when 138 then 'Bohrung (24) (Schelle)'
        end AS strdeleid
     FROM status s,  vk3listewa vk, werkauftrag w
WHERE
s.waid = vk.waid
and
s.waid = w.waid
-- and   s.storniert=0
-- and   w.classid like 'CadWerkauftragData'
-- and   w.sdmaterialnr = 400020
-- and   s.freigabedatum > 414560000
-- and   s.freigabedatum < 416390000 
and s.wanr in
(
)
and  (vk.eleid = 6 or vk.eleid = 7 or vk.eleid = 24 or vk.eleid = 138)
group by s.wanr,s.waid
);



/*3333333333333333333***********************************************************
-- Daten aus vk3  fuer Rohrquali DN and friends
*/
DROP TEMPORARY TABLE IF EXISTS test.joergstrangfromavRlaengequali;
CREATE TEMPORARY TABLE IF NOT EXISTS test.joergstrangfromavRlaengequali AS( 
SELECT  vk.*,
case vk.eleid 
        when 51 then 'DINEN10217-1-Fe -glatt'
        when 52 then 'DINEN10255-Fe -glatt Gewinderohr'
        -- when 7 then 'DIN EN 10255 Gewrohr geschwei verzinkt'
        end AS rohrquali,
        anschlussdn AS dnval,
        anzahllaenge AS Rohrlaenge
FROM  vk3listewa vk, status s, werkauftrag w
WHERE
s.waid = vk.waid
and
s.waid = w.waid
-- and   s.storniert=0
-- and   w.classid like 'CadWerkauftragData'
-- and   w.sdmaterialnr = 400020
-- and   s.freigabedatum > 414560000
-- and   s.freigabedatum < 416390000  
and s.wanr in
(
)
and  (vk.eleid = 51 OR vk.eleid = 52)
group by s.wanr,s.waid
);




/*4444444444444444***************************************************
-- handschweiß Element aus der Vk3
*/
DROP TEMPORARY TABLE IF EXISTS test.joergAvelemente;
CREATE TEMPORARY TABLE IF NOT EXISTS test.joergAvelemente AS (
SELECT  s.wanr, s.waid,
        SUM(IF (eleid = 15, eleid, 0)) AS Rohranfang,
        SUM(IF (eleid = 6 OR eleid = 7 OR eleid = 24, dn, 0 )) AS ElemDN,
        
        SUM(IF (eleid = 6 OR eleid = 7 OR eleid = 24, anzahllaenge, 0)) AS AnMuffenStutzenMaschine,      
        SUM(IF (eleid = 2, anzahllaenge, 0)) AS WNStutzen,                                                                                       -- 1 
        SUM(IF (eleid = 1 OR eleid = 4, anzahllaenge, 0)) AS Rohrstutzen,                                                                        -- 2
        SUM(IF (eleid = 9 OR eleid = 15 OR eleid = 16, anzahllaenge, 0)) AS ScheBoBl,                                                            -- 3
        SUM(IF (eleid = 5 OR eleid = 35 OR eleid = 41, anzahllaenge, 0)) AS handschwMuffNippel,                                                  -- 4
        SUM(IF (eleid = 28 OR eleid = 29 OR eleid = 36 OR eleid = 63 , anzahllaenge, 0)) AS Boegen,                                              -- 5
        SUM(IF (eleid = 25 OR eleid = 26 OR eleid = 27 , anzahllaenge, 0)) AS ReduEinzieh,                                                       -- 6
        SUM(IF (eleid = 8 OR eleid = 11 OR eleid = 12 OR eleid = 54 OR eleid = 55 OR eleid = 57 OR eleid = 58, anzahllaenge, 0)) AS Flachflansch -- 7
FROM  vk3listewa, status s, werkauftrag w
where vk3listewa.waid = s.waid 
and
w.waid =  s.waid
-- and   w.classid like 'CadWerkauftragData'
-- and   s.freigabedatum > 414560000
-- and   s.freigabedatum < 416390000  
and  s.wanr  in
(
)
group by s.wanr,s.waid);



-- *************************************
SELECT test.joergstrang.wanr, test.joergstrang.waid, test.joergstrang.classid, test.joergstrang.sdauftragsnr,
test.joergstrang.sdpositionsnr, test.joergstrang.sdmaterialnr,
test.joergstrang.kcauftragsnr,test.joergstrang.kcposnr,test.joergstrang.pspelement,test.joergstrang.posnr,

test.joergstrangfromavRlaengequali.dnval, test.joergstrangfromavRlaengequali.Rohrlaenge/test.joergstrang.AnzahlRohre*1000  AS Rohrlaenge, test.joergstrangfromavRlaengequali.rohrquali, 
IF (test.joergAvelemente.Rohranfang = 15, 'Scheibe', 'Nut') AS Rohranfang,
test.joergstrang.Rohrende,
test.joergstrangfromav.strdeleid,
ElemDN,
AnMuffenStutzenMaschine,
test.joergstrangfromavRlaengequali.dnval AS Rohrdurchmesser,
test.joergstrang.AnzahlRohre,
test.joergstrang.beschbez,
 
WNStutzen AS 'WN-Stutzen',  
Rohrstutzen, 
ScheBoBl  AS 'Sche./Bo./Bl.', 
handschwMuffNippel  AS 'handschw. Muff./Nippel', 
Boegen AS 'Bögen', 
ReduEinzieh  AS 'Redu./Einzieh.', 
Flachflansch
 FROM  test.joergstrang 
LEFT JOIN test.joergAvelemente ON (test.joergstrang.waid = test.joergAvelemente.waid) 
LEFT JOIN test.joergstrangfromavRlaengequali  ON (test.joergstrangfromavRlaengequali.waid = test.joergAvelemente.waid) 
LEFT JOIN test.joergstrangfromav ON (test.joergAvelemente.waid = test.joergstrangfromav.waid);


