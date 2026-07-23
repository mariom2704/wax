SELECT s.wanr, s.waid,v.eleid, r.dnvl as Rohrdurchmesser, r.laenge as Rohrlaenge, w.anzahl
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
and   (v.eleid = 1 || v.eleid = 2 || v.eleid = 5 || v.eleid = 40 || v.eleid = 39)
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
order by Rohrdurchmesser, Rohrlaenge;