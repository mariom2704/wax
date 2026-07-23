SELECT s.wanr, s.waid, r.dnvl as Rohrdurchmesser, r.laenge as Rohrlaenge, w.anzahl, r.rohranfang, r.rohrende, ele.bezeichnung, ele.abstrel, ele.laenge, ele.winkel, ele.makeanlage
FROM waxserver.werkauftrag w,
     waxserver.verteiler st,
     waxserver.status s,
     waxserver.rohrleitung r,
     waxserver.element ele,
     waxserver.FertSaegeNutRohre  utRohre 
where 
      w.waid=st.waid
and   w.waid=s.waid
and   w.waid=r.waid
and   ele.waid=r.waid
and   utRohre.waid = w.waid
and   s.storniert=0
and   ele.makeanlage = 0
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
group by ele.abstrel
order by wanr,waid;