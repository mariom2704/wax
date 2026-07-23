SELECT s.wanr, s.waid, r.dnvl as Rohrdurchmesser, r.laenge as Rohrlaenge, w.anzahl, r.rohranfang, r.rohrende 
FROM waxserver.werkauftrag w,
     waxserver.verteiler st,
     waxserver.status s,
     waxserver.rohrleitung r
where
w.waid=st.waid
and   w.waid=s.waid
and   w.waid=r.waid

and   s.storniert=0
and   r.laenge = 6000
and   r.dnvl >= 65
and   r.rohranfang = 20
and   r.rohrende   = 20
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
and s.waid NOT IN (SELECT  s.waid
FROM waxserver.werkauftrag w,
     waxserver.verteiler st,
     waxserver.status s,
     waxserver.rohrleitung r,
     waxserver.vk3listewa v
where w.waid=st.waid
and   w.waid=s.waid
and   w.waid=r.waid
and   w.waid=v.waid
and   s.storniert=0
and   r.laenge = 6000
and   r.dnvl >= 65
and   r.rohranfang = 20
and   r.rohrende   = 20)
order by Rohrdurchmesser, Rohrlaenge;