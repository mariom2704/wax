DROP TEMPORARY TABLE IF EXISTS tmp.joergstrang;

CREATE TEMPORARY TABLE IF NOT EXISTS tmp.joergstrang AS (
SELECT wanr, s.waid, w.sdauftragsnr, w.sdpositionsnr, w.kcauftragsnr, w.kcposnr, w.pspelement,  w.posnr, r.dnvl, r.laenge as Rohrlaenge,
    case rohranfang 
        when 15 then 'Scheibe'
        when 20 then 'Nut'
        when 21 then 'Gewinde'
        end AS Rohranfang,
    case rohrende 
        when 15 then 'Scheibe'
        when 20 then 'Nut'
        when 21 then 'Gewinde'
        end AS Rohrende,
    case vk.eleid 
        when 24 then 'Bohrung (24)'
        when 6 then 'RobbiMuffe (6)'
        when 7 then 'RobbiMuffe gek(7)'
        end AS Bezeichnung,
       st.muffenDN as Muffen_DN, r.dnvl as Rohrdurchmesser,
       vk.anzahllaenge /w.anzahl as Anz_Muffen_pro_Rohr , 
       vk.anzahllaenge  AS Gesamtzahl_Muffen, muffenGekehlt,
        w.anzahl AS AnzahlRohre, w.beschbez
FROM waxserver.werkauftrag w,
     waxserver.strangrohr st,
     waxserver.status s,
     waxserver.rohrleitung r,
     waxserver.vk3listewa vk,
     waxstamm.bauelementgruppen b
where w.waid=st.waid
and   w.waid=s.waid
and   w.waid=r.waid
and   w.waid=vk.waid
and   (vk.eleid = 6 or vk.eleid = 7 or vk.eleid = 24)
and   st.muffenID=b.elemid
and   s.storniert=0
and   (s.wanr = 148547 OR  s.wanr =  148543)
group by wanr, s.waid, Anz_Muffen_pro_Rohr, Rohrdurchmesser, Muffen_DN
order by wanr, Rohrlaenge);


DROP TEMPORARY TABLE IF EXISTS tmp.joergAvelemente;
CREATE TEMPORARY TABLE IF NOT EXISTS tmp.joergAvelemente AS (
SELECT  status.wanr, status.waid,
        SUM(IF (eleid = 2, anzahllaenge, 0)) AS 'WN-Stutzen',                                                                                           -- 1 
        SUM(IF (eleid = 1  OR eleid = 4, anzahllaenge, 0)) AS 'Rohrtutzen',                                                                             -- 2
        SUM(IF (eleid = 9 OR eleid = 15 OR eleid = 16, anzahllaenge, 0)) AS 'Sche./Bo./Bl.',                                                            -- 3
        SUM(IF (eleid = 5 OR eleid = 35 OR eleid = 41, anzahllaenge, 0)) AS 'handschw. Muff./Nippel',                                                   -- 4
        SUM(IF (eleid = 28 OR eleid = 29 OR eleid = 36 OR eleid = 63 , anzahllaenge, 0)) AS 'Boegen',                                                  -- 5
        SUM(IF (eleid = 25 OR eleid = 26 OR eleid = 27 , anzahllaenge, 0)) AS 'Redu./Einzieh.',                                                         -- 6
        SUM(IF (eleid = 8 OR eleid = 11 OR eleid = 12 OR eleid = 54 OR eleid = 55 OR eleid = 57 OR eleid = 58, anzahllaenge, 0)) AS 'Flachflansch'      -- 7
FROM  vk3listewa, status 
where vk3listewa.waid = status.waid and (status.waid = 534430  OR status.waid = 534431 OR status.waid = 534432)
group by status.wanr,status.waid);


SELECT * FROM tmp.joergstrang LEFT JOIN tmp.joergAvelemente  
ON (tmp.joergstrang.waid = tmp.joergAvelemente.waid) where  tmp.joergstrang.waid = tmp.joergAvelemente.waid;
