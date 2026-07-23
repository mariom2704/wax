
DROP TEMPORARY TABLE IF EXISTS test.joergrohr;
CREATE TEMPORARY TABLE IF NOT EXISTS test.joergrohr AS (
SELECT  wanr, s.waid, w.classid, w.sdauftragsnr, w.sdpositionsnr,  w.sdmaterialnr, w.kcauftragsnr, w.kcposnr, w.pspelement,  w.posnr, r.dnvl, r.laenge as Rohrlaenge,
   case r.rohrquali 
        when 21 then 'DINEN10217-1-Fe -glatt'
        when 22 then 'DINEN10255-Fe -glatt Gewinderohr'
        when 7 then 'DIN EN 10255 Gewrohr geschwei verzinkt'
        when 4 then 'DIN2448 Rohr nahtlos normalwandig'
        end AS rohrquali,
   case rohranfang 
        when 8  then 'Flachflansch' 
        when 15 then 'Scheibe'
        when 16 then 'Boden'
        when 20 then 'Nut'
        when 21 then 'Gewinde'
        when 23 then 'Glatt'
        when 57  then 'Flachflansch' 
        when 58  then 'Flachflansch' 
        end AS Rohranfang,
    case rohrende 
        when 8  then 'Flachflansch' 
        when 15 then 'Scheibe'
        when 16 then 'Boden'
        when 20 then 'Nut'
        when 21 then 'Gewinde'
        when 23 then 'Glatt'
        when 57  then 'Flachflansch' 
        when 58  then 'Flachflansch' 
        end AS Rohrende,
        r.dnvl as Rohrdurchmesser,
        w.anzahl AS AnzahlRohre, w.beschbez
FROM waxserver.werkauftrag w,
     waxserver.status s,
     waxserver.rohrleitung r
where w.waid=s.waid
and   w.waid=r.waid
and   s.storniert=0
and s.wanr in
(
)
group by wanr, s.waid,  Rohrdurchmesser
order by wanr, Rohrlaenge);



-- 2222222222222222222*************************************
DROP TEMPORARY TABLE IF EXISTS test.joergrohrfromav;
CREATE TEMPORARY TABLE IF NOT EXISTS test.joergrohrfromav AS( 
SELECT s.waid,  vk.DN as Muffen_DN,
   case vk.eleid 
        when 39 then 'Stan. Muffe'
        when 40 then 'Nutstutzen'
        when 24 then 'Bohrung (24)'
        when 6 then 'RobbiMuffe (6)'
        when 7 then 'RobbiMuffe gek(7)'
        when 138 then 'Bohrung (24) (Schelle)'
        end
        AS strdeleid
     FROM status s, vk3listewa vk
WHERE
s.waid = vk.waid
and (vk.eleid = 6 or vk.eleid = 7 or vk.eleid = 24 or vk.eleid = 138 or vk.eleid = 39 or vk.eleid = 40 )
and s.wanr in
(
)
group by s.wanr,s.waid
);



-- 333333333333*************************************
DROP TEMPORARY TABLE IF EXISTS test.joergAvelemente;
CREATE TEMPORARY TABLE IF NOT EXISTS test.joergAvelemente AS (
SELECT  s.wanr, s.waid,
        SUM(IF (eleid = 6 OR eleid = 7 OR eleid = 24 or eleid = 39 OR eleid = 40, vk3listewa.dn, 0 )) AS ElemDN,
        SUM(IF (eleid = 6 OR eleid = 7 OR eleid = 24 or eleid = 39 or eleid = 40, anzahllaenge, 0)) AS AnMuffenStutzenMaschine, 
        SUM(IF (eleid = 2, anzahllaenge, 0)) AS WNStutzen,                                                                                         -- 1 
        SUM(IF (eleid = 1  OR eleid = 4, anzahllaenge, 0)) AS Rohrstutzen,                                                                         -- 2
        SUM(IF (eleid = 9  OR eleid = 15 OR eleid = 16, anzahllaenge, 0)) AS ScheBoBl,                                                             -- 3
        SUM(IF (eleid = 5  OR eleid = 35 OR eleid = 41, anzahllaenge, 0)) AS handschwMuffNippel,                                                   -- 4
        SUM(IF (eleid = 28 OR eleid = 29 OR eleid = 36 OR eleid = 63 , anzahllaenge, 0)) AS Boegen,                                                -- 5
        SUM(IF (eleid = 25 OR eleid = 26 OR eleid = 27 , anzahllaenge, 0)) AS ReduEinzieh,                                                         -- 6
        SUM(IF (eleid = 8  OR eleid = 11 OR eleid = 12 OR eleid = 54 OR eleid = 55 OR eleid = 57 OR eleid = 58, anzahllaenge, 0)) AS Flachflansch  -- 7
FROM  vk3listewa, status s, werkauftrag
where vk3listewa.waid = s.waid 
and werkauftrag.waid =  s.waid
and s.wanr in
(
)
group by s.wanr,s.waid);



-- *************************************
SELECT test.joergrohr.wanr, test.joergrohr.waid, test.joergrohr.classid, joergrohr.sdauftragsnr,
test.joergrohr.sdpositionsnr, test.joergrohr.sdmaterialnr,
test.joergrohr.kcauftragsnr,test.joergrohr.kcposnr,test.joergrohr.pspelement,
test.joergrohr.posnr,test.joergrohr.dnvl, test.joergrohr.Rohrlaenge, test.joergrohr.rohrquali, 
test.joergrohr.Rohranfang,
test.joergrohr.Rohrende,
test.joergrohrfromav.strdeleid,
ElemDN,
AnMuffenStutzenMaschine,
test.joergrohr.Rohrdurchmesser,
test.joergrohr.AnzahlRohre,
test.joergrohr.beschbez,
 
WNStutzen AS 'WN-Stutzen',  
Rohrstutzen, 
ScheBoBl  AS 'Sche./Bo./Bl.', 
handschwMuffNippel  AS 'handschw. Muff./Nippel', 
Boegen AS 'BÃ¶gen', 
ReduEinzieh  AS 'Redu./Einzieh.', 
Flachflansch
 FROM  test.joergrohr 
LEFT JOIN test.joergAvelemente ON (test.joergrohr.waid = test.joergAvelemente.waid) 
LEFT JOIN test.joergrohrfromav ON (test.joergAvelemente.waid = test.joergrohrfromav.waid);

