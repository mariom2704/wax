	SELECT s.wanr, s.waid,  SUM(r.anzahl) AS Anzahl
FROM waxserver.rohrleitung r,
     waxserver.status s,
     waxserver.strangrohr st
where  s.waid=r.waid
and s.waid=st.waid
and   s.storniert=0
and s.wanr > 0
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
and s.wanr not in 
(SELECT s.wanr
FROM waxserver.rohrleitung r,
     waxserver.status s,
     waxserver.strangrohr st
where r.laenge > 6000
and s.waid=r.waid
and s.waid=st.waid
and   s.storniert=0
and s.wanr > 0

and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*) group by s.wanr;