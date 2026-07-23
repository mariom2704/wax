SELECT s.wanr, r.laenge, r.dnvl,
(select count(anzahl) from waxserver.element e 
      where e.waid = r.waid 
      and (id = 1 or id = 2 or id = 5 or id = 35 or id = 24 or id = 4)) as anzahlEL

FROM waxserver.rohrleitung r,
     waxserver.status s

where r.laenge < 1500
and s.waid=r.waid
and   s.storniert=0
and s.wanr > 0

and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*

order by s.wanr