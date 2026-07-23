SELECT s.wanr,  w.kcauftragsnr, r.dnvl as Rohrdurchmesser, r.laenge as Rohrlaenge, w.sdmaterialnr
FROM waxserver.werkauftrag w,
     waxserver.verteiler st,
     waxserver.status s,
     waxserver.rohrleitung r
where w.waid=st.waid
and   w.waid=s.waid
and   w.waid=r.waid
and   s.storniert=0
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
and   (w.sdmaterialnr = 400041 or w.sdmaterialnr = 400040)
order by Rohrdurchmesser, Rohrlaenge;