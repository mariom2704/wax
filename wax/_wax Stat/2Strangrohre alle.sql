SELECT  s.wanr, s.waid, SUM(r.anzahl) AS AnzhlRohre
FROM waxserver.rohrleitung r,
     waxserver.status s,
     waxserver.strangrohr st
where s.waid=r.waid
and s.waid=st.waid
and   s.storniert=0
and s.wanr > 0

and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*

group by s.wanr;